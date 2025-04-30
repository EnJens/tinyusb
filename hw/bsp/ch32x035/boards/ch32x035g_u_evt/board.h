/* metadata:
   name: nanoCH32V203
   url: https://github.com/wuxx/nanoCH32V203
*/

#ifndef BOARD_H_
#define BOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

// The LEDs are not connected to anything on the board,
// so users need to hook up LED1 to PB0 to get output
#define LED_PORT       GPIOB
#define LED_PIN        GPIO_Pin_0
#define LED_STATE_ON   0
#define LED_CLOCK_EN() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)
#define LED_MODE       GPIO_Mode_Out_PP

#define UART_DEV        USART1
#define UART_CLOCK_EN() RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE)
#define UART_TX_PIN     GPIO_Pin_10
#define UART_RX_PIN     GPIO_Pin_9

#ifdef __cplusplus
}
#endif

#endif
