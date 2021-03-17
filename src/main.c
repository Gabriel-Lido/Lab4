//GABRIEL LIDO AMARAL

#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h"      // device drivers
#include "cmsis_os2.h"       // CMSIS-RTOS

typedef struct {
  
  int periodo;
  uint8_t led;
} struct_led;

/*Tarefa de manipulação de um LED*/
void led_mananger(void *args) {
  
  int state = 0, tickCount;
  struct_led *leds = (struct_led *)args;
  
  while (1){
    
    tickCount = osKernelGetTickCount();
    state ^= leds->led;
    LEDWrite(leds->led, state);
    osDelayUntil(tickCount + leds->periodo);
  }
}

void config_led(struct_led *led, int period, int io){
  led->periodo = period; 
  led->led = io;
}

void main(void) {
  
  LEDInit(LED1 | LED2 | LED3 | LED4);

  struct_led leds[4];
  
  /*LED1 - 200ms*/
  config_led(&leds[0], 200, LED1);

  /*LED2 - 300ms*/
  config_led(&leds[1], 300, LED2);

  /*LED3 - 500ms*/
  config_led(&leds[2], 500, LED3);

  /*LED4 - 700ms*/
  config_led(&leds[3], 700, LED4);
  
  /*Inicializa o escalonador*/
   osKernelInitialize();

  /*Cria 4 instâncias da mesma tarefa*/
  osThreadNew(led_mananger, &leds[0], NULL);
  osThreadNew(led_mananger, &leds[1], NULL);
  osThreadNew(led_mananger, &leds[2], NULL);
  osThreadNew(led_mananger, &leds[3], NULL);

  if (osKernelGetState() == osKernelReady)
    osKernelStart();

  while (1)
    ;
} // main