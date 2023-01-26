
#include "lib/include.h"


// La Tiva Chica cuenta con dos módulos de PWM0 y PWM1
// y cada módulo contiene 4 generadores de PWM, además de que cada generador
// proporciona dos salidas como pwmA y pwmB (comparten la frecuencia)

//Experimento 1
/*Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 50,000,000 Hz
 * junto con el generador 1  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 10KHz
 */
//Experimento 2
/*Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
 * junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 50Hz con tres potenciometros variar el ciclo de trabajo
 * para controlar la posicion de tres servos sg90 o otros.
 *
 */
//Experimento 3
/*Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
 * junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 50Hz , utilizando el uart de la practica 3
 * se enviara dato desde interfaz de simulink para controlar la intensidad luminosa 
 * usando un led RGB externa 
 *
 */

extern void PWM_CONFIGURATION(int module, int divisor,  int freq, int f_clk, int duty_cycle)
{
    SYSCTL->RCGCPWM |= (1 << module); // 0 o 1 para cada módulo
    SYSCTL->RCGCGPIO |= 0x2; // Activamos puerto B, A corresponde a 0, B a 1 hasta F a 5.
    if (divisor == 0){
        SYSCTL->RCC &= ~0x00100000; // el bit 20 es el de USEPWMDIV
    }
    else if (divisor == 64){
        SYSCTL->RCC |= 0x00100000; // el bit 20 es el de USEPWMDIV
        SYSCTL->RCC |= 0x000E0000; // 64 divisor
        f_clk = f_clk / divisor;
    }
    GPIOB->AFSEL |= (1 << 4) | (1 << 5); // PB4 Y PB5 PINS
    GPIOB->PCTL &= ~0x00FF0000;//0b11111111000000001111111111111111; Poner en 0 los pines 4 y 5
    GPIOB->PCTL |= 0x00440000;//0b00000000010001000000000000000000; // Configurar modo de PWM asignando el valor decimal al campo de bit de los pines 4 y 5
    GPIOB->DEN |= (1 << 4) | (1 << 5); // Digital Enable para los mismos pines del AFSEL

    PWM0 -> _1_CTL &= ~(1 << 0); // Deshabilita el contador del generador 1
    PWM0 -> _1_CTL &= ~(1 << 1); // COUNT DOWN MODE

    PWM0->_1_GENA = 0x0000008C; // Cuando contador = LOAD Drive pwmA High, when comparator A matches counter Drive pwmA Low
    //PWM0->_1_GENB = 0x0000008C;
    // con f_clk Mhz de reloj
    PWM0->_1_LOAD = (f_clk/freq); /*cuentas=fclk/fpwm  para 1khz cuentas = (16,000,000/1600)*/
    PWM0->_1_CMPA = (int)((1.0 - (duty_cycle / 100.0)) *(f_clk / freq));//2499; //(int)((duty_cycle / 100) * (int)(f_clk / freq)) - 1;//(1 - (duty_cycle / 100)) * (int)(f_clk / freq);
    PWM0 -> _1_CTL = 1;
    PWM0->ENABLE = 0xC; // Habilitar channel 2 y 3
}

