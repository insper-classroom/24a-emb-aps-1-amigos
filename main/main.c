
//  * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
//  *
//  * SPDX-License-Identifier: BSD-3-Clause
//  */

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

int BUTTON_PIN_RED =    12; //ok
int BUTTON_PIN_GREEN =  27; //OK
int BUTTON_PIN_BLUE =   5; //OK
int BUTTON_PIN_YELLOW =18;  //ok
int BUTTTON_PIN_START = 19;

int buzzer = 3;


int volatile green = 0;
int volatile red = 0;
int volatile blue = 0;
int volatile yellow = 0;
int volatile start = 0;

int volatile rodada = 0;
int volatile sequencia[16];
int volatile game_over = 0;
int volatile pontuacao = 0;


//******************* CALLBACKS ******************* 
void btn_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTTON_PIN_START) {
        start = 1;
    }

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
void tocar_musica_tema(int time_ms) {
    

    //fur elise
    // int notas[] = {659, 622, 659, 622, 659, 494, 587, 523, 587, 494, 587, 440, 523, 349, 415, 494, 659, 622, 659, 622, 659, 494, 587, 523, 587, 494, 587, 440, 523, 349, 415, 494, 659, 622, 659, 622, 659, 494, 587, 523, 587, 494, 587, 440, 523, 349, 415, 494, 659, 622, 659, 622, 659, 494, 587, 523, 587, 494, 587, 440, 523, 349, 415, 494};
    // int duracoes[] = {200, 200, 200, 200, 200, 200, 200, 800, 200, 200, 200, 200, 600, 200, 200, 200, 600, 800, 200, 200, 200, 200, 200, 200, 200, 200, 800, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200};

    // Definir as frequências das notas musicais correspondentes à melodia de Harry Potter (uma oitava acima)
    int notas [] = {329, 440, 523, 494, 440, 329, 294, 494, 440, 523, 494, 392, 494, 329, 329, 440, 523, 494};
    int duracoes[] = {300, 300, 300, 300, 300, 300, 600, 300, 300, 600, 300, 300, 600, 300, 300, 300, 300, 300};

    // int notas[] = {329, 440, 523, 494, 440, 329, 294, 494, 440, 523, 494, 392, 494, 329, 329, 440, 523, 494, 440, 329, 392, 370, 349, 277, 349, 329, 329, 329, 523, 440, 523};
    // int duracoes[] = {300, 300, 300, 300, 300, 300, 1000, 300, 300, 300, 300, 300, 300, 1000, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 1000, 300, 300, 1000};
    // int duracoes[] = {300,300,300,300,300,300,600,300,300,300,300,300,600,300,300,300,300,300,300,300,300,300,300,300,300,300,600,300,300,600};
    int num_notas = sizeof(notas) / sizeof(notas[0]);


    // for (int i = 0; i < num_notas; i++) {
    //     if ((time_us_64() - start_time) / 1000 >= time_ms) {
    //         break; } // Parar se já tiver passado o tempo especificado
        for (int j = 0; j < num_notas ; j++) {
            barulho(notas[j], duracoes[j], buzzer); 
            sleep_ms(100);
        }
        sleep_ms(2000);
        // Mantém o buzzer ligado pelo tempo da nota
         // Desliga o buzzer

         // Breve pausa entre as notas
         
    }
// }

void inicio() {
    tocar_musica_tema(6000); // Toca a música tema de Harry Potter

    // buzzer_led(LED_PIN_BLUE);
    // buzzer_led(LED_PIN_YELLOW);
    // buzzer_led(LED_PIN_GREEN);
    // buzzer_led(LED_PIN_RED);
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

    gpio_put(led_pin, !gpio_get(led_pin));
    barulho(freq, 700, buzzer);
    gpio_put(led_pin, !gpio_get(led_pin));
}


void proximaRodada() {
    const cores[4] = {LED_PIN_BLUE, LED_PIN_GREEN, LED_PIN_RED, LED_PIN_YELLOW}; 
    int sorteio = rand() % 4;
    sequencia[rodada] = cores[sorteio];
    rodada++;
}

void reproduzirSequencia() {
    for (int i=0; i < rodada; i++) {
        buzzer_led(sequencia[i]);
        sleep_ms(100);
    }
}

void aguardarJogador(){
    for (int i=0; i<rodada; i++) {
        while ((red == 0) && (green == 0) && (blue ==0) && (yellow == 0)) {
            sleep_ms(10);
        }

        if ((red && sequencia[i]==LED_PIN_RED) || 
            (green && sequencia[i]==LED_PIN_GREEN) || 
            (blue && sequencia[i]==LED_PIN_BLUE) || 
            (yellow && sequencia[i]==LED_PIN_YELLOW)) {
            //acertou
            buzzer_led(sequencia[i]);

            pontuacao  = rodada;
            red = 0;
            blue = 0;
            green = 0;
            yellow = 0;

        } else {
            //criar efeito luminoso e sonoro para indicar erro
            buzzer_led(LED_PIN_RED);
            buzzer_led(LED_PIN_RED);
            buzzer_led(LED_PIN_RED);
            game_over = 1;
            break;
        }
    }

    sleep_ms(300);
}
    
int main() {
    stdio_init_all();

    srand(time(NULL)); 

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

    //botao start
    gpio_set_dir(BUTTTON_PIN_START, GPIO_IN);
    gpio_pull_up(BUTTTON_PIN_START);
    gpio_set_irq_enabled_with_callback(BUTTTON_PIN_START, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    //botao red
    gpio_set_dir(BUTTON_PIN_RED, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_RED);
    gpio_set_irq_enabled(BUTTON_PIN_RED, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    
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

    while (1) {
        if (start) {
            proximaRodada();
            reproduzirSequencia();
            aguardarJogador();

            // para iniciar um novo jogo
            if (game_over){
                game_over = 0;
                for (int i = 0; i < pontuacao ; i++){
                    buzzer_led(LED_PIN_GREEN);
                    sleep_ms(100);
                }
                start = 0;
                
            }
        }

        if (start == 0){
            rodada = 0;
            red = 0;
            green = 0;
            blue = 0;
            yellow = 0;
            pontuacao = 0;
            
            // buzzer_led(LED_PIN_GREEN);
            // buzzer_led(LED_PIN_BLUE);

    
            // desliga o programa    
        }

        if (rodada == 5){
            //vitoria
            tocar_musica_tema(8000);
            rodada = 0;
            red = 0;
            green = 0;
            blue = 0;
            yellow = 0;
            game_over = 0;
            start = 0;
            sleep_ms(500);
        
        }
            
        }
        
        
}



