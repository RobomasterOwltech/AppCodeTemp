#include "stdint.h"
#include "stm32f1xx.h"

#define HEADLIGHT_PIN GPIO_PIN_0
#define MOTOR_PWM_PIN GPIO_PIN_1

void pwm_init() {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
    GPIOA->CRL |= (GPIO_CRL_MODE1_1 | GPIO_CRL_MODE1_0 | GPIO_CRL_CNF1_1);

    TIM2->PSC = 1 - 1;
    TIM2->ARR = 255;
    TIM2->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;
    TIM2->CCER |= TIM_CCER_CC2E;
    TIM2->CR1 |= TIM_CR1_CEN;
}

void set_motor_speed(uint8_t duty_cycle) { TIM2->CCR2 = duty_cycle; }

void gpio_init() {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    GPIOA->CRL |= GPIO_CRL_CNF0_1;
    GPIOA->ODR |= HEADLIGHT_PIN;
}

void motor_control_task() {
    if (!(GPIOA->IDR & HEADLIGHT_PIN)) {
        set_motor_speed(178);
    } else {
        set_motor_speed(0);
    }
}

int main(void) {
    gpio_init();
    pwm_init();

    osThreadDef(THREAD_1, motor_control_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    osThreadId LEDThread1Handle = osThreadCreate(osThread(THREAD_1), NULL);

    osKernelStart();

    while (1) {
        motor_control_task();
    }
}
