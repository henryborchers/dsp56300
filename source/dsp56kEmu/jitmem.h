#pragma once

#include "jitregtracker.h"
#include "jitregtypes.h"
#include "types.h"

namespace dsp56k
{
	class JitBlock;

	class Jitmem
	{
	public:
		Jitmem(JitBlock& _block) : m_block(_block) {}

		void mov(TReg24& _dst, const JitReg128& _src);
		void mov(TReg48& _dst, const JitReg128& _src);
		void mov(TReg56& _dst, const JitReg128& _src);

		void mov(const JitReg128& _dst, TReg24& _src);
		void mov(const JitReg128& _dst, TReg56& _src);
		void mov(const JitReg128& _dst, TReg48& _src);

		void mov(const asmjit::x86::Gp& _dst, const TReg24& _src);
		void mov(const asmjit::x86::Gp& _dst, const TReg48& _src);
		void mov(const asmjit::x86::Gp& _dst, const TReg56& _src);

		void mov(const TReg24& _dst, const asmjit::x86::Gp& _src);
		void mov(const TReg48& _dst, const asmjit::x86::Gp& _src);
		void mov(const TReg56& _dst, const asmjit::x86::Gp& _src);

		void mov(uint64_t& _dst, const asmjit::x86::Gp& _src) const;
		void mov(uint32_t& _dst, const asmjit::x86::Gp& _src) const;
		void mov(uint8_t& _dst, const asmjit::x86::Gp& _src) const;

		void mov(const asmjit::x86::Gp& _dst, const uint64_t& _src) const;
		void mov(const asmjit::x86::Gp& _dst, const uint32_t& _src) const;
		void mov(const asmjit::x86::Gp& _dst, const uint8_t& _src) const;

		void mov(void* _dst, void* _src, uint32_t _size);
		void mov(void* _dst, const JitReg& _src, uint32_t _size);

		template<typename T, unsigned int B>
		asmjit::x86::Mem ptr(const JitReg64& _temp, const RegType<T, B>& _reg)
		{
			return ptr<T>(_temp, &_reg.var);
		}

		template<typename T>
		asmjit::x86::Mem ptr(const JitReg64& _temp, const T* _t) const;

		template<typename T>
		void ptrToReg(const JitReg64& _r, const T* _t) const;

		template<typename T>
		asmjit::x86::Mem makePtr(const void* _base, const T* _member)
		{
			return makePtr(_base, _member, sizeof(T));
		}
		asmjit::x86::Mem makePtr(const void* _base, const void* _member, size_t _size);
		static void setPtrOffset(asmjit::x86::Mem& _mem, const void* _base, const void* _member);
		
		void readDspMemory(const JitReg& _dst, EMemArea _area, const JitReg& _offset) const;
		void writeDspMemory(EMemArea _area, const JitReg& _offset, const JitReg& _src) const;

		void readDspMemory(const JitReg& _dst, EMemArea _area, TWord _offset) const;
		void writeDspMemory(EMemArea _area, TWord _offset, const JitReg& _src) const;

		void readPeriph(const JitReg64& _dst, EMemArea _area, const TWord& _offset) const;
		void readPeriph(const JitReg64& _dst, EMemArea _area, const JitReg64& _offset) const;
		void writePeriph(EMemArea _area, const JitReg64& _offset, const JitReg64& _value) const;
		void writePeriph(EMemArea _area, const TWord& _offset, const JitReg64& _value) const;

	private:
		void getMemAreaPtr(const JitReg64& _dst, EMemArea _area, TWord offset = 0) const;
		void getMemAreaPtr(const JitReg64& _dst, EMemArea _area, const JitReg& _offset) const;
		JitBlock& m_block;
	};
}
