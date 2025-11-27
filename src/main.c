/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 18:44:49 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/27 01:05:56 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtos.h"

/*==============================================================================
    MAIN
==============================================================================*/
int	main(void)
{
	ft_banner("Minirtos");
	ft_printf("===\tTEST\t===\n\n");
	if (rtos_init() != 0)
	{
		printf("Error\nrtos_init failed\n");
		return (1);
	}
	if (queue_init() != 0)
	{
		printf("Error\nqueue_init failed\n");
		return (1);
	}

	if (rtos_task_create(task_sensor, NULL, 250) == -1 \
		|| rtos_task_create(task_proc, NULL, 500) == -1 \
		|| rtos_task_create(task_logger, NULL, 750) == -1)
	{
		printf("Error\nrtos_task_create failed\n");
		return (1);
	}
	rtos_start();
	return (0);
}