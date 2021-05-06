#pragma once

#include "jittypes.h"
#include "types.h"

namespace asmjit
{
	namespace x86
	{
		class Assembler;
	}
}

namespace dsp56k
{
	class DSP;

	class JitDspRegs
	{
	public:
		JitDspRegs(asmjit::x86::Assembler& _a, DSP& _dsp);

		~JitDspRegs();

		template<typename T>
		void getR(T _dst, int _agu)
		{
			if(!isLoaded(LoadedRegR0 + _agu))
				loadAGU(_agu);

			m_asm.movd(_dst, asmjit::x86::xmm(xmmR0+_agu));
		}

		template<typename T>
		void getN(T _dst, int _agu)
		{
			const auto xm(asmjit::x86::xmm(xmmR0+_agu));

			m_asm.pshufd(xm, xm, asmjit::Imm(0xe1));		// swap lower two words to get N in word 0
			m_asm.movd(_dst, xm);
			m_asm.pshufd(xm, xm, asmjit::Imm(0xe1));		// swap back
		}

		template<typename T>
		void getM(T _dst, int _agu)
		{
			const auto xm(asmjit::x86::xmm(xmmR0+_agu));
			m_asm.pshufd(xm, xm, asmjit::Imm(0xc6));		// swap words 0 and 2 to ret M in word 0
			m_asm.movd(_dst, xm);
			m_asm.pshufd(xm, xm, asmjit::Imm(0xc6));		// swap back
		}

		asmjit::x86::Gp getPC();
		asmjit::x86::Gp getSR();
		asmjit::x86::Gp getLC();
		asmjit::x86::Gp getLA();
		void getALU(asmjit::x86::Gp _dst, int _alu);
		void setALU(int _alu, asmjit::x86::Gp _src);

	private:
		enum LoadedRegs
		{
			LoadedRegR0,	LoadedRegR1,	LoadedRegR2,	LoadedRegR3,	LoadedRegR4,	LoadedRegR5,	LoadedRegR6,	LoadedRegR7,
			LoadedRegA,		LoadedRegB,
			LoadedRegX,		LoadedRegY,
			LoadedRegLC,	LoadedRegLA,
			LoadedRegSR,	LoadedRegPC,
		};

		void loadDSPRegs();
		void storeDSPRegs();

		void loadAGU(int _agu);
		void loadALU(int _alu);
		void loadXY(int _xy);

		void storeAGU(int _agu);
		void storeALU(int _alu);
		void storeXY(int _xy);

		void load24(const asmjit::x86::Gp& _dst, TReg24& _src);
		void store24(TReg24& _dst, const asmjit::x86::Gp& _src);

		bool isLoaded(const uint32_t _reg) const		{ return m_loadedRegs & (1<<_reg); }
		void setLoaded(const uint32_t _reg)				{ m_loadedRegs |= (1<<_reg); }
		void setUnloaded(const uint32_t _reg)			{ m_loadedRegs &= ~(1<<_reg); }

		asmjit::x86::Assembler& m_asm;
		DSP& m_dsp;
		uint32_t m_loadedRegs = 0;
	};
}
