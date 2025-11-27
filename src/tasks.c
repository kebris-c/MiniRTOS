/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:10:05 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/27 03:00:52 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtos.h"

/*==============================================================================
    TASKS
==============================================================================*/
/*
 * task_sensor():
 *	Simulates reading from a 12-bit ADC sensor.
 *	- Sends the raw sensor value to QUEUE_SENSOR.
 *	- Uses rtos_delay to simulate acquisition time.
 *	Notes:
 *		- Each task iteration is independent.
 *		- Uses pc and static variables to maintain state across yields.
 */
void	task_sensor(void *arg)
{
	static int16_t	adc_raw_data = 0;

	switch (g_curr_task->pc)
	{
		case (0):
			adc_raw_data = (int16_t)driver_adc_read();
			if (queue_send_msg(QUEUE_SENSOR, &adc_raw_data, \
					sizeof(adc_raw_data)) == -1)
				return ;
			rtos_delay(50);
			return ;
	}
}

/*
 * task_proc():
 *	Processes raw sensor data from QUEUE_SENSOR.
 *	- Converts raw ADC value to Celsius.
 *	- Determines fan speed based on temperature thresholds.
 *	- Sends processed temperature to QUEUE_PROC.
 *	- Uses pc to split operations across scheduler
 *		cycles (cooperative multitasking).
 *	Notes:
 *		- Simulates critical component temperature monitoring.
 *		- Typical 12-bit ADC conversion: 0-4096 mapped to -40 to +125°C.
 */
void	task_proc(void *arg)
{
	static int16_t	raw = 0;
	static int16_t	celsius = 0;
	static int		fan_speed_percent = 0;

	switch (g_curr_task->pc)
	{
		case (0):
			if (queue_recv_msg(QUEUE_SENSOR, &raw, sizeof(int)) == -1)
			{
				rtos_yield();
				return ;
			}
			g_curr_task->pc++;
			rtos_yield();
			return ;
		case (1):
			celsius = (raw / 4096.0) * 165 - 40;
			if (celsius > THRESH_CRITICAL)
				fan_speed_percent = 100;
			else if (celsius > THRESH_HIGH)
				fan_speed_percent = 75;
			else if (celsius > THRESH_MID)
				fan_speed_percent = 50;
			else if (celsius > THRESH_LOW)
				fan_speed_percent = 25;
			else
				fan_speed_percent = 0;
			driver_fan_set(0, fan_speed_percent);
			driver_fan_set(1, fan_speed_percent);
			g_curr_task->pc++;
			rtos_yield();
			return ;
		case (2):
			if (queue_send_msg(QUEUE_PROC, &celsius, sizeof(celsius)) == -1)
			{
				rtos_yield();
				return ;
			}
			g_curr_task->pc = 0;
			return ;
	}
}

/*
 * task_logger():
 *	Receives processed temperature from QUEUE_PROC.
 *	- Formats a textual log message.
 *	- Sends the message via UART using driver_uart_send.
 *	- Acts as centralized logging task.
 *	Notes:
 *		- Uses pc and static buffers to cooperate with the scheduler.
 *		- Non-blocking: yields if no messages are available.
 */
void	task_logger(void *arg)
{
	static int16_t	value;
	static char		buf[64];
	int				len;

	switch (g_curr_task->pc)
	{
		case (0):
			if (queue_recv_msg(QUEUE_PROC, &value, sizeof(value)) == -1)
			{
				rtos_yield();
				return ;
			}
			g_curr_task->pc++;
			rtos_yield();
			return ;
		case (1):
			len = snprintf(buf, sizeof(buf), "[LOG] temp=%dºC\n", value);
			driver_uart_send(buf, (size_t)len);
			g_curr_task->pc = 0;
			return ;
		}
}