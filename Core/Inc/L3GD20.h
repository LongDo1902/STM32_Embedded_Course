/*
 * L3GD20.h
 *
 *  Created on: Apr 8, 2025
 *      Author: dobao
 */

#ifndef INC_L3GD20_H_
#define INC_L3GD20_H_

#define WHO_AM_I		0x0F //Device ID register
#define CTRL_REG1		0x20 //Power mode, data rate, bandwidth, axis enable
#define CTRL_REG2		0x21 //High-pass filter config
#define CTRL_REG3		0x22 //Interrupt and output signal config
#define CTRL_REG4		0x23 //Data format and full-scale sel
#define CTRL_REG5		0x24 //FIFO, high-pass filter, output sel

#define REFERENCE		0x25 //Ref val for interrupt generation
#define OUT_TEMP		0x26 //Internal temperature output (raw, 8-bit)
#define STATUS_REG		0x27 //Data ready and overrun status

#define OUT_X_L			0x28 //X-axis angular rate (LSB)
#define OUT_X_H			0x29 //X-axis angular rate (MSB)
#define OUT_Y_L			0x2A //Y-axis angular rate (LSB)
#define OUT_Y_H			0x2B //Y-axis angular rate (MSB)
#define OUT_Z_L			0x2C //Z-axis angular rate (LSH)
#define OUT_Z_H			0x2D //Z-axis angular rate (MSB)

#define FIFO_CTRL_REG	0x2E //FIFO mode, watermark threshold
#define FIFO_SRC_REG	0x2F //FIFO status (level, full, empty, watermark)

#define INT1_CFG		0x30 //Axis, direction config for INT1 trigger
#define INT1_SRC		0x31 //INT1 status: which axis/direction triggered

#define INT1_TSH_XH		0x32 //X-axis interrupt threshold (MSB)
#define INT1_TSH_XL		0x33 //X-axis interrupt threshold (LSB)
#define INT1_TSH_YH		0x34 //Y-axis interrupt threshold (MSB)
#define INT1_TSH_YL		0x35 //Y-axis interrupt threshold (LSB)
#define INT1_TSH_ZH		0x36 //Z-axis interrupt threshold (MSB)
#define INT1_TSH_ZL		0x37 //Z-axis interrupt threshold (LSB)

#define INT1_DURATION	0x38 //Min duration condition must hold to trigger INT1

////////////////////////////END OF REGISTER MAPPING////////////////////////////

/*
 * WHO_AM_I (default: 1101 0100)
 */
#define I_AM_L3GD20		((uint8_t)0xD4) //Default value

/*
 * CTRL_REG1 (default: 0000 0111)
 * Axes enable and disable
 * Power modes
 * Output Data Rate and Bandwidth
 */
#define L3GD20_X_ENABLE		((uint8_t)0x01) //Set bit 1 to 1
#define L3GD20_Y_ENABLE 	((uint8_t)0x02) //Set bit 2 to 1
#define L3GD20_Z_ENABLE 	((uint8_t)0x04) //Set bit 3 to 1
#define L3GD20_AXES_ENABLE	((uint8_t)0x07) //Set bit 1-3 to 1
#define L3GD20_AXES_DISABLE	((uint8_t)0x00) //Set bit 1-3 to 0

#define L3GD20_MODE_POWERDOWN	((uint8_t)0x00) //Set bit 4 to 0
#define L3GD20_ACTIVE			((uint8_t)0x08)) //Set but 4 to 1

#define L3GD20_ODR95HZ_BW12HZ	((uint8_t)0x00) //ODR 95Hz, Cut-off 12.5Hz
#define L3GD20_ODR95HZ_BW25HZ	((uint8_t)0x30) //ODR 95Hz, Cut-off 25Hz

#define L3GD20_ODR380HZ_BW20Hz	((uint8_t)0x80) //ODR 380Hz, Cut-off 20Hz
#define L3GD20_ODR380HZ_BW100HZ	((uint8_t)0xB0) //ODR 380Hz, Cut-off 100Hz

#define L3GD20_ODR760HZ_BW30HZ	((uint8_t)0xC0) //ODR 760Hz, Cut-off 30Hz
#define L3GD20_ODR760HZ_BW100HZ	((uint8_t)0xF0) //ODR 760Hz, Cut-off 100Hz

/*
 * CTRL_REG2 (default: 0000 0000)
 * High-pass filter mode selection
 * High-pass filter cut-off freq config
 */
#define L3GD20_HPM_NORMALMODE_MANUALRES	((uint8_t)0x00) //Only reset when read the REFERENCE reg
#define L3GD20_HPM_REFSIGNAL			((uint8_t)0x10) //Use val of REFERENCE reg as the baseline
#define L3GD20_HPM_NORMALMODE			((uint8_t)0x20) //Internal reset, auto-adjusts its baseline internally
#define L3GD20_HPM_AUTORESET_INT		((uint8_t)0x30) //Reset automatically after an interrupt event

//Correspond to 95Hz, 380Hz, 760Hz ODR
#define L3GD20_HPFCF1	((uint8_t)0x00)	//7.2HZ, 27Hz, 51.4Hz
#define L3GD20_HPFCF2	((uint8_t)0x01) //3.5Hz, 13.5Hz, 27Hz
#define L3GD20_HPFCF3	((uint8_t)0x02) //1.8Hz, 7.2Hz, 13.5Hz
#define L3GD20_HPFCF4	((uint8_t)0x03) //0.9Hz, 3.5Hz, 7.2Hz
#define L3GD20_HPFCF5	((uint8_t)0x04) //0.45Hz, 1.8Hz, 3.5Hz
#define L3GD20_HPFCF6	((uint8_t)0x05) //0.18Hz, 0.9Hz, 1.8Hz
#define L3GD20_HPFCF7	((uint8_t)0x06) //0.09Hz, 0.45Hz, 0.9Hz
#define L3GD20_HPFCF8	((uint8_t)0x07) //0.045Hz, 0.18Hz, 0.45Hz
#define L3GD20_HPFCF9	((uint8_t)0x08) //0.018Hz, 0.09Hz, 0.18Hz
#define L3GD20_HPFCF10	((uint8_t)0x09) //0.009Hz, 0.045Hz, 0.09Hz

/*
 * CTRL_REG3 (default: 0000 0000)
 */
#define L3GD20_INT2_FIFO_EMPTY_ENABLE		((uint8_t)0x01)
#define L3GD20_INT2_FIFO_OVERRUN_ENABLE		((uint8_t)0x02)
#define L3GD20_INT2_WATERMARK_ENABLE		((uint8_t)0x04)
#define L3GD20_INT2_DATAREADY_ENABLE		((uint8_t)0x08)

#define L3GD20_PUSHPULL				((uint8_t)0x00)
#define L3GD20_OPENDRAIN			((uint8_t)0x10)

#define L3GD20_INT1_HIGHEDGE		((uint8_t)0x00)
#define L3GD20_INT1_LOWEDGE			((uint8_t)0x20)

#define L3GD20_INT1_BOOT_ENABLE		((uint8_t)0x40)
#define L3GD20_INT1_BOOT_DISABLE	((uint8_t)0x00)

#define L3GD20_INT1_ENABLE			((uint8_t)0x80)
#define L3GD20_INT1_DISABLE			((uint8_t)0x00)

/*
 * CTRL_REG4 (default: 0000 0000)
 */
#define L3GD20_SPI_4W			((uint8_t)0x00) //Standard SPI, 4-wire SPI
#define L3GD20_SPI_3W			((uint8_t)0x01) //3-wire SPI

#define L3GD20_FS_250DPS		((uint8_t)0x00) //Works for slow rotations (Slow, stable drone), high resolution
#define L3GD20_FS_500DPS		((uint8_t)0x10) //Works for medium rotations (Acro Flight)
#define L3GD20_FS_2000DPS		((uint8_t)0x20) //Works for fast spins (Racing, FPV drone), low resolution

#define L3GD20_LITTLE_ENDIAN	((uint8_t)0x00) //LSB is in lower register addr (Recommended sel)
#define L3GD20_BIG_ENDIAN	 	((uint8_t)0x40) //MSB is in lower register addr

#define L3GD20_BDU_CONTINUOS	((uint8_t)0x00) //Data can change while reading
#define L3GD20_BDU_LOCK			((uint8_t)0x80) //Lock data update until finishing reading MSB and LSB (Recommended Sel)


#endif /* INC_L3GD20_H_ */



















