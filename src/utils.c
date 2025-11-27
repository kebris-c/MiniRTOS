/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 18:47:57 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/27 02:59:53 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtos.h"

/*==============================================
	TIMING FUNCTIONS
================================================*/
/*
 * get_time_ms():
 *	Returns system time in milliseconds.
 *	- Uses gettimeofday internally.
 *	- Used for scheduling and task delays.
 */
unsigned long	get_time_ms(void)
{
	t_timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000UL) + (tv.tv_usec / 1000UL));
}