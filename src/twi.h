/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef TWI_H_
#define TWI_H_

/**@file at_twi.h
 *
 * @brief Vendor-specific AT command for TWI service.
 * @{
 */

/**
 * @brief Initialize TWI AT command parser.
 *
 * @retval 0 If the operation was successful.
 *           Otherwise, a (negative) error code is returned.
 */
int twi_init(void);

int do_twi_write(uint16_t index, uint16_t dev_addr, uint8_t *data, uint16_t len);

void lm27_bank_test();

/** @} */

#endif /* TWI_H_ */
