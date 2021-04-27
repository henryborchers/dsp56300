#pragma once

#include "dsp.h"
#include "opcodetypes.h"

namespace dsp56k
{
	using TInstructionFunc = void (DSP::*)(TWord op);

	enum PermutationFuncs
	{
		Abs_Field_d_0 = InstructionCount,
		Abs_Field_d_1,
		Asl_D_Field_d_0,
		Asl_D_Field_d_1,

		PermutationFunc_END,
		PermutationFunc_COUNT = PermutationFunc_END - InstructionCount
	};

	struct PermutationValueToFunc
	{
		const TWord value;
		const PermutationFuncs func;
	};

	struct FieldPermutationType
	{
		constexpr explicit FieldPermutationType(const Instruction _instruction, const Field _field, const TWord _value, const PermutationFuncs _func) noexcept
		: instruction(_instruction)
		, field(_field)
		, value(_value)
		, func(_func)
		{
		}

		const Instruction instruction;
		const Field field;
		const TWord value;
		const PermutationFuncs func;
	};

	static const FieldPermutationType g_permutationTypes[] =
	{
		FieldPermutationType(Abs, Field_d, 0, Abs_Field_d_0),
		FieldPermutationType(Abs, Field_d, 1, Abs_Field_d_1),
		FieldPermutationType(Asl_D, Field_d, 0,Asl_D_Field_d_0),
		FieldPermutationType(Asl_D, Field_d, 1,Asl_D_Field_d_1)
	};

