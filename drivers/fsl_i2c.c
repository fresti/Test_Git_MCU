#include "fsl_i2c.h"

static const uint16_t s_i2cDividerTable[] = { 20, 22, 24, 26, 28, 30, 34, 40,
		28, 32, 36, 40, 44, 48, 56, 68, 48, 56, 64, 72, 80, 88, 104, 128, 80,
		96, 112, 128, 144, 160, 192, 240, 160, 192, 224, 256, 288, 320, 384,
		480, 320, 384, 448, 512, 576, 640, 768, 960, 640, 768, 896, 1024, 1152,
		1280, 1536, 1920, 1280, 1536, 1792, 2048, 2304, 2560, 3072, 3840 };

void I2C_Init(I2C_Type *base) {
	base->C1 &= ~(I2C_C1_IICEN_MASK); //Disable I2C before configuration

	uint32_t multiplier;
	uint32_t computedRate;
	uint32_t absError;
	uint32_t bestError = UINT32_MAX;
	uint32_t bestMult = 0u;
	uint32_t bestIcr = 0u;
	uint8_t mult;
	uint8_t i;
	uint32_t srcClock = CLOCK_GetFreq(I2C1_CLK_SRC);

	/* Search for the settings with the lowest error. Mult is the MULT field of the I2C_F register,
	 * and ranges from 0-2. It selects the multiplier factor for the divider. */
	for (mult = 0u; (mult <= 2u) && (bestError != 0); ++mult) {
		multiplier = 1u << mult;

		/* Scan table to find best match. */
		for (i = 0u; i < sizeof(s_i2cDividerTable) / sizeof(uint16_t); ++i) {
			computedRate = srcClock / (multiplier * s_i2cDividerTable[i]);
			absError =
					100000U > computedRate ?
							(100000U - computedRate) : (computedRate - 100000U);

			if (absError < bestError) {
				bestMult = mult;
				bestIcr = i;
				bestError = absError;

				/* If the error is 0, then we can stop searching because we won't find a better match. */
				if (absError == 0) {
					break;
				}
			}
		}
	}
	//PRINTF("\n\r %d, %d, %d \n\r",bestMult, bestIcr, srcClock);
	/* Set frequency register based on best settings. */
	base->F = I2C_F_MULT(bestMult) | I2C_F_ICR(bestIcr);
	base->C1 = I2C_C1_IICEN(1);
}

void I2C_Start(I2C_Type *base, uint8_t addr_device) {
	int i = 0;
	base->C1 |= I2C_C1_TX_MASK;
	base->C1 |= I2C_C1_MST_MASK;

	base->S = I2C_S_IICIF_MASK;
	base->D = addr_device;
	while (!(base->S & I2C_S_IICIF_MASK)) { // on reste dans la boucle jusqua que le bit iicif se met a 1 indiquant le transfert de l'adresse //
//		i++;
//		if (i >= 100) {
//			i = 0;
//			return;
//		}
	}
	while ((base->S & 1U) != 0) {
		i++;
		if (i >= 100) {
			i = 0;
			return;
		}
		base->S = I2C_S_IICIF_MASK;
	}
}

void I2C_Stop(I2C_Type *base) {
	I2C1->S = 0b00010010;
	base->C1 &= ~(I2C_C1_MST_MASK | I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);
	while (base->S & I2C_S_BUSY_MASK) {
	}
}
void I2C_Restart(I2C_Type *base) {
	base->C1 |= 1 << 2;
}
void I2C_Send_Byte(I2C_Type *base, uint8_t TxBuff) {
	while ((base->S & (1 << 7)) == 0) {
	}
	int i = 0;
	base->S = I2C_S_IICIF_MASK;
	base->C1 |= 1 << 4;                //write mode
	base->D = TxBuff;				 //write the data in data register
	while (!(base->S & I2C_S_IICIF_MASK)) {
//		i++;
//		if (i >= 100) {
//			i = 0;
//			return;
//		}
	}			 //transfer is finished
	while ((base->S & 1U) != 0) {  //ACK received
		i++;
		if (i >= 100) {
			i = 0;
			return;
		}
	}
}

volatile uint8_t I2C_Get_One_Byte(I2C_Type *base) {
	volatile uint8_t dummy, data = 0;
	dummy++;
	int i;
	while ((base->S & (1 << 7)) == 0) {
	}
	base->S = I2C_S_IICIF_MASK;
	base->C1 &= ~(1 << 4);             //read mode
	base->C1 &= ~(1 << 3);			 //clear TXACK
	base->C1 |= (1 << 3);			 //send NACK
	dummy = base->D;				//do dummy read
	while (!(base->S & I2C_S_IICIF_MASK)) {
//		i++;
//		if (i >= 100) {
//			i = 0;
//			return;
//		}
	}
	base->S = I2C_S_IICIF_MASK;
	I2C_Stop(base);
	data = base->D;			 //return the Data register
	return data;
}

void I2C_Write_Reg_2Bytes(I2C_Type *base, uint8_t Add_Device, uint8_t Add_Reg,
		uint8_t Data_MSB, uint8_t Data_LSB) {
	I2C_Start(base, Add_Device);
	I2C_Send_Byte(base, Add_Reg);
	I2C_Send_Byte(base, Data_LSB);
	I2C_Send_Byte(base, Data_MSB);
	I2C_Stop(base);
}

volatile uint16_t I2C_Read_Reg_2Bytes(I2C_Type *base, uint8_t Add_Device,
		uint8_t Add_Reg) {
	uint8_t LSB, MSB;
	int i;
	I2C_Start(base, Add_Device);
	I2C_Send_Byte(base, Add_Reg);
	I2C_Restart(base);
	I2C_Send_Byte(base, (Add_Device | 0b1));
	volatile uint8_t dummy, data = 0;
	dummy++;
	while ((base->S & (1 << 7)) == 0) {
	}
	base->S = I2C_S_IICIF_MASK;
	base->C1 &= ~(1 << 4);             //read mode
	base->C1 &= ~(1 << 3);			 //clear TXACK
	dummy = base->D;				//do dummy read
	while (!(base->S & I2C_S_IICIF_MASK)) {
//		i++;
//		if (i >= 100) {
//			i = 0;
//			return;
//		}
	}
	base->S = I2C_S_IICIF_MASK;
	base->C1 |= (1 << 3);			 //send NACK
	MSB = base->D;			 //return the Data register
	while (!(base->S & I2C_S_IICIF_MASK)) {
//		i++;
//		if (i >= 100) {
//			i = 0;
//			return;
//		}
	}
	base->S = I2C_S_IICIF_MASK;
	I2C_Stop(base);
	LSB = base->D;

	return (MSB << 8 | LSB);
}

void I2C_Init_MAX(void) {
	I2C_Write_Reg_2Bytes(I2C1, 0x16, 0xa5, 0x04, 0x88);
	I2C_Write_Reg_2Bytes(I2C1, 0x16, 0xa8, 0xfc, 0x18);
	I2C_Write_Reg_2Bytes(I2C1, 0x16, 0xa9, 0x03, 0xe8);
	I2C_Write_Reg_2Bytes(I2C1, 0x16, 0xb3, 0x03, 0xe8);
	I2C_Write_Reg_2Bytes(I2C1, 0x16, 0xb6, 0x3c, 0x01);
	I2C_Write_Reg_2Bytes(I2C1, 0x16, 0xba, 0xff, 0x8a);
}
