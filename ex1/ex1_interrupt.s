.syntax unified

.include "efm32gg.s"

/////////////////////////////////////////////////////////////////////////////
//
// Exception vector table
// This table contains addresses for all exception handlers
//
/////////////////////////////////////////////////////////////////////////////

.section .vectors
	.long   stack_top               /* Top of Stack                 */
	.long   _reset                  /* Reset Handler                */
	.long   dummy_handler           /* NMI Handler                  */
	.long   dummy_handler           /* Hard Fault Handler           */
	.long   dummy_handler           /* MPU Fault Handler            */
	.long   dummy_handler           /* Bus Fault Handler            */
	.long   dummy_handler           /* Usage Fault Handler          */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* SVCall Handler               */
	.long   dummy_handler           /* Debug Monitor Handler        */
	.long   dummy_handler           /* Reserved                     */
	.long   dummy_handler           /* PendSV Handler               */
	.long   dummy_handler           /* SysTick Handler              */

	/* External Interrupts */
	.long   dummy_handler
	.long   gpio_handler            /* GPIO even handler */
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   gpio_handler            /* GPIO odd handler */
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler
	.long   dummy_handler

.section .text

	/////////////////////////////////////////////////////////////////////////////
	//
	// Reset handler
	// The CPU will start executing here after a reset
	//
	/////////////////////////////////////////////////////////////////////////////


/* Buttons C[0..7] */
/* Leds A[8..15]   */

.globl  _reset
.type   _reset, %function
.thumb_func
_reset:
	/* Enable the GPIO clock */
	ldr r1, =CMU_BASE
	ldr r2, [r1, #CMU_HFPERCLKEN0]
	mov r3, #1
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
	orr r3, r3, r2
	str r3, [r1, #CMU_HFPERCLKEN0]

	/* Set driver stregth */
	ldr r1, =GPIO_PA_BASE
	mov r2, #2
	str r2, [r1]

	/* LED initialization */
	mov r3, #8
	mov r2, #0

_led_mode_loop:
	lsl r2, r2, #4
	orr r2, #4
	subs r3, r3, #1
	bne _led_mode_loop
	ldr r1, =GPIO_PA_BASE
	str r2, [r1, #GPIO_MODEH]

	/* Turn all pins off */
	ldr r2, =0xffff
	str r2, [r1, #GPIO_DOUTSET]

	/* Button initialization */
	mov r3, #8
	mov r2, #0

_btn_mode_loop:
	lsl r2, r2, #4
	orr r2, #2
	subs r3, r3, #1
	bne _btn_mode_loop
	ldr r1, =GPIO_PC_BASE
	str r2, [r1, #GPIO_MODEL]

	/* Set the pull-mode to pull-up */
	ldr r2, =0xffff
	str r2, [r1, #GPIO_DOUTSET]

	/* Enable button interrupt */
	
	ldr r4, =0x22222222
	ldr r5, =GPIO_BASE
	str r4, [r5, #GPIO_EXTIPSELL]
	
	/* Enable rising and falling edge detection */
	 
	ldr r4, =0xff
	str r4, [r5, #GPIO_EXTIFALL]
	str r4, [r5, #GPIO_IFC]
	str r4, [r5, #GPIO_IEN]
	
	/* Enable NVIC */
	
	mov r0, #1
	bl _enable_interrupt

	mov r0, #11
	bl _enable_interrupt

	cpsie f
    
	/* Deepsleep after returning from interrupt handler */
	// SEVONPEND = 1
	// SLEEPDEEP = 1
	// SLEEPONEXIT = 1
    mov r7, #0b10110
    ldr r8, =SCR
    str r7, [r8]
    wfi
	
/* For testing button without interrupt */
_button_loop:
	ldr r1, =GPIO_PC_BASE
	ldr r2, [r1, #GPIO_DIN]
	lsl r2, r2, #8
	ldr r1, =GPIO_PA_BASE
	str r2, [r1, #GPIO_DOUT]
	b _button_loop

/* Takes in NVIC interrupt number in R0 */
_enable_interrupt:
	push {r1-r3}
	ldr r2, =NVIC_ISER
	mov r1, #32
	udiv r3, r0, r1  /* Unsigned divide, r3 = r0/r1 */
	lsl r3, r3, #2
	and r0, r0, #31 /* r0 modulo 32 */
	mov r1, #1
	lsl r1, r1, r0
	str r1, [r2, r3]
	pop {r1-r3}
	bx lr

/* Turn on LED based on index in R0 */
_led_on:
	push {r1-r2}
	mov r1, #1
	add r0, r0, #8
	lsl r1, r1, r0
	ldr r2, =GPIO_PA_BASE
	str r1, [r2, #GPIO_DOUTCLR]
	pop {r1-r2}
	bx lr

/* Turn off LED based on index in R0 */
_led_off:
	push {r1-r2}
	mov r1, #1
	add r0, r0, #8
	lsl r1, r1, r0
	ldr r2, =GPIO_PA_BASE
	str r1, [r2, #GPIO_DOUTSET]
	pop {r1-r2}
	bx lr

/*
 * Standard interrupt handling. CPU saves the stack frame consisting
 * of R0-R3, R12, LR, PC, PSR.
 */
.thumb_func
gpio_handler:
	ldr r0, =GPIO_BASE
	ldr r1, [r0, #GPIO_IF]
	str r1, [r0, #GPIO_IFC]
	lsl r1, r1, #8
	ldr r2, =GPIO_PA_BASE
	mvn r1, r1
	str r1, [r2, #GPIO_DOUT]
	bx lr
	
.thumb_func
dummy_handler: 
	b .

