#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <time.h>
#include "arq.h"

extern int LED_PIN_RED = 16; //OK
extern int LED_PIN_GREEN = 17; //OK
extern int LED_PIN_BLUE = 15;  //OK
extern int LED_PIN_YELLOW = 14; //ok

extern int BUTTON_PIN_RED =    12; //ok
extern int BUTTON_PIN_GREEN =  27; //OK
extern int BUTTON_PIN_BLUE =   5; //OK
extern int BUTTON_PIN_YELLOW =18;  //ok
extern int BUTTTON_PIN_START = 19;

extern int buzzer = 3;

extern int volatile green = 0;
extern int volatile red = 0;
extern int volatile blue = 0;
extern int volatile yellow = 0;
extern int volatile start = 1;

extern int volatile rodada = 0;
extern int volatile sequencia[16];
extern int volatile game_over = 0;

void btn_callback(uint gpio, uint32_t events);
void barulho(int freq, int tempo, int pino);
void tocar_musica_tema(int time_ms);
void inicio();
void buzzer_led(int led_pin);
void proximaRodada();
void reproduzirSequencia();
void aguardarJogador();