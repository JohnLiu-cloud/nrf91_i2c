/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <logging/log.h>
#include <zephyr.h>
#include <drivers/i2c.h>
#include <stdio.h>
#include "twi.h"

LOG_MODULE_REGISTER(slm_twi, CONFIG_SLM_LOG_LEVEL);

#if defined(CONFIG_HAS_HW_NRF_TWIM3)
#define TWI_MAX_INSTANCE	4
#elif defined(CONFIG_HAS_HW_NRF_TWIM2)
#define TWI_MAX_INSTANCE	3
#elif defined(CONFIG_HAS_HW_NRF_TWIM1)
#define TWI_MAX_INSTANCE	2
#elif defined(CONFIG_HAS_HW_NRF_TWIM0)
#define TWI_MAX_INSTANCE	1
#endif
#define TWI_ADDR_LEN		2
#define TWI_DATA_LEN		128


static const struct device *slm_twi_dev[TWI_MAX_INSTANCE];
static uint8_t twi_data[TWI_DATA_LEN * 2 + 1];


int do_twi_write(uint16_t index, uint16_t dev_addr, uint8_t *data, uint16_t len)
{
	int ret = -EINVAL;

	ret = i2c_write(slm_twi_dev[index], data, len, dev_addr);
	if (ret < 0) {
		LOG_ERR("Fail to write twi data at address: %hx", dev_addr);
	}

	return ret;
}

int do_twi_read(uint16_t index, uint16_t dev_addr, uint8_t num_read)
{
	int ret = -EINVAL;

	if (!slm_twi_dev[index]) {
		LOG_ERR("TWI device is not opened");
		return ret;
	}

	if (num_read > TWI_DATA_LEN) {
		LOG_ERR("Not enough buffer. Increase TWI_DATA_LEN");
		return -ENOBUFS;
	}

	memset(twi_data, 0, sizeof(twi_data));
	ret = i2c_read(slm_twi_dev[index], twi_data, (uint32_t)num_read, dev_addr);
	if (ret < 0) {
		LOG_ERR("Fail to read twi data");
		return ret;
	}
	return ret;
}

int twi_init(void)
{
#if DT_NODE_HAS_COMPAT_STATUS(DT_NODELABEL(i2c0), nordic_nrf_twim, okay)
	slm_twi_dev[0] = device_get_binding(DT_LABEL(DT_NODELABEL(i2c0)));
	LOG_DBG("bind %s", slm_twi_dev[0]->name);
#endif
#if DT_NODE_HAS_COMPAT_STATUS(DT_NODELABEL(i2c1), nordic_nrf_twim, okay)
	slm_twi_dev[1] = device_get_binding(DT_LABEL(DT_NODELABEL(i2c1)));
	LOG_DBG("bind %s", slm_twi_dev[1]->name);
#endif
#if DT_NODE_HAS_COMPAT_STATUS(DT_NODELABEL(i2c2), nordic_nrf_twim, okay)
	slm_twi_dev[2] = device_get_binding(DT_LABEL(DT_NODELABEL(i2c2)));
	LOG_DBG("bind %s", slm_twi_dev[2]->name);
#endif
#if DT_NODE_HAS_COMPAT_STATUS(DT_NODELABEL(i2c3), nordic_nrf_twim, okay)
	slm_twi_dev[3] = device_get_binding(DT_LABEL(DT_NODELABEL(i2c3)));
	LOG_DBG("bind %s", slm_twi_dev[3]->name);
#endif

	return 0;
}


#define LM27965_ADDR		0x36 
//#define LM27965M_ADDR		0x38
#define LM27_REG_GENERAL	0x10
#define LM27_REG_BANK_A		0xA0
#define LM27_REG_BANK_B		0xB0
#define LM27_REG_BANK_C		0xC0


static int lm27_write_bytes(uint8_t addr, uint8_t *data, uint32_t num_bytes)
{
	do_twi_write( 2, addr, data, num_bytes );
	return 0;
}

void lm27_Init(void)
{

}

void lm27_reg_General( uint8_t ctrl )
{
	uint8_t def = 0x20;
	uint8_t data[2];
	data[0] = LM27_REG_GENERAL;
	data[1] = ctrl|def;
	lm27_write_bytes( LM27965_ADDR, data, 2 );
}

void lm27_reg_bankA( uint8_t bank )
{
	uint8_t def = 0xE0;
	uint8_t data[2];
	data[0] = LM27_REG_BANK_A;
	data[1] = bank|def;
	lm27_write_bytes( LM27965_ADDR, data, 2 );
}

void lm27_reg_bankB(uint8_t bank )
{
	uint8_t def = 0xE0;
	uint8_t data[2];
	data[0] = LM27_REG_BANK_B;
	data[1] = bank|def;
	lm27_write_bytes( LM27965_ADDR, data, 2 );
}

void lm27_reg_bankC( uint8_t bank )
{
	uint8_t def = 0xFC;
	uint8_t data[2];
	data[0] = LM27_REG_BANK_C;
	data[1] = bank|def;
	lm27_write_bytes( LM27965_ADDR, data, 2 );
}

void lm27_bank_test()
{
	lm27_reg_General( 0x07 );

	lm27_reg_bankA(0x1f );

	lm27_reg_bankB( 0x1f );

	lm27_reg_bankC( 0x03 );

	//lm27_reg_bankA( i2c_dev, 0x00 );

	//lm27_reg_bankB( i2c_dev, 0x00 );

	//lm27_reg_bankC( i2c_dev, 0x00 );
}