	constexpr TInstructionFunc g_jumpTable[] =
	{
		&DSP::op_Abs,							// Abs
		&DSP::op_ADC,							// ADC 
		&DSP::op_Add_SD,						// Add_SD
		&DSP::op_Add_xx,						// Add_xx
		&DSP::op_Add_xxxx,						// Add_xxxx 
		&DSP::op_Addl,							// Addl 
		&DSP::op_Addr,							// Addr 
		&DSP::op_And_SD,						// And_SD 
		&DSP::op_And_xx,						// And_xx 
		&DSP::op_And_xxxx,						// And_xxxx 
		&DSP::op_Andi,							// Andi 
		&DSP::op_Asl_D,							// Asl_D 
		&DSP::op_Asl_ii,						// Asl_ii 
		&DSP::op_Asl_S1S2D,						// Asl_S1S2D 
		&DSP::op_Asr_D,							// Asr_D 
		&DSP::op_Asr_ii,						// Asr_ii 
		&DSP::op_Asr_S1S2D,						// Asr_S1S2D 
		&DSP::op_Bcc_xxxx,						// Bcc_xxxx 
		&DSP::op_Bcc_xxx,						// Bcc_xxx 
		&DSP::op_Bcc_Rn,						// Bcc_Rn 
		&DSP::op_Bchg_ea,						// Bchg_ea 
		&DSP::op_Bchg_aa,						// Bchg_aa 
		&DSP::op_Bchg_pp,						// Bchg_pp 
		&DSP::op_Bchg_qq,						// Bchg_qq 
		&DSP::op_Bchg_D,						// Bchg_D 
		&DSP::op_Bclr_ea,						// Bclr_ea 
		&DSP::op_Bclr_aa,						// Bclr_aa 
		&DSP::op_Bclr_pp,						// Bclr_pp 
		&DSP::op_Bclr_qq,						// Bclr_qq 
		&DSP::op_Bclr_D,						// Bclr_D 
		&DSP::op_Bra_xxxx,						// Bra_xxxx 
		&DSP::op_Bra_xxx,						// Bra_xxx 
		&DSP::op_Bra_Rn,						// Bra_Rn 
		&DSP::op_Brclr_ea,						// Brclr_ea 
		&DSP::op_Brclr_aa,						// Brclr_aa 
		&DSP::op_Brclr_pp,						// Brclr_pp 
		&DSP::op_Brclr_qq,						// Brclr_qq 
		&DSP::op_Brclr_S,						// Brclr_S 
		&DSP::op_BRKcc,							// BRKcc 
		&DSP::op_Brset_ea,						// Brset_ea 
		&DSP::op_Brset_aa,						// Brset_aa 
		&DSP::op_Brset_pp,						// Brset_pp 
		&DSP::op_Brset_qq,						// Brset_qq 
		&DSP::op_Brset_S,						// Brset_S 
		&DSP::op_BScc_xxxx,						// BScc_xxxx 
		&DSP::op_BScc_xxx,						// BScc_xxx 
		&DSP::op_BScc_Rn,						// BScc_Rn 
		&DSP::op_Bsclr_ea,						// Bsclr_ea 
		&DSP::op_Bsclr_aa,						// Bsclr_aa 
		&DSP::op_Bsclr_pp,						// Bsclr_pp 
		&DSP::op_Bsclr_qq,						// Bsclr_qq 
		&DSP::op_Bsclr_S,						// Bsclr_S 
		&DSP::op_Bset_ea,						// Bset_ea 
		&DSP::op_Bset_aa,						// Bset_aa 
		&DSP::op_Bset_pp,						// Bset_pp 
		&DSP::op_Bset_qq,						// Bset_qq 
		&DSP::op_Bset_D,						// Bset_D 
		&DSP::op_Bsr_xxxx,						// Bsr_xxxx 
		&DSP::op_Bsr_xxx,						// Bsr_xxx 
		&DSP::op_Bsr_Rn,						// Bsr_Rn 
		&DSP::op_Bsset_ea,						// Bsset_ea 
		&DSP::op_Bsset_aa,						// Bsset_aa 
		&DSP::op_Bsset_pp,						// Bsset_pp 
		&DSP::op_Bsset_qq,						// Bsset_qq 
		&DSP::op_Bsset_S,						// Bsset_S 
		&DSP::op_Btst_ea,						// Btst_ea 
		&DSP::op_Btst_aa,						// Btst_aa 
		&DSP::op_Btst_pp,						// Btst_pp 
		&DSP::op_Btst_qq,						// Btst_qq 
		&DSP::op_Btst_D,						// Btst_D 
		&DSP::op_Clb,							// Clb 
		&DSP::op_Clr,							// Clr 
		&DSP::op_Cmp_S1S2,						// Cmp_S1S2 
		&DSP::op_Cmp_xxS2,						// Cmp_xxS2 
		&DSP::op_Cmp_xxxxS2,					// Cmp_xxxxS2 
		&DSP::op_Cmpm_S1S2,						// Cmpm_S1S2 
		&DSP::op_Cmpu_S1S2,						// Cmpu_S1S2 
		&DSP::op_Debug,							// Debug 
		&DSP::op_Debugcc,						// Debugcc 
		&DSP::op_Dec,							// Dec 
		&DSP::op_Div,							// Div 
		&DSP::op_Dmac,							// Dmac 
		&DSP::op_Do_ea,							// Do_ea 
		&DSP::op_Do_aa,							// Do_aa 
		&DSP::op_Do_xxx,						// Do_xxx 
		&DSP::op_Do_S,							// Do_S 
		&DSP::op_DoForever,						// DoForever 
		&DSP::op_Dor_ea,						// Dor_ea 
		&DSP::op_Dor_aa,						// Dor_aa 
		&DSP::op_Dor_xxx,						// Dor_xxx 
		&DSP::op_Dor_S,							// Dor_S 
		&DSP::op_DorForever,					// DorForever 
		&DSP::op_Enddo,							// Enddo 
		&DSP::op_Eor_SD,						// Eor_SD 
		&DSP::op_Eor_xx,						// Eor_xx 
		&DSP::op_Eor_xxxx,						// Eor_xxxx 
		&DSP::op_Extract_S1S2,					// Extract_S1S2 
		&DSP::op_Extract_CoS2,					// Extract_CoS2 
		&DSP::op_Extractu_S1S2,					// Extractu_S1S2 
		&DSP::op_Extractu_CoS2,					// Extractu_CoS2 
		&DSP::op_Ifcc,							// Ifcc 
		&DSP::op_Ifcc_U,						// Ifcc_U 
		&DSP::op_Illegal,						// Illegal 
		&DSP::op_Inc,							// Inc 
		&DSP::op_Insert_S1S2,					// Insert_S1S2 
		&DSP::op_Insert_CoS2,					// Insert_CoS2 
		&DSP::op_Jcc_xxx,						// Jcc_xxx 
		&DSP::op_Jcc_ea,						// Jcc_ea 
		&DSP::op_Jclr_ea,						// Jclr_ea 
		&DSP::op_Jclr_aa,						// Jclr_aa 
		&DSP::op_Jclr_pp,						// Jclr_pp 
		&DSP::op_Jclr_qq,						// Jclr_qq 
		&DSP::op_Jclr_S,						// Jclr_S 
		&DSP::op_Jmp_ea,						// Jmp_ea 
		&DSP::op_Jmp_xxx,						// Jmp_xxx 
		&DSP::op_Jscc_xxx,						// Jscc_xxx 
		&DSP::op_Jscc_ea,						// Jscc_ea 
		&DSP::op_Jsclr_ea,						// Jsclr_ea 
		&DSP::op_Jsclr_aa,						// Jsclr_aa 
		&DSP::op_Jsclr_pp,						// Jsclr_pp 
		&DSP::op_Jsclr_qq,						// Jsclr_qq 
		&DSP::op_Jsclr_S,						// Jsclr_S 
		&DSP::op_Jset_ea,						// Jset_ea 
		&DSP::op_Jset_aa,						// Jset_aa 
		&DSP::op_Jset_pp,						// Jset_pp 
		&DSP::op_Jset_qq,						// Jset_qq 
		&DSP::op_Jset_S,						// Jset_S 
		&DSP::op_Jsr_ea,						// Jsr_ea 
		&DSP::op_Jsr_xxx,						// Jsr_xxx 
		&DSP::op_Jsset_ea,						// Jsset_ea 
		&DSP::op_Jsset_aa,						// Jsset_aa 
		&DSP::op_Jsset_pp,						// Jsset_pp 
		&DSP::op_Jsset_qq,						// Jsset_qq 
		&DSP::op_Jsset_S,						// Jsset_S 
		&DSP::op_Lra_Rn,						// Lra_Rn 
		&DSP::op_Lra_xxxx,						// Lra_xxxx 
		&DSP::op_Lsl_D,							// Lsl_D 
		&DSP::op_Lsl_ii,						// Lsl_ii 
		&DSP::op_Lsl_SD,						// Lsl_SD 
		&DSP::op_Lsr_D,							// Lsr_D 
		&DSP::op_Lsr_ii,						// Lsr_ii 
		&DSP::op_Lsr_SD,						// Lsr_SD 
		&DSP::op_Lua_ea,						// Lua_ea 
		&DSP::op_Lua_Rn,						// Lua_Rn 
		&DSP::op_Mac_S1S2,						// Mac_S1S2 
		&DSP::op_Mac_S,							// Mac_S 
		&DSP::op_Maci_xxxx,						// Maci_xxxx 
		&DSP::op_Macsu,							// Macsu 
		&DSP::op_Macr_S1S2,						// Macr_S1S2 
		&DSP::op_Macr_S,						// Macr_S 
		&DSP::op_Macri_xxxx,					// Macri_xxxx 
		&DSP::op_Max,							// Max 
		&DSP::op_Maxm,							// Maxm 
		&DSP::op_Merge,							// Merge 
		&DSP::op_Move_Nop,						// Move_Nop 
		&DSP::op_Move_xx,						// Move_xx 
		&DSP::op_Mover,							// Mover 
		&DSP::op_Move_ea,						// Move_ea 
		&DSP::op_Movex_ea,						// Movex_ea 
		&DSP::op_Movex_aa,						// Movex_aa 
		&DSP::op_Movex_Rnxxxx,					// Movex_Rnxxxx 
		&DSP::op_Movex_Rnxxx,					// Movex_Rnxxx 
		&DSP::op_Movexr_ea,						// Movexr_ea 
		&DSP::op_Movexr_A,						// Movexr_A 
		&DSP::op_Movey_ea,						// Movey_ea 
		&DSP::op_Movey_aa,						// Movey_aa 
		&DSP::op_Movey_Rnxxxx,					// Movey_Rnxxxx 
		&DSP::op_Movey_Rnxxx,					// Movey_Rnxxx 
		&DSP::op_Moveyr_ea,						// Moveyr_ea 
		&DSP::op_Moveyr_A,						// Moveyr_A 
		&DSP::op_Movel_ea,						// Movel_ea 
		&DSP::op_Movel_aa,						// Movel_aa 
		&DSP::op_Movexy,						// Movexy 
		&DSP::op_Movec_ea,						// Movec_ea 
		&DSP::op_Movec_aa,						// Movec_aa 
		&DSP::op_Movec_S1D2,					// Movec_S1D2 
		&DSP::op_Movec_xx,						// Movec_xx 
		&DSP::op_Movem_ea,						// Movem_ea 
		&DSP::op_Movem_aa,						// Movem_aa 
		&DSP::op_Movep_ppea,					// Movep_ppea 
		&DSP::op_Movep_Xqqea,					// Movep_Xqqea 
		&DSP::op_Movep_Yqqea,					// Movep_Yqqea 
		&DSP::op_Movep_eapp,					// Movep_eapp 
		&DSP::op_Movep_eaqq,					// Movep_eaqq 
		&DSP::op_Movep_Spp,						// Movep_Spp 
		&DSP::op_Movep_SXqq,					// Movep_SXqq 
		&DSP::op_Movep_SYqq,					// Movep_SYqq 
		&DSP::op_Mpy_S1S2D,						// Mpy_S1S2D 
		&DSP::op_Mpy_SD,						// Mpy_SD 
		&DSP::op_Mpy_su,						// Mpy_su 
		&DSP::op_Mpyi,							// Mpyi 
		&DSP::op_Mpyr_S1S2D,					// Mpyr_S1S2D 
		&DSP::op_Mpyr_SD,						// Mpyr_SD 
		&DSP::op_Mpyri,							// Mpyri 
		&DSP::op_Neg,							// Neg 
		&DSP::op_Nop,							// Nop 
		&DSP::op_Norm,							// Norm 
		&DSP::op_Normf,							// Normf 
		&DSP::op_Not,							// Not 
		&DSP::op_Or_SD,							// Or_SD 
		&DSP::op_Or_xx,							// Or_xx 
		&DSP::op_Or_xxxx,						// Or_xxxx 
		&DSP::op_Ori,							// Ori 
		&DSP::op_Pflush,						// Pflush 
		&DSP::op_Pflushun,						// Pflushun 
		&DSP::op_Pfree,							// Pfree 
		&DSP::op_Plock,							// Plock 
		&DSP::op_Plockr,						// Plockr 
		&DSP::op_Punlock,						// Punlock 
		&DSP::op_Punlockr,						// Punlockr 
		&DSP::op_Rep_ea,						// Rep_ea 
		&DSP::op_Rep_aa,						// Rep_aa 
		&DSP::op_Rep_xxx,						// Rep_xxx 
		&DSP::op_Rep_S,							// Rep_S 
		&DSP::op_Reset,							// Reset 
		&DSP::op_Rnd,							// Rnd 
		&DSP::op_Rol,							// Rol 
		&DSP::op_Ror,							// Ror 
		&DSP::op_Rti,							// Rti 
		&DSP::op_Rts,							// Rts 
		&DSP::op_Sbc,							// Sbc 
		&DSP::op_Stop,							// Stop 
		&DSP::op_Sub_SD,						// Sub_SD 
		&DSP::op_Sub_xx,						// Sub_xx 
		&DSP::op_Sub_xxxx,						// Sub_xxxx 
		&DSP::op_Subl,							// Subl 
		&DSP::op_subr,							// subr 
		&DSP::op_Tcc_S1D1,						// Tcc_S1D1 
		&DSP::op_Tcc_S1D1S2D2,					// Tcc_S1D1S2D2 
		&DSP::op_Tcc_S2D2,						// Tcc_S2D2 
		&DSP::op_Tfr,							// Tfr 
		&DSP::op_Trap,							// Trap 
		&DSP::op_Trapcc,						// Trapcc 
		&DSP::op_Tst,							// Tst 
		&DSP::op_Vsl,							// Vsl 
		&DSP::op_Wait,							// Wait 
		&DSP::op_ResolveCache,					// ResolveCache
		&DSP::op_Parallel,						// Parallel

		// Permutations
		&DSP::opCE_Abs<0>,
		&DSP::opCE_Abs<1>,
		&DSP::opCE_Asl_D<0>,
		&DSP::opCE_Asl_D<1>,
	};

	static_assert(sizeof(g_jumpTable) / sizeof(g_jumpTable[0]) == (InstructionCount + PermutationFunc_COUNT), "jump table entries missing or too many");
	static_assert(sizeof(g_jumpTable) / sizeof(g_jumpTable[0]) < 256, "jump table too large");
}
