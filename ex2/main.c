#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "nvic.h"

/*
 * TODO calculate the appropriate sample period for the sound wave(s) you 
 * want to generate. The core clock (which the timer clock is derived
 * from) runs at 14 MHz by default. Also remember that the timer counter
 * registers are 16 bits. 
 */
/*
 * The period between sound samples, in clock cycles 
 */
#define   SAMPLE_PERIOD   0

/*
 * Declaration of peripheral setup functions 
 */
void setup_timer(uint32_t period);
void setup_dac();
void setup_nvic();

/*
 * Your code will start executing here 
 */
int main(void)
{
	setup_gpio();
	setup_dac();
	setup_timer(SAMPLE_PERIOD);

	setup_nvic();

	/*
	 * TODO for higher energy efficiency, sleep while waiting for
	 * interrupts instead of infinite loop for busy-waiting 
	 */
	while (1) ;

	return 0;
}

#define IRQ_TIMER1 54
#define IRQ_GPIO_EVEN 5
#define IRQ_GPIO_ODD 4

void setup_nvic()
{
	nvic_enable(IRQ_TIMER1);
	nvic_set_priority(IRQ_TIMER1, NVIC_PRI_2);

	nvic_enable(IRQ_GPIO_EVEN);
	nvic_set_priority(IRQ_GPIO_EVEN, NVIC_PRI_2);

	nvic_enable(IRQ_GPIO_ODD);
	nvic_set_priority(IRQ_GPIO_ODD, NVIC_PRI_2);
}

/*
 * if other interrupt handlers are needed, use the following names:
 * NMI_Handler HardFault_Handler MemManage_Handler BusFault_Handler
 * UsageFault_Handler Reserved7_Handler Reserved8_Handler
 * Reserved9_Handler Reserved10_Handler SVC_Handler DebugMon_Handler
 * Reserved13_Handler PendSV_Handler SysTick_Handler DMA_IRQHandler
 * GPIO_EVEN_IRQHandler TIMER0_IRQHandler USART0_RX_IRQHandler
 * USART0_TX_IRQHandler USB_IRQHandler ACMP0_IRQHandler ADC0_IRQHandler
 * DAC0_IRQHandler I2C0_IRQHandler I2C1_IRQHandler GPIO_ODD_IRQHandler
 * TIMER1_IRQHandler TIMER2_IRQHandler TIMER3_IRQHandler
 * USART1_RX_IRQHandler USART1_TX_IRQHandler LESENSE_IRQHandler
 * USART2_RX_IRQHandler USART2_TX_IRQHandler UART0_RX_IRQHandler
 * UART0_TX_IRQHandler UART1_RX_IRQHandler UART1_TX_IRQHandler
 * LEUART0_IRQHandler LEUART1_IRQHandler LETIMER0_IRQHandler
 * PCNT0_IRQHandler PCNT1_IRQHandler PCNT2_IRQHandler RTC_IRQHandler
 * BURTC_IRQHandler CMU_IRQHandler VCMP_IRQHandler LCD_IRQHandler
 * MSC_IRQHandler AES_IRQHandler EBI_IRQHandler EMU_IRQHandler 
 */
