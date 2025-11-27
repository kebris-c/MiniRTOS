/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:25:20 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/27 02:59:09 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtos.h"

/*==============================================================================
	INTERNAL STATE (extern globals)
==============================================================================*/
t_msg_queue	g_queues[MAX_QUEUES];

/*==============================================================================
	INITIALIZATION
==============================================================================*/
/*
 * queue_init():
 *	Initializes all message queues.
 *	- Sets head, tail, count to 0.
 *	- Assigns capacity and item size.
 *	Notes:
 *		- Must be called before sending or receiving messages.
 */
int	queue_init(void)
{
	int	i;

	i = 0;
	while (i < MAX_QUEUES)
	{
		memset(&g_queues[i], 0, sizeof(t_msg_queue));
		g_queues[i].capacity = CAPACITY;
		g_queues[i].item_size = ITEM_SIZE;
		i++;
	}
	return (0);
}

/*==============================================================================
	QUEUE CONTROL
==============================================================================*/
/*
 * queue_send_msg():
 *	Sends a message to the specified queue.
 *	- Overwrites oldest data if queue is full.
 *	- Unblocks a task waiting on this queue if any.
 *	- Works with fixed-size items (ITEM_SIZE bytes).
 *	Notes:
 *		- Uses uint8_t buffer for generic storage.
 */
int	queue_send_msg(int queue_id, const void *data, size_t size)
{
	t_msg_queue	*q;
	uint8_t		*dest;
	int			i;

	if (queue_id < 0 || queue_id >= MAX_QUEUES \
			|| !data || size == 0 || size > ITEM_SIZE)
		return (-1);
	q = &g_queues[queue_id];
	if (q->count >= q->capacity)
	{
		q->head = (q->head + 1) % q->capacity;
		q->count = q->capacity - 1;
	}
	dest = q->buffer + (q->tail * q->item_size);
	if (size > q->item_size)
		size = q->item_size;
	ft_memcpy(dest, data, size);
	q->tail = (q->tail + 1) % q->capacity;
	q->count++;
	i = 0;
	while (i < g_num_tasks)
	{
		if (g_task_blocked_on[i] == queue_id)
		{
			g_task_list[i].state = TASK_READY;
			g_task_blocked_on[i] = -1;
			break ;
		}
		i++;
	}
	return (0);
}

/*
 * queue_recv_msg():
 *	Receives a message from the specified queue.
 *	- If queue is empty, current task is blocked until a message arrives.
 *	- Returns -1 immediately if no message available (task should yield).
 *	Notes:
 *		- Works with fixed-size items (ITEM_SIZE bytes).
 *		- Handles cooperative blocking via TASK_BLOCKED state.
 */
int	queue_recv_msg(int queue_id, void *buffer, size_t size)
{
	t_msg_queue	*q;
	uint8_t		*src;

	if (queue_id < 0 || queue_id >= MAX_QUEUES \
			|| !buffer || size == 0 || size > ITEM_SIZE)
		return (-1);
	q = &g_queues[queue_id];
	if (q->count == 0)
	{
		if (g_curr_task != NULL)
		{
			g_curr_task->state = TASK_BLOCKED;
			g_task_blocked_on[g_curr_task->id] = queue_id;
		}
		return (-1);
	}
	src = q->buffer + (q->head * q->item_size);
	if (size > q->item_size)
		size = q->item_size;
	ft_memcpy(buffer, src, size);
	q->head = (q->head + 1) % q->capacity;
	q->count--;
	return (0);
}