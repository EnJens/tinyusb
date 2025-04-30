/* metadata:
manufacturer: WCH
*/

#include <stdio.h>

// https://github.com/openwch/ch32v307/pull/90
// https://github.com/openwch/ch32v20x/pull/12
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#endif

#include "ch32x035.h"

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include "bsp/board_api.h"
#include "board.h"

__attribute__((interrupt)) __attribute__((used))
void USBFS_IRQHandler(void) {
  #if CFG_TUD_WCH_USBIP_USBFS
  tud_int_handler(0);
  #endif
}


__attribute__((interrupt)) __attribute__((used))
void USBFSWakeUp_IRQHandler(void) {
  #if CFG_TUD_WCH_USBIP_USBFS
  tud_int_handler(0);
  #endif
}


#if CFG_TUSB_OS == OPT_OS_NONE
volatile uint32_t system_ticks = 0;

__attribute__((interrupt))
void SysTick_Handler(void) {
  SysTick->SR = 0;
  system_ticks++;
}

uint32_t SysTick_Config(uint32_t ticks) {
  NVIC_EnableIRQ(SysTicK_IRQn);
  SysTick->CTLR = 0;
  SysTick->SR = 0;
  SysTick->CNT = 0;
  SysTick->CMP = ticks - 1;
  SysTick->CTLR = 0xF;
  return 0;
}


#define USB_IOEN                    0x00000080
#define USB_PHY_V33                 0x00000040
#define UDP_PUE_MASK                0x0000000C
#define UDP_PUE_DISABLE             0x00000000
#define UDP_PUE_35UA                0x00000004
#define UDP_PUE_10K                 0x00000008
#define UDP_PUE_1K5                 0x0000000C

#define UDM_PUE_MASK                0x00000003
#define UDM_PUE_DISABLE             0x00000000
#define UDM_PUE_35UA                0x00000001
#define UDM_PUE_10K                 0x00000002
#define UDM_PUE_1K5                 0x00000003

uint32_t board_millis(void) {
  return system_ticks;
}
#endif

void board_init(void) {
  __disable_irq();

#if CFG_TUSB_OS == OPT_OS_NONE
  SysTick_Config(SystemCoreClock / 1000);
#endif

  LED_CLOCK_EN();

  GPIO_InitTypeDef GPIO_InitStructure = {
    .GPIO_Pin = LED_PIN,
    .GPIO_Mode = LED_MODE,
    .GPIO_Speed = GPIO_Speed_50MHz,
  };
  GPIO_Init(LED_PORT, &GPIO_InitStructure);

#ifdef UART_DEV
  UART_CLOCK_EN();
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitTypeDef usart_init = {
    .GPIO_Pin = UART_TX_PIN,
    .GPIO_Speed = GPIO_Speed_50MHz,
    .GPIO_Mode = GPIO_Mode_AF_PP,
  };
  GPIO_Init(GPIOB, &usart_init);

  USART_InitTypeDef usart = {
    .USART_BaudRate = 115200,
    .USART_WordLength = USART_WordLength_8b,
    .USART_StopBits = USART_StopBits_1,
    .USART_Parity = USART_Parity_No,
    .USART_Mode = USART_Mode_Tx,
    .USART_HardwareFlowControl = USART_HardwareFlowControl_None,
  };
  USART_Init(UART_DEV, &usart);
  USART_Cmd(UART_DEV, ENABLE);
#endif

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_USBFS, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_16;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_17;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

/*  if( PWR_VDD_SupplyVoltage() == PWR_VDD_5V )
  {
      AFIO->CTLR = (AFIO->CTLR & ~(UDP_PUE_MASK | UDM_PUE_MASK | USB_PHY_V33)) | UDP_PUE_10K | USB_IOEN;
  }
  else
  {*/
      AFIO->CTLR = (AFIO->CTLR & ~(UDP_PUE_MASK | UDM_PUE_MASK )) | USB_PHY_V33 | UDP_PUE_1K5 | USB_IOEN;
  //}

  __enable_irq();
}

void board_reset_to_bootloader(void) {
//   board_led_write(true);
//
//   __disable_irq();
//
// #if CFG_TUD_ENABLED
//   tud_deinit(0);
//   RCC_APB1PeriphResetCmd(RCC_APB1Periph_USB, ENABLE);
//   RCC_APB1PeriphResetCmd(RCC_APB1Periph_USB, DISABLE);
// #endif
//
//   SysTick->CTLR = 0;
//   for (int i = WWDG_IRQn; i< DMA1_Channel8_IRQn; i++) {
//     NVIC_DisableIRQ(i);
//   }
//
//   __enable_irq();
//
//   // define function pointer to BOOT ROM address
//   void (*bootloader_entry)(void) = (void (*)(void))0x1FFF8000;
//
//   bootloader_entry();
//
//   board_led_write(false);

  // while(1) { }
}

void board_led_write(bool state) {
  GPIO_WriteBit(LED_PORT, LED_PIN, state ? LED_STATE_ON : (1-LED_STATE_ON));
}

uint32_t board_button_read(void) {
  return false;
}

size_t board_get_unique_id(uint8_t id[], size_t max_len) {
  (void) max_len;
  volatile uint32_t* ch32_uuid = ((volatile uint32_t*) 0x1FFFF7E8UL);
  uint32_t* serial_32 = (uint32_t*) (uintptr_t) id;
  serial_32[0] = ch32_uuid[0];
  serial_32[1] = ch32_uuid[1];
  serial_32[2] = ch32_uuid[2];

  return 12;
}

int board_uart_read(uint8_t *buf, int len) {
  (void) buf;
  (void) len;
  return 0;
}

int board_uart_write(void const *buf, int len) {
#ifdef UART_DEV
  const char *bufc = (const char *) buf;
  for (int i = 0; i < len; i++) {
    while (USART_GetFlagStatus(UART_DEV, USART_FLAG_TC) == RESET);
    USART_SendData(UART_DEV, *bufc++);
  }
#else
  (void) buf; (void) len;
#endif

  return len;
}

//--------------------------------------------------------------------
// Neopixel
//--------------------------------------------------------------------
