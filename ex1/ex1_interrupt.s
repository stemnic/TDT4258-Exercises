.syntax unified

.include "efm32gg.s"

/*
 * The vector table at address 0 or whatevers defines in SCB->VTOR is what
 * the processor look up into, to find the right interrupt address to run. The
 * negative numbers is defines for fault exceptions and OS suff like SVC and 
 * SysTick. The positive numbers from 0 is user vendor implemented exceptions. 
 * Normally you should add an alias in the assembly routine, so you can just
 * override the functions, but this is also OK...
 */
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

/* Buttons C[0..7] */
/* Leds A[8..15]   */

.globl  _reset
.type   _reset, %function
.thumb_func
_reset:
	/* Enable the GPIO clock */
	ldr r1, =CMU_BASE

	/* Read the content of the clock enable register in the clock management
	 * unit and stores it on r2 */
	ldr r2, [r1, #CMU_HFPERCLKEN0]

	/* Moves the number 1 into r3 */
	mov r3, #1

	/* Bitshifts the number 1 the number of position defined in CMU_HFPERCLKEN0_GPIO */
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO

	/* Perform bitwise or between the r3 and r2. This gives the final bitmask */
	orr r3, r3, r2

	/* Store the bitmask back again.  */
	str r3, [r1, #CMU_HFPERCLKEN0]

	/*
	 * The above part is much simple if the registers implements a set-register
	 * and a clear register.
	 */

	/* Set driver stregth */
	ldr r1, =GPIO_PA_BASE

	/* Move the number 2 into r2 */
	mov r2, #2

	/* Stores the number 2 in the address of GPIO_PA_BASE aka the first register */
	str r2, [r1]

	/* LED initialization */
	
	/* R3 wil hold the number of times to loop => 8 */
	mov r3, #8
	mov r2, #0

	/* Label - when writing b label this functions is branched to */
_led_mode_loop:
	/* Sets the mode value to 4 and bitshift r0 times. This starts at zero
	 * thus bitshifting zero times and increment with 4 each time */
	lsl r2, r2, #4

	/* Move in 4 to the lowest 4 bits in r2 */
	orr r2, #4

	/* Subtract the loop counter, the s suffix means that the CPSR register will
	 * be updated as well. This means the status flags will be updated. A normal
	 * sub + a cmd will do the same thing */
	subs r3, r3, #1

	/* Branches to the lable if the result of the status operation subs was not
	 * zero */
	bne _led_mode_loop

	/* The loop has iterated 8 times and the r2 hold the entire bitmask for 
	 * all the leds */

	 /* Load the GPIO_PA_BASE address into r1 */
	ldr r1, =GPIO_PA_BASE

	/* Store the final bitmask into the GPIO_PA_BASE + GPIO_MODEH */
	str r2, [r1, #GPIO_MODEH]

	/* Turn all pins off */
	ldr r2, =0xffff

	/* Store a 1 on the lowest 16 bits intro the DOUT register. Thus turing 
	 * off all leds */
	str r2, [r1, #GPIO_DOUTSET]

	/* Button initialization */
	mov r3, #8
	mov r2, #0

_btn_mode_loop:
    /* Bitshift the gpio mask with 4 each time. This first time does not matter
	 * since the value is zero. */
	lsl r2, r2, #4

	/* Stor the button operation 2 into the right 4 bits describing the button
	 * in the MODEL */
	orr r2, #2

	/* Subtract the loop counter, look above for the subs */
	subs r3, r3, #1

	/* If not done 8 times branch to the label */
	bne _btn_mode_loop

	/* Moves the GPIO_PC_BASE into the r1 register */
	ldr r1, =GPIO_PC_BASE

	/* Stores the entire bitmask into the MODEL register in PC */
	str r2, [r1, #GPIO_MODEL]

	/* Set the pull-mode to pull-up */

	/* In input mode the pullup and pulldown are controlled with the out register
	 * usally this is controlled with a separate register */
	ldr r2, =0xffff

	/* Enable pullup on all lines */
	str r2, [r1, #GPIO_DOUTSET]

	/* Enable button interrupt */
	/* Just load the value directly */
	ldr r4, =0x22222222

	/* Load the GPIO_BASE address into r5 */
	ldr r5, =GPIO_BASE

	/* Store the number containing the interrupt configuration into EXTIPSELL */
	str r4, [r5, #GPIO_EXTIPSELL]
	
	/* Enable rising and falling edge detection */
	ldr r4, =0xff

	/* Store 8 ones in both the falling andr rising edge register, the interrupt clear and
	 * the interrupt enable register */
	str r4, [r5, #GPIO_EXTIFALL]
	str r4, [r5, #GPIO_EXTIRISE]
	str r4, [r5, #GPIO_IFC]
	str r4, [r5, #GPIO_IEN]
	
	/* Enable NVIC */
	/* Place the interrupt number to enable on R0 and branch with link (AAPCS) */
	mov r0, #1
	bl _enable_interrupt

	/* Place the interrupt number to enable on R0 and branch with link (AAPCS) */
	mov r0, #11
	bl _enable_interrupt

	cpsie f
    
	/* Deepsleep after returning from interrupt handler */
	/* If oscillators are enabled they should minimize current consumption. Typically
	 * the internal RC oscillator is enabled. But the internal SCLK 32 KHz uses
	 * less power. There is more clocks in a MCU than the processor clock*/

	/* Writes a 1 to the following bits SEVONPEND in order or make up from WFE
	 * SLEEPDEEP to signal that the PCLK can be stopped and SLEEPONEXIT to
	 * sleep in ISR exit */
	mov r7, #0b10110
	ldr r8, =SCR
	str r7, [r8]
	wfi
	
/* For testing button without interrupt */
_button_loop:
	/*
	 * The button loop reads the content of the data in register for the GPIO
	 * port C. This register is manipulated to contain all the button bits in 
	 * the LSByte (lease significant byte) and stored to the data out register
	 * on the GPIO port A. This is where the LED sits
	 */
	/* Places the GPIO_PC_BASE address inside r1 */
	ldr r1, =GPIO_PC_BASE

	/* Load r2 with the register at address GPIO_PC_BASE + GPIO_DIN */
	ldr r2, [r1, #GPIO_DIN]

	/*
	 * Performs a logical shift left on the content on R2 aka the value at 
	 * address GPIO_PC_BASE + GPIO_DIN. That means that the content is shfted
	 * 8 places to the left
	 */
	lsl r2, r2, #8

	/* Places the GPIO_PA_BASE address inside r1 */
	ldr r1, =GPIO_PA_BASE

	/* Stores the content of R2 to the address of GPIO_PA_BASE + GPIO_DOUT */
	str r2, [r1, #GPIO_DOUT]

	/*
	 * Relative unditional branch. Note that the range varies depending on 
	 * running in Thumb or ARM mode. If the processor runs in Thumb but supports
	 * Thumb2 code the b.w can extend the branch range from 2K to 32M
	 * 
	 * This statement branches to the _button_loop
	 */
	b _button_loop

/* Takes in NVIC interrupt number in R0 */
_enable_interrupt:
	/*
	 * This is a function for enabling interrupt thought the Cortex NVIC 
	 * peripheral on the processor. According to the AAPCS or ARM architecture
	 * procedure calling standard the registers used for functions calls are 
	 * normally the R0-R3. This means that in order to call a function the
	 * arguments is placed in these registers (and on stack). This is much more
	 * interesting when looking at AArch64. 
	 */

	/*
	 * Push is a alias for stmdb store-multiple-decrement before or sdmfd store
	 * multiple fully descending. These uses the stack pointer as an argument
	 * stmdb sp! and and optional ! in order to update the sp after the decrement.
	 * This comes from the nature of the Cortex fully descending stack. It does
	 * not matter here, but on more complex processors you have multiple stack 
	 * pointers and then this is useful. 
	 */
	push {r1-r3}

	/* Load the address of NVIC_ISER into r2 */
	ldr r2, =NVIC_ISER

	/* Move the number 32 into r1 */
	mov r1, #32

	udiv r3, r0, r1  /* Unsigned divide, r3 = r0/r1 */

	/* Takes the content of R3 and multiply by 4 */
	lsl r3, r3, #2

	/* Performs a and operation between #31 which is 32 - 1 and r0 */
	and r0, r0, #31 /* r0 modulo 32 */

	/* Moves the number 1 into r1 */
	mov r1, #1

	/* Takes the content of r1 aka 1 and bitshift it r0 places to the left */
	lsl r1, r1, r0

	/* Store the manipulated bitmask back into its register NVIC_ISR + register offset */
	str r1, [r2, r3]

	/* Pop the used registers from stack, ldmia sp! or ldmfd sp! can also be used */
	pop {r1-r3}

	/*
	 * Since this funcitons is called by a bl label the processor executes a
	 * branch with link. This means that the PC is places into the LR and then
	 * the branch is performed. This is right but not intuitive. The cortex
	 * processor PC acctually points 4 bytes ahead in ARM mode and 2 bytes ahead
	 * in Thumb mode. Therefore by storing the lr the subroutine will jump too 
	 * the NEXT intstruciton. This is the case for branches but NOT for interrupts.
	 * On bigger processors (and the NVIC) must therefore adjust the LR 
	 * (acctually the stack pointer) by -4 in ARM mode and -2 in Thumb mode in
	 * order to be able to return the right place. In case of a fault exception
	 * this adjustment is 8 bytes in ARM mode because of the pipeline
	 */
	bx lr

/* Turn on LED based on index in R0 */
_led_on:
	/* Look here for the push operation _enable_interrupt */
	push {r1-r2}

	/* Moved number 1 to register r1 */
	mov r1, #1

	/* Adds 8 to the value in R0, aka whatever you called the function with */
	add r0, r0, #8

	/* Moved the bit in r1 the number of places indicated by the pin number
	 * pluss 8, this has to do with the port name */
	lsl r1, r1, r0

	/* Loads register r2 with the value of GPIO_PA_BASE aka the base address 
	 * for port A */
	ldr r2, =GPIO_PA_BASE

	/* Writes the previous generated bitmask to the OUTCLR register. This 
	 * operation only cares for the bits which is one */
	str r1, [r2, #GPIO_DOUTCLR]

	/* Pop the used registers from stack, ldmia sp! or ldmfd sp! can also be used */
	pop {r1-r2}

	/* Look here for the bx lr operation _enable_interrupt */
	bx lr

/* Turn off LED based on index in R0 */
_led_off:
	/* Look here for the push operation _enable_interrupt */
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
 * of R0-R3, R12, LR, PC, PSR. The Cortex-M has this feature included unlike
 * other processors where this must be done manually. Then, in order to use
 * nesting like this, the LR_irq and CPSR_irq must also be push to the correct
 * stack to avoid register corruption
 */
.thumb_func
gpio_handler:
	/*
	 * LDR {type}{cond} Rt, [Rn, #offset] load with immediate offset. This can 
	 * be used with either pre-indexed or post-indexed addressing. For eksempel
	 * to load the address BARE = 56 into register R0, one can use
	 * ldr r0, =GPIO_BASE. Note however that different assemblers use different
	 * syntax here, and the big difference between .s and .S files.
	 */
	ldr r0, =GPIO_BASE
	/* Load r1 with the register at address GPIO_BASE + GPIO_IF */
	ldr r1, [r0, #GPIO_IF]

	/* Stores the value of r1 aka the interrupt status register into the 
	 * interrupt clear register in order to clear the flag */
	str r1, [r0, #GPIO_IFC]

	/* Reads the gpio button and turns the respective button on */
	ldr r1, =GPIO_PC_BASE
	ldr r2, [r1, #GPIO_DIN] /* Read the DIN values [0:7] */
	lsl r2, r2, #8 /* Button data in port -> moved to [8:15] */

	/* Loads the base address of the GPIO_PA_BASE into r2 */
	ldr r1, =GPIO_PA_BASE

	/* Inverts the second operand and stored the result in r1 */
	//mvn r1, r1

	/* Stores the inverted value in the DOUT register */
	str r2, [r1, #GPIO_DOUT]

	/* Branch to the LR which was stacked in the stack frame by the processor
	 * upon entering the interrupt routine */
	bx lr
	
/* The cortex-M does not have a GIC or APIC interrupt controller to enable 
 * spesific interrupt and add handlers. Instead all the functions addresses
 * are defined in the vector table. This dummy handler is used when an interrupt
 * is enabled but the interrupt vector is not overridden. Check out WEAK attribute
 * in C or alias in assembly */
.thumb_func
dummy_handler:
	b .

