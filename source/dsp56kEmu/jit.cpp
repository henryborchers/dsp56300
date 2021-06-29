#include "jit.h"

#include "dsp.h"
#include "jitblock.h"
#include "jithelper.h"
#include "jitops.h"

#include "asmjit/core/jitruntime.h"
#include "asmjit/x86/x86assembler.h"

#if defined(_WIN32) && defined(_WIN64)
#define JIT_VTUNE_PROFILING
#include "../vtuneSdk/include/jitprofiling.h"
#endif


using namespace asmjit;
using namespace x86;

/*
	The idea of this JIT is to keep all relevant DSP registers in x64 registers. Register allocation looks like this:

	RAX	= func ret val                             |               XMM00 = DSP AGU 0 [0,M,N,R]
	RBX							*                  |               XMM01 = DSP AGU 1 [0,M,N,R]
	RCX	= func arg 0 (Microsoft)                   |               XMM02 = DSP AGU 2 [0,M,N,R]
	RDX	= func arg 1 / temp			               |               XMM03 = DSP AGU 3 [0,M,N,R]
	RBP	= temp					*                  |               XMM04 = DSP AGU 4 [0,M,N,R]
	RSI	=    						               |               XMM05 = DSP AGU 5 [0,M,N,R]
	RDI	= func arg 0 (linux)    				   |               XMM06 = DSP AGU 6 [0,M,N,R]
	RSP							*	               |               XMM07 = DSP AGU 7 [0,M,N,R]
	R8  = func arg 2 / DSP Status Register		   |               XMM08 = DSP A
	R9  = func arg 3 / DSP Program Counter		   |               XMM09 = DSP B
	R10 = DSP Loop Counter			               |               XMM10 = DSP X
	R11 = DSP Loop Address			               |               XMM11 = DSP Y
	R12	= temp                  *                  |               XMM12 = last modified ALU for lazy SR updates
	R13	= temp                  *                  |               XMM13 = temp
	R14	= temp                  *                  |               XMM14 = temp
	R15	= temp                  *                  |               XMM15 = temp

	* = callee-save = we need to restore the previous register state before returning
*/

namespace dsp56k
{
	constexpr bool g_traceOps = false;

	Jit::Jit(DSP& _dsp) : m_dsp(_dsp)
	{
		m_jitCache.resize(_dsp.memory().size(), JitCacheEntry{&Jit::create, nullptr});
	}

	Jit::~Jit()
	{
		for(size_t i=0; i<m_jitCache.size(); ++i)
		{
			auto* const entry = m_jitCache[i].block;
			if(entry)
				destroy(entry);
		}
	}

	void Jit::exec()
	{
		// loop processing
		if(m_dsp.sr_test(SR_LF))
		{
			if(m_dsp.getPC().var == m_dsp.regs().la.var + 1)
			{
				auto& lc = m_dsp.regs().lc.var;
				if(lc <= 1)
				{
					m_dsp.do_end();					
				}
				else
				{
					--lc;
					m_dsp.setPC(hiword(m_dsp.regs().ss[m_dsp.ssIndex()]));
				}
			}
		}

		const auto pc = static_cast<TWord>(m_dsp.getPC().var);

//		LOG("Exec @ " << HEX(pc));

		// get JIT code
		auto& cacheEntry = m_jitCache[pc];

		assert(cacheEntry.func);

		(this->*cacheEntry.func)(pc, cacheEntry.block);
	}

	void Jit::notifyProgramMemWrite(TWord _offset)
	{
		destroy(_offset);
	}

	void Jit::emit(const TWord _pc)
	{
		AsmJitLogger logger;
		AsmJitErrorHandler errorHandler;
		CodeHolder code;

//		code.setLogger(&logger);
		code.setErrorHandler(&errorHandler);
		code.init(m_rt.environment());

		Assembler m_asm(&code);

		JitBlock* b;

		{
			b = new JitBlock(m_asm, m_dsp);

			if(!b->emit(_pc, m_jitCache, m_volatileP))
			{
				LOG("FATAL: code generation failed for PC " << HEX(_pc));
				delete b;
				return;
			}
		}

		m_asm.ret();

		JitBlock::JitEntry func;

		const auto err = m_rt.add(&func, &code);

		if(err)
		{
			const auto* const errString = DebugUtils::errorAsString(err);
			LOG("JIT failed: " << err << " - " << errString);
			return;
		}

		b->setFunc(func);

		const auto first = b->getPCFirst();
		const auto last = first + b->getPMemSize();

		for(auto i=first; i<last; ++i)
		{			
			m_jitCache[i].block = b;
			m_jitCache[i].func = i == first ? &Jit::run : &Jit::recreate;
		}

#ifdef JIT_VTUNE_PROFILING
		if(iJIT_IsProfilingActive() == iJIT_SAMPLING_ON)
		{
			iJIT_Method_Load jmethod = {0};
			jmethod.method_id = iJIT_GetNewMethodID();
			std::stringstream ss;
			char temp[64];
			sprintf(temp, "$%06x-$%06x", first, last-1);
			jmethod.method_name = temp;
			jmethod.class_file_name = const_cast<char*>("dsp56k::Jit");
			jmethod.source_file_name = __FILE__;
			jmethod.method_load_address = static_cast<void*>(func);
			jmethod.method_size = static_cast<unsigned int>(code.codeSize());

			iJIT_NotifyEvent(iJVM_EVENT_TYPE_METHOD_LOAD_FINISHED, &jmethod);
		}
#endif

//		LOG("New block generated @ " << HEX(_pc) << " up to " << HEX(_pc + b->getPMemSize() - 1) << ", instruction count " << b->getEncodedInstructionCount() << ", disasm " << b->getDisasm());
	}

