/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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
#ifndef _FSL_I2C_H_
#define _FSL_I2C_H_

#include "fsl_common.h"
#include "fsl_debug_console.h"


void I2C_Init(I2C_Type *base);
//void I2C_SetHoldTime(I2C_Type *base, uint32_t sclStopHoldTime_ns, uint32_t sourceClock_Hz);
void I2C_Start(I2C_Type *base,uint8_t addr_device);
void I2C_Stop(I2C_Type *base);
void I2C_Restart(I2C_Type *base);
void I2C_Send_Byte(I2C_Type *base, uint8_t TxBuff);
volatile uint8_t I2C_Get_One_Byte(I2C_Type *base);
volatile uint8_t I2C_Get_One_Byte_No_Stop(I2C_Type *base);
//uint8_t I2C_Get_Last_Byte(I2C_Type *base);

void I2C_Write_Reg_2Bytes(I2C_Type *base,uint8_t Add_Device, uint8_t Add_Reg, uint8_t Data_MSB, uint8_t Data_LSB);
volatile uint16_t I2C_Read_Reg_2Bytes(I2C_Type *base,uint8_t Add_Device, uint8_t Add_Reg);
void I2C_Init_MAX(void);


#endif /* _FSL_I2C_H_*/
