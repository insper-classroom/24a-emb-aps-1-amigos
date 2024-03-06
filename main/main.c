/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
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

int pontuacao = 0;
int sequencia[100];
int sequencia_jogador[100];
int rodada = 0;
int passo = 0;
int botao_pressionado = 0;
bool game_over = false;

//******************* CALLBACKS ******************* 
void btn_callback(uint gpio, uint32_t events) {
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

void proximaRodada(){
    int sorteio = rand() % 4;
    sequencia[rodada] = sorteio;
    rodada++;
}

void reproduzirSequencia(){
    for(int i = 0; i < rodada; i++){
        barulho(tons[sequencia[i]], 500, buzzer);
        gpio_put(leds[sequencia[i]], 1);
        sleep_ms(500);
        gpio_put(leds[sequencia[i]], 0);
        sleep_ms(500);
    }
}

void aguardarJogador(){
    for (int i = 0; i < rodada; i++){
        botao_pressionado = 0;
        bool jogada_efetuada = false;
        while (!jogada_efetuada){
            for (int j = 0; j < 4; j++){
                if (gpio_get(botoes[j]) == 0){
                    botao_pressionado = j;
                    barulho(tons[j], 500, buzzer);
                    jogada_efetuada = true;
                }
            }
    }
    
    if (sequencia[passo] != botao_pressionado){
        //criar efeito luminoso e sonoro para indicar erro
        for (int i = 0; i < 3; i++){
            gpio_put(leds[sequencia[passo]], 1);
            barulho(262, 500, buzzer);
            sleep_ms(500);
            gpio_put(leds[sequencia[passo]], 0);
            sleep_ms(500);
        }
        game_over = true;
        break;
        }
        passo++;
}
    passo = 0;
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




    while (true) {

    

    proximaRodada();
    reproduzirSequencia();
    aguardarJogador();
    // para iniciar um novo jogo
    if (game_over){
        rodada = 0;
        passo = 0;
        game_over = false;
    }

}
}