	void Jit::destroy(JitBlock* _jitBlock)
	{
		const auto first = _jitBlock->getPCFirst();
		const auto last = first + _jitBlock->getPMemSize();

//		LOG("Destroying JIT block at PC " << HEX(first) << ", length " << _block->getPMemSize());

		for(auto i=first; i<last; ++i)
		{
			m_jitCache[i].block = nullptr;
			m_jitCache[i].func = &Jit::create;
		}

		if(_jitBlock->getPMemSize() == 1)
		{
			// if a 1-word-op, cache it
			auto& cacheEntry = m_jitCache[first];

			TWord opA;
			TWord opB;
			m_dsp.memory().getOpcode(first, opA, opB);

			if(cacheEntry.singleOpCache.find(opA) == cacheEntry.singleOpCache.end())
			{
//				LOG("Caching 1-word-op " << HEX(opA) << " at PC " << HEX(first));

				cacheEntry.singleOpCache.insert(std::make_pair(opA, _jitBlock));
				return;
			}
		}

		m_rt.release(_jitBlock->getFunc());

		delete _jitBlock;
	}

	void Jit::destroy(TWord _pc)
	{
		const auto block = m_jitCache[_pc].block;
		if(!block)
			return;
		destroy(block);
	}

	void Jit::run(TWord _pc, JitBlock* _block)
	{
		_block->exec();

		m_dsp.m_instructions += _block->getExecutedInstructionCount();

		m_dsp.setPC(_block->nextPC());

		if(g_traceOps)
		{
			const TWord lastPC = _pc + _block->getPMemSize() - _block->getLastOpSize();
			TWord op, opB;
			m_dsp.mem.getOpcode(lastPC, op, opB);
			m_dsp.traceOp(lastPC, op, opB, _block->getLastOpSize());

			// make the diff tool happy, interpreter traces two ops. For the sake of simplicity, just trace it once more
			if(_block->getDisasm().find("rep ") == 0)
				m_dsp.traceOp(lastPC, op, opB, _block->getLastOpSize());
		}

		// if JIT code has written to P memory, destroy a JIT block if present at the write location
		if(_block->pMemWriteAddress() != g_pcInvalid)
		{
			const TWord addr = _block->pMemWriteAddress();

			if (m_jitCache[addr].block)
				m_volatileP.insert(addr);

			notifyProgramMemWrite(_block->pMemWriteAddress());
			m_dsp.notifyProgramMemWrite(_block->pMemWriteAddress());
		}
	}

	void Jit::create(const TWord _pc, JitBlock* _block)
	{
		auto& cacheEntry = m_jitCache[_pc];

		if(m_jitCache[_pc+1].block != nullptr)
		{
			// we will generate a 1-word op, try to find in single op cache
			TWord opA;
			TWord opB;
			m_dsp.memory().getOpcode(_pc, opA, opB);

			auto it = cacheEntry.singleOpCache.find(opA);

			if(it != cacheEntry.singleOpCache.end())
			{
//				LOG("Returning 1-word-op " << HEX(opA) << " at PC " << HEX(_pc));
				cacheEntry.block = it->second;
				cacheEntry.singleOpCache.erase(it);
				cacheEntry.func = &Jit::run;
				run(_pc, cacheEntry.block);
				return;
			}
		}
		emit(_pc);
		run(_pc, cacheEntry.block);
	}

	void Jit::recreate(const TWord _pc, JitBlock* _block)
	{
		// there is code, but the JIT block does not start at the PC position that we want to run. We need to throw the block away and regenerate
//		LOG("Unable to jump into the middle of a block, destroying existing block & recreating from " << HEX(pc));
		destroy(_block);
		create(_pc, _block);
	}
}
