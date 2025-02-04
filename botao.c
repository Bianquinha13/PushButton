#include <stdio.h>
#include "pico/stdlib.h"

//Definindo os LEDs e o botão.
#define BLUE_LED_PIN 11
#define RED_LED_PIN 12
#define GREEN_LED_PIN 13
#define BUTTON 5

volatile bool leds_active = false;
volatile int led_state = 0;

int64_t alarm_callback_2(alarm_id_t id, void *user_data);
int64_t alarm_callback_1(alarm_id_t id, void *user_data);

//Callback para desligar três leds.
int64_t alarm_callback_3(alarm_id_t id, void *user_data) {
    gpio_put(GREEN_LED_PIN, false);
    leds_active = false; // Permite novo acionamento do botão
    return 0;
}

//Callback para desligar dois leds.
int64_t alarm_callback_2(alarm_id_t id, void *user_data) {
    gpio_put(RED_LED_PIN, false);
    add_alarm_in_ms(3000, alarm_callback_3, NULL, false);
    return 0;
}

//Callback para desligar 1 led.
int64_t alarm_callback_1(alarm_id_t id, void *user_data) {
    gpio_put(BLUE_LED_PIN, false);
    add_alarm_in_ms(3000, alarm_callback_2, NULL, false);
    return 0;
}

void button_callback(uint gpio, uint32_t events){
    if (!leds_active) {
        leds_active = true;
        gpio_put(BLUE_LED_PIN, true);
        gpio_put(RED_LED_PIN, true);
        gpio_put(GREEN_LED_PIN, true);
        add_alarm_in_ms(3000, alarm_callback_1, NULL, false);
    }
}

int main()
{
    stdio_init_all();

    //Iniciando os Leds e Botão
    gpio_init(BLUE_LED_PIN);
    gpio_set_dir(BLUE_LED_PIN, GPIO_OUT);

    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);

    gpio_init(GREEN_LED_PIN);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);

    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);

    //Configurar interrupção para botão
    gpio_set_irq_enabled_with_callback(BUTTON, GPIO_IRQ_EDGE_RISE, true, &button_callback);

    while (true) {
        printf("Botão...\n");
        sleep_ms(1000);
    }
}
