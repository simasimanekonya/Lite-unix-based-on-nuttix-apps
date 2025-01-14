/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include <pico/stdlib.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * hello_main
 ****************************************************************************/

#if defined(BUILD_MODULE)
int main(int argc, FAR char *argv[])
#else
int helloworld_main(int argc, char *argv[])
#endif
{
  // Raspberry Pi PicoのGPIO 25をLEDとして設定
  const uint LED_PIN = 25; // GPIO 25はRaspberry Pi Picoの内蔵LED

  // Pico SDKの初期化
  stdio_init_all();

  // GPIOを出力モードに設定
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  while (1)
    {
      // LEDをONにする
      gpio_put(LED_PIN, 1);
      printf("LED ON\n");

      // LEDをOFFにする
      gpio_put(LED_PIN, 0);
      printf("LED OFF\n");

      // 500ms間隔で点滅
      sleep_ms(500);
    }

  return 0;
}
