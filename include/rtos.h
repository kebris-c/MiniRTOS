/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtos.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 18:46:17 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/27 01:58:41 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTOS_H
# define RTOS_H

/*==============================================================================
		HEADERS
==============================================================================*/
# include "libft/include/libft.h"
# include "banner/include/banner.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdint.h>
# include <string.h>

/*==============================================================================
		DEFINES
==============================================================================*/
# define MAX_TASKS	3

# define MAX_QUEUES	3
# define CAPACITY	6
# define ITEM_SIZE	2
# define QUEUE_SENSOR 0
# define QUEUE_PROC 1
# define QUEUE_LOGGER 2

# define THRESH_CRITICAL 85
# define THRESH_HIGH 70
# define THRESH_MID 50
# define THRESH_LOW 30

/*==============================================================================
		TYPEDEFS
==============================================================================*/
typedef void	(*t_task_func)(void *);

typedef enum e_task_state
{
	TASK_READY,
	TASK_RUNNING,
	TASK_BLOCKED
}	t_task_state;

typedef struct s_tcb
{
	int				id;
	int				pc;
	t_task_state	state;
	t_task_func		func;
	void			*arg;
	unsigned int	period_ms;
	unsigned long	next_run;
}	t_tcb;

typedef struct s_msg_queue
{
	uint8_t	buffer[CAPACITY * ITEM_SIZE];
	int		head;
	int		tail;
	int		count;
	int		capacity;
	size_t	item_size;
}	t_msg_queue;

typedef struct timeval t_timeval;

/*==============================================================================
		EXTERNS GLOBALS
==============================================================================*/
extern t_msg_queue	g_queues[MAX_QUEUES];

extern t_tcb		*g_curr_task;
extern t_tcb		g_task_list[MAX_TASKS];
extern int			g_task_blocked_on[MAX_TASKS];
extern int			g_num_tasks;

/*==============================================================================
		PROTOTYPES
==============================================================================*/
//	rtos.c
int				rtos_init(void);
int				rtos_task_create(t_task_func func, void *arg, unsigned int period_ms);
void			rtos_start(void);
void			rtos_delay(unsigned int ms);
void			rtos_yield(void);
//	tasks.c
void			task_sensor(void *arg);
void			task_proc(void *arg);
void			task_logger(void *arg);
//	queue.c
int				queue_init(void);
int				queue_send_msg(int queue_id, const void *data, size_t size);
int				queue_recv_msg(int queue_id, void *buffer, size_t size);
//	drivers.c
int				driver_adc_read(void);
void			driver_uart_send(const void *buffer, size_t len);
void			driver_fan_set(int fan_id, int speed_percent);
//	utils.c
unsigned long	get_time_ms(void);

#endif