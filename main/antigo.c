/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

int const LED_PIN_RED = 16; //OK
int const LED_PIN_GREEN = 17; //OK
int const LED_PIN_BLUE = 15;  //OK
int const LED_PIN_YELLOW = 14; //ok

int const BUTTON_PIN_RED =    12; //ok
int const BUTTON_PIN_GREEN =  27; //OK
int const BUTTON_PIN_BLUE =   5; //OK
int const BUTTON_PIN_YELLOW =18;  //ok

int const buzzer = 3;

int volatile green = 0;
int volatile red = 0;
int volatile blue = 0;
int volatile yellow = 0;

int volatile inicio = 1;
int volatile sequencia = 0;
int volatile rodada = 5;
int volatile player = 0;

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

    //ligar os leds para teste
    // gpio_put(LED_PIN_RED, !gpio_get(LED_PIN_RED));
    // gpio_put(LED_PIN_GREEN, !gpio_get(LED_PIN_GREEN));
    // gpio_put(LED_PIN_BLUE, !gpio_get(LED_PIN_BLUE));

        while (inicio) {
            buzzer_led(LED_PIN_RED);
            buzzer_led(LED_PIN_GREEN);
            buzzer_led(LED_PIN_YELLOW);
            buzzer_led(LED_PIN_BLUE);
            sleep_ms(500);

            inicio = 0;
            sequencia = 1;
        }

        while (sequencia) {
            int cores[4] = {LED_PIN_BLUE, LED_PIN_GREEN, LED_PIN_RED, LED_PIN_YELLOW}; 

            for (int i; i <= rodada; i++) {
                int indice = rand() % 4;
                int cor = cores[indice];
                buzzer_led(cor);
            }
            
            rodada += 1;
            sequencia = 0;
            player = 1;
        }

        while (player) {
            if (red) {
                sleep_ms(100);
                gpio_put(LED_PIN_RED, !gpio_get(LED_PIN_RED));
                barulho(400, 700, buzzer);
                gpio_put(LED_PIN_RED, !gpio_get(LED_PIN_RED));
                red = 0;
            }
            if (green) {
                sleep_ms(100);
                gpio_put(LED_PIN_GREEN, !gpio_get(LED_PIN_GREEN));
                barulho(500, 700, buzzer);
                gpio_put(LED_PIN_GREEN, 0);
                green = 0;
            }
            if (blue) {
                sleep_ms(100);
                gpio_put(LED_PIN_BLUE, !gpio_get(LED_PIN_BLUE));
                barulho(600, 700, buzzer);
                gpio_put(LED_PIN_BLUE, 0);
                blue = 0;
            }
            if (yellow) {
                sleep_ms(100);
                gpio_put(LED_PIN_YELLOW, !gpio_get(LED_PIN_YELLOW));
                barulho(700, 700, buzzer);
                gpio_put(LED_PIN_YELLOW, 0);
                yellow = 0;
            }
        }

        //aperta botao, acende luz faz som
     }

}
