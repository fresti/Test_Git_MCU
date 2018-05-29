/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/************Include************/
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "time.h"
/******************************/

#define mainQUEUE_LENGTH (1)
#define BOARD_LED_GPIO GPIOD
#define BOARD_LED_GPIO_PIN 1<<6U
long i;
long j;


void delay(int ms){
    long i;
    long j = ms*4000;
    for (i = 0; i <= j; i++) {

    }
}



int main(void) {

	gpio_pin_config_t pin_config_input = { kGPIO_DigitalInput, 1, };
	gpio_pin_config_t pin_config_output = { kGPIO_DigitalOutput, 0, };

	BOARD_InitPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();

	GPIO_PinInit(GPIOE, 24U, &pin_config_input);
	GPIO_PinInit(GPIOD, 6U, &pin_config_output);

	BOARD_I2C_ConfigurePins();

	printf("Televersement effectue\n");
	printf("Here is a new branch\n");
	int i = 0, j = 0;

	while(1){
		i++;
		j++;

		if(i > 100){
			printf("i = %d\n", i);
			i = 0;
		}

		printf("TU es dans une nouvelle branch ! \n");
	}

}
