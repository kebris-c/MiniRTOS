/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drivers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 00:32:49 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/27 03:00:07 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtos.h"

/*
 * driver_adc_read():
 *	Simulates a 12-bit ADC sensor read.
 *	- Returns integer values 0-4095.
 *	- Example incrementing pattern for test purposes.
 *	Notes:
 *		- Replace with actual ADC read in hardware.
 */
int	driver_adc_read(void)
{
	static int	adc_counter = 0;

	adc_counter = (adc_counter + 100) % 4096;
	return (adc_counter);
}

/*
 * driver_fan_set():
 *	Sets fan speed for simulated fans.
 *	- fan_id: identifies which fan to control.
 *	- speed_percent: 0-100.
 *	Notes:
 *		- In this simulation, prints to stdout.
 *		- In real hardware, would write PWM duty cycle or equivalent.
 */
void	driver_fan_set(int fan_id, int speed_percent)
{
	printf("[FAN] id=%d speed=%d%%\n", fan_id, speed_percent);
}

/*
 * driver_uart_send():
 *	Sends a buffer over UART (simulated).
 *	- buffer: pointer to bytes to send.
 *	- len: number of bytes to send.
 *	Notes:
 *		- Here it writes to stdout using fwrite.
 *		- In real hardware, would interact with UART peripheral registers.
 */
void	driver_uart_send(const void *buffer, size_t len)
{
	fwrite(buffer, 1, len, stdout);
}