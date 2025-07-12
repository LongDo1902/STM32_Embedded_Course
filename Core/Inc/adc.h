/*
 * adc.h
 *
 *  Created on: Jul 12, 2025
 *      Author: dobao
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <stdio.h>
#include <stdint.h>

#include "rcc.h"

void ADC_tempSensorInit();
float tempSensorRead();

#endif /* INC_ADC_H_ */
