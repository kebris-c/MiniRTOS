/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtos.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 18:47:44 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/27 02:50:54 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtos.h"

/*==============================================================================
	INTERNAL STATE (extern globals)
==============================================================================*/
//	extern globals
t_tcb		*g_curr_task;
t_tcb		g_task_list[MAX_TASKS];
int			g_task_blocked_on[MAX_TASKS];
int			g_num_tasks;
//	static globals
static uint8_t	g_yield_requested = 0;

/*==============================================================================
	INITIALIZATION
==============================================================================*/
/*
 * rtos_init():
 *	Initializes the RTOS internal structures.
 *	- Clears task list and task states.
 *	- Resets blocked-on-queue tracking.
 *	This must be called before creating tasks or starting the scheduler.
 */
int	rtos_init(void)
{
	int	i;
	ft_memset(g_task_list, 0, sizeof(g_task_list));
	g_curr_task = NULL;
	g_num_tasks = 0;
	i = 0;
	while (i < MAX_TASKS)
	{
		g_task_blocked_on[i] = -1;
		i++;
	}
	return (0);
}

/*==============================================================================
	TASK MANAGEMENT
==============================================================================*/
/*
 * rtos_task_create():
 *	Adds a new task to the RTOS task list.
 *	- func: pointer to the task function.
 *	- arg: optional argument passed to the task.
 *	- period_ms: task periodicity in milliseconds (0 for one-shot/manual yield).
 *	Returns task ID or -1 if maximum tasks exceeded.
 *	The new task starts as TASK_READY.
 */
int	rtos_task_create(t_task_func func, void *arg, unsigned int period_ms)
{
	t_tcb	*task;

	if (g_num_tasks >= MAX_TASKS)
		return (-1);
	task = &g_task_list[g_num_tasks];
	task->id = g_num_tasks;
	task->pc = 0;
	task->state = TASK_READY;
	task->func = func;
	task->arg = arg;
	task->period_ms = period_ms;
	task->next_run = get_time_ms();
	g_num_tasks++;
	return (task->id);
}

/*==============================================================================
	SCHEDULING
==============================================================================*/
/*
 * rtos_start():
 *	Main RTOS scheduler loop.
 *	- Implements a cooperative scheduler with optional delays.
 *	- Iterates over tasks in round-robin fashion.
 *	- Executes TASK_READY tasks whose next_run has elapsed.
 *	- Handles task yield via rtos_yield.
 *	Notes:
 *		- This is not preemptive.
 *		- Tasks must yield cooperatively if long-running.
 */
void	rtos_start(void)
{
	unsigned long	now;
	t_tcb			*task;
	int				id;

	printf("[RTOS] Starting scheduler with %d tasks\n", g_num_tasks);
	while (1)
	{
		for (id = 0; id < g_num_tasks; id++)
		{
			now = get_time_ms();
			g_yield_requested = 0;
			task = &g_task_list[id];
			if (task->state != TASK_READY)
				continue ;
			if (now < task->next_run)
				continue ;
			g_curr_task = task;
			task->state = TASK_RUNNING;
			task->func(task->arg);
			if (g_yield_requested)
			{
				g_yield_requested = 0;
				continue ;
			}
			task->state = TASK_READY;
			if (task->period_ms > 0)
				task->next_run = now + task->period_ms;
			task->pc = 0;
		}
		g_curr_task = NULL;
		usleep(500);
	}
}

/*
 * rtos_yield():
 *	Causes the current task to voluntarily give up CPU time.
 *	Scheduler will continue to the next eligible task.
 *	Should be called from within a running task.
 */
void	rtos_yield(void)
{
	g_yield_requested = 1;
}

/*
 * rtos_delay():
 *	Delays the current task for the specified milliseconds.
 *	- Updates the task's next_run time.
 *	- Immediately yields control to the scheduler.
 *	Useful to simulate sensor acquisition times or periodic operations.
 */
void	rtos_delay(unsigned int ms)
{
	if (g_curr_task == NULL)
		return ;
	g_curr_task->next_run = get_time_ms() + ms;
	rtos_yield();
}