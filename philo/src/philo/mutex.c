/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:19:24 by vzurera-          #+#    #+#             */
/*   Updated: 2024/02/18 19:47:52 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//	Initializes, locks, unlocks, or destroys a mutex based on the action
void	mtx_set(t_table *table, t_mtx *mutex, int action)
{
	if (action == MTX_INIT && pthread_mutex_init(mutex, NULL))
		exit_error(table, MTX_INIT);
	if (action == MTX_LOCK && pthread_mutex_lock(mutex))
		exit_error(table, MTX_LOCK);
	if (action == MTX_UNLOCK && pthread_mutex_unlock(mutex))
		exit_error(table, MTX_UNLOCK);
	if (action == MTX_DESTROY && pthread_mutex_destroy(mutex))
		exit_error(table, MTX_DESTROY);
}

//	Sets the value of an integer protected by a mutex
void	set_int(t_table *table, t_mtx *mutex, int *value1, int value2)
{
	mtx_set(table, mutex, MTX_LOCK);
	*value1 = value2;
	mtx_set(table, mutex, MTX_UNLOCK);
}

//	Retrieves the value of an integer protected by a mutex
int	get_int(t_table *table, t_mtx *mutex, int *value1)
{
	int	result;

	mtx_set(table, mutex, MTX_LOCK);
	result = *value1;
	mtx_set(table, mutex, MTX_UNLOCK);
	return (result);
}

//	Retrieves the value of a long integer protected by a mutex
long	get_long(t_table *table, t_mtx *mutex, long *value1)
{
	long	result;

	mtx_set(table, mutex, MTX_LOCK);
	result = *value1;
	mtx_set(table, mutex, MTX_UNLOCK);
	return (result);
}

//	Sets the value of a long integer protected by a mutex
void	set_long(t_table *table, t_mtx *mutex, long *value1, long value2)
{
	mtx_set(table, mutex, MTX_LOCK);
	if (!value2)
		*value1 += 1;
	else
		*value1 = value2;
	mtx_set(table, mutex, MTX_UNLOCK);
}
