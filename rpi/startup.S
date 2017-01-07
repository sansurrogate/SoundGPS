@ startup
	.global _start
	.align
_start:
	.global _exception_vector_start
_exception_vector_start:
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
	.global _exception_vector_end
_exception_vector_end:

_reset:
.ifdef RPI2
	// 割り込みベクタを0x8000にリマップ
	mov r0,#0x8000
	MCR p15, 4, r0, c12, c0, 0
	ldr sp, =0x06400000

.else
	// IRQモードのスタックポインタ設定
	// IRQ,FIQ disable, Thumb disable, IRQ mode
	mov r0, #0x000000d2
	msr cpsr_c, r0
	ldr sp, =0x00008000

	// SVCモードのスタックポインタ設定
	// IRQ,FIQ disable, Thumb disable, SVC mode
	mov r0, #0x000000d3
	msr cpsr_c, r0
	ldr sp, =0x06400000

	// 例外ベクタをアドレス0x00に配置する
	bl interrupt_init_exception_vector
.endif

	// main呼び出し
	bl main

	// 終了（無限ループ）
	b .

.global _hang
_hang:
	// IRQ,FIQ disable, Thumb disable, SVC mode
	@ldr r0, =0x000000d3
	@msr cpsr, r0
	b .


_IRQ_iterrupt:
	// スタックにレジスタの内容を退避させる
	stmfd r13!, {r0-r12,lr}
	// C言語の割り込みハンドラ呼び出し
	bl interrupt_IRQ_handler
	// スタックからレジスタの内容を復元する。
	ldmfd r13!, {r0-r12,lr}
	// 戻りアドレス修正（IRQはlr-4）して、元の実行番地に復帰
.ifdef RPI2
	eret
.else
	subs pc, lr, #4
.endif


// IRQ有効化（CPSR設定）
	.global enable_IRQ
enable_IRQ:
	mrs r0, cpsr
	bic r0, r0, #0x80
	msr cpsr_c,r0
	bx lr

// IRQ無効化（CPSR設定）
	.global disable_IRQ
disable_IRQ:
	mrs r0, cpsr
	ldr r1, =0x80
	orr r0, r0, r1
	msr cpsr_c, r0
	bx lr

// CPSRの値を取得して返す（r0は戻り値を入れるレジスタ）
	.global getmode
getmode:
	mrs r0, cpsr
	bx lr
