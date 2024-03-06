/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <time.h>

//****************** VARIAVEIS GLOBAIS ******************

int LED_PIN_RED = 16; //OK
int LED_PIN_GREEN = 17; //OK
int LED_PIN_BLUE = 15;  //OK
int LED_PIN_YELLOW = 14; //ok
int leds[4] = {16, 17, 15, 14}; // vermelho(dó), verde(ré), azul(mi), amarelo(fá)

int BUTTON_PIN_RED =    12; //ok
int BUTTON_PIN_GREEN =  27; //OK
int BUTTON_PIN_BLUE =   5; //OK
int BUTTON_PIN_YELLOW =18;  //ok
int botoes[4] = {12, 27, 5, 18};

int buzzer = 3;
int tons[4] = {262, 294, 330, 349}; // dó, ré, mi, fá

int volatile green = 0;
int volatile red = 0;
int volatile blue = 0;
int volatile yellow = 0;
int volatile btn = 0;

int pontuacao = 0;
int sequencia[100];
int sequencia_jogador[100];
int rodada = 0;
int passo = 0;
int botao_pressionado = 0;
bool game_over = false;

//******************* CALLBACKS ******************* 
void btn_callback(uint gpio, uint32_t events) {
    btn = 1;
    if (gpio == BUTTON_PIN_RED) {
        red = 1;
    }
    if (gpio == BUTTON_PIN_GREEN) {
        green = 1;
    }
    if (gpio == BUTTON_PIN_BLUE) {
        blue = 1;
    }
    if (gpio == BUTTON_PIN_YELLOW) {
        yellow = 1;
    }
}

//************* FUNCOES ****************
void barulho(int freq, int tempo, int pino){
    int periodo = 1000000/freq;
    for(int i = 0; i < tempo*1000/periodo; i++){
        gpio_put(pino, 1);
        sleep_us(periodo/2);
        gpio_put(pino, 0);
        sleep_us(periodo/2);
    }
}

void buzzer_led(int led_pin) {
    int freq;
    if (led_pin == LED_PIN_RED) {
        freq = 400;
    } else if (led_pin == LED_PIN_GREEN) {
        freq = 500;
    } else if (led_pin == LED_PIN_YELLOW) {
        freq = 600;
    } else {
        freq = 700;
    }
    
    sleep_ms(100);
    gpio_put(led_pin, !gpio_get(led_pin));
    barulho(freq, 700, buzzer);
    gpio_put(led_pin, !gpio_get(led_pin));
}

void inicio() {
    buzzer_led(LED_PIN_RED);
    buzzer_led(LED_PIN_GREEN);
    buzzer_led(LED_PIN_YELLOW);
    buzzer_led(LED_PIN_BLUE);
    sleep_ms(500);
}

void proximaRodada() {
    int cores[4] = {LED_PIN_BLUE, LED_PIN_GREEN, LED_PIN_RED, LED_PIN_YELLOW}; 
    int sorteio = rand() % 4;
    sequencia[rodada] = cores[sorteio];
    rodada++;
}

void reproduzirSequencia() {
    for (int i; i < rodada; i++) {
        buzzer_led(sequencia[i]);
    }
}

void aguardarJogador(){
    for (int i=0; i<rodada; i++) {
        while (!btn) {
        }

        if ((red && sequencia[i]==LED_PIN_RED) || (green && sequencia[i]==LED_PIN_GREEN) || (blue && sequencia[i]==LED_PIN_BLUE) || (yellow && sequencia[i]==LED_PIN_YELLOW)) {
            //acertou
            buzzer_led(sequencia[i]);
        } else {
            //criar efeito luminoso e sonoro para indicar erro
            buzzer_led(LED_PIN_RED);
            buzzer_led(LED_PIN_RED);
            buzzer_led(LED_PIN_RED);
            game_over = true;
            break;
        }
        btn = 0;
    }
}
    
int main() {
    stdio_init_all();
    //init dos leds
    gpio_init(LED_PIN_RED);
    gpio_init(LED_PIN_GREEN);
    gpio_init(LED_PIN_BLUE);
    gpio_init(LED_PIN_YELLOW);

    //init dos botoes
    gpio_init(BUTTON_PIN_RED);
    gpio_init(BUTTON_PIN_GREEN);
    gpio_init(BUTTON_PIN_BLUE);
    gpio_init(BUTTON_PIN_YELLOW);

    // set the LED pin to output
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);
    gpio_set_dir(LED_PIN_YELLOW, GPIO_OUT);

    //botao red
    gpio_set_dir(BUTTON_PIN_RED, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_RED);
    gpio_set_irq_enabled_with_callback(BUTTON_PIN_RED, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    
    //botao green
    gpio_set_dir(BUTTON_PIN_GREEN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_GREEN);
    gpio_set_irq_enabled(BUTTON_PIN_GREEN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
   
    //botao blue
    gpio_set_dir(BUTTON_PIN_BLUE, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_BLUE);
    gpio_set_irq_enabled(BUTTON_PIN_BLUE, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    //botao yellow
    gpio_set_dir(BUTTON_PIN_YELLOW, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_YELLOW);
    gpio_set_irq_enabled(BUTTON_PIN_YELLOW, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    gpio_init(buzzer);
    gpio_set_dir(buzzer, GPIO_OUT);

    inicio();

    while (true) {
        proximaRodada();
        reproduzirSequencia();
        aguardarJogador();

        // para iniciar um novo jogo
        if (game_over){
            rodada = 0;
            //passo = 0;
            game_over = false;
        }
    }
}
