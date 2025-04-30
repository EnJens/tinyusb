/* metadata:
   name: nanoCH32V203
   url: https://github.com/wuxx/nanoCH32V203
*/

#ifndef BOARD_H_
#define BOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LED_PORT       GPIOB
#define LED_PIN        GPIO_Pin_0
#define LED_STATE_ON   0
#define LED_CLOCK_EN() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)
#define LED_MODE       GPIO_Mode_Out_PP

#define UART_DEV        USART1
#define UART_CLOCK_EN() RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE)
#define UART_TX_PIN     GPIO_Pin_9
#define UART_RX_PIN     GPIO_Pin_10

#ifdef __cplusplus
}
#endif

#endif
