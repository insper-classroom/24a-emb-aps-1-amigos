/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

int LED_PIN_RED = 16; //OK
int LED_PIN_GREEN = 17; //OK
int LED_PIN_BLUE = 15;  //OK
int LED_PIN_YELLOW = 14; //ok

int BUTTON_PIN_RED =    12; //ok
int BUTTON_PIN_GREEN =  27; //OK
int BUTTON_PIN_BLUE =   5; //OK
int BUTTON_PIN_YELLOW =18;  //ok

int buzzer = 3;

int volatile green = 0;
int volatile red = 0;
int volatile blue = 0;
int volatile yellow = 0;

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
     

        if (red) {
            gpio_put(LED_PIN_RED, !gpio_get(LED_PIN_RED));
            barulho(440, 1500, buzzer);
            red = 0;
        }
        if (green) {
            gpio_put(LED_PIN_GREEN, !gpio_get(LED_PIN_GREEN));
            sleep_ms(1000);
            gpio_put(LED_PIN_GREEN, 0);
            barulho(440, 1500, buzzer);
            green = 0;
        }
        if (blue) {
            gpio_put(LED_PIN_BLUE, !gpio_get(LED_PIN_BLUE));
            sleep_ms(1000);
            gpio_put(LED_PIN_BLUE, 0);
            barulho(440, 1500, buzzer);
            blue = 0;
        }
        if (yellow) {
            gpio_put(LED_PIN_YELLOW, !gpio_get(LED_PIN_YELLOW));
            sleep_ms(1000);
            gpio_put(LED_PIN_YELLOW, 0);
            barulho(440, 1500, buzzer);
            yellow = 0;
        }
     }

}
