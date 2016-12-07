@ startup
	.global _start
	.align
_start:
	ldr pc,reset_handler
	ldr pc,undefined_handler
	ldr pc,swi_handler
	ldr pc,prefetch_handler
	ldr pc,data_handler
	ldr pc,hyp_handler
	ldr pc,irq_handler
	ldr pc,fiq_handler
reset_handler:      .word _reset
undefined_handler:  .word _hang
swi_handler:        .word _hang
prefetch_handler:   .word _hang
data_handler:       .word _hang
hyp_handler:        .word _hang
irq_handler:        .word _IRQ_iterrupt
fiq_handler:        .word _hang

_reset:
	// SVCモードのスタックポインタ設定
	// IRQ,FIQ disable, Thumb disable, SVC mode
	;@mov	r0, #0x000000d3
	;@msr	cpsr_c, r0
	// 割り込みベクタを0x8000にリマップ
	mov r0,#0x8000
	MCR p15, 4, r0, c12, c0, 0
	ldr	sp, =0x06400000

	// IRQモードのスタックポインタ設定
	// IRQ,FIQ disable, Thumb disable, IRQ mode
	@mov	r0, #0x000000f2
	@msr	cpsr_c, r0
	@ldr	sp, =0x00008000
	@eret


	// main呼び出し
	bl main

	// 終了（無限ループ）
	b .

.global _hang
_hang:
	// IRQ,FIQ disable, Thumb disable, SVC mode
	@ldr	r0, =0x000000d3
	@msr	cpsr, r0
	b .


_IRQ_iterrupt:
	// スタックにレジスタの内容を退避させる
	stmfd	r13!, {r0-r12,lr}
	// C言語の割り込みハンドラ呼び出し
	bl	interrupt_IRQ_handler
	// スタックからレジスタの内容を復元する。
	ldmfd	r13!, {r0-r12,lr}
	// 戻りアドレス修正（IRQはlr-4）して、元の実行番地に復帰
	;@subs	pc,lr, #4
	eret


// IRQ有効化（CPSR設定）
	.global enable_IRQ
enable_IRQ:
	mrs	r0, cpsr
	bic r0, r0, #0x80
	msr cpsr_c,r0
	bx lr

// IRQ無効化（CPSR設定）
	.global disable_IRQ
disable_IRQ:
	mrs	r0, cpsr
	ldr	r1,	=0x80
	orr r0, r0, r1
	msr	cpsr_c, r0
	bx lr

// CPSRの値を取得して返す（r0は戻り値を入れるレジスタ）
	.global getmode
getmode:
	mrs	r0, cpsr
	bx lr


// ベクターテーブル設定用
@	.global  _initial_vector_start
@_initial_vector_start:
@	ldr	pc, _vec_Reset
@	ldr	pc, _vec_Undef
@	ldr	pc, _vec_SWI
@	ldr	pc, _vec_PrefAbort
@	ldr	pc, _vec_DataAbort
@	ldr	pc, _vec_Hyp
@	ldr	pc, _vec_IRQ
@	ldr	pc, _vec_FIQ
@_vec_Reset:		.word	_start
@_vec_Undef:		.word	_hangup
@_vec_SWI:		.word	_hangup
@_vec_PrefAbort:	.word	_hangup
@_vec_DataAbort:	.word	_hangup
@_vec_Hyp:	.word	_hangup
@_vec_IRQ:		.word	_IRQ_iterrupt
@_vec_FIQ:		.word	_hangup
@	.global  _initial_vector_end
@_initial_vector_end:
@	nop
// 最後に何か命令を置かないと_initial_vector_endラベルが消える
