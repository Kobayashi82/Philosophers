/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 21:26:39 by vzurera-          #+#    #+#             */
/*   Updated: 2024/06/13 12:41:37 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//	Print the current action of a philosopher
static void	waiter(t_philo *philo, int status)
{
	long	time;
	int		dinner_end;
	t_table	*table;

	table = philo->table;
	if (get_int(table, &philo->mtx_philo, &philo->meal_done))
		return ;
	mtx_set(table, &table->mtx_waiter, MTX_LOCK);
	time = get_time(MLSEC) - table->dinner_start;
	dinner_end = get_int(table, &table->mtx_table, &table->dinner_end);
	if ((status == FORK_A || status == FORK_B) && !dinner_end)
		printf(W"%ld\t"W" %3d   has taken a fork\n"NC, time, philo->id);
	else if (status == EATING && !dinner_end)
		printf(G"%ld\t"G" %3d   is eating\n"NC, time, philo->id);
	else if (status == SLEEPING && !dinner_end)
		printf(C"%ld\t"C" %3d   is sleeping\n"NC, time, philo->id);
	else if (status == THINKING && !dinner_end)
		printf(Y"%ld\t"Y" %3d   is thinking\n"NC, time, philo->id);
	else if (status == DIED)
		printf(RD"%ld\t %3d   died\n"NC, time, philo->id);
	mtx_set(philo->table, &table->mtx_waiter, MTX_UNLOCK);
}

//	Controls the dining process and checks for philosopher's health
static void	*maitre(void *vtable)
{
	int		i;
	int		dinner_end;
	t_table	*table;

	table = (t_table *) vtable;
	while (!(get_long(table, &table->mtx_table, &table->th) == table->t_philos))
		;
	dinner_end = get_int(table, &table->mtx_table, &table->dinner_end);
	while (!get_int(table, &table->mtx_table, &table->dinner_end))
	{
		i = -1;
		while (++i < table->t_philos && !dinner_end)
		{
			dinner_end = get_int(table, &table->mtx_table, &table->dinner_end);
			if (!get_int(table, &table->philos[i].mtx_philo,
					&table->philos[i].meal_done) && (get_time(MLSEC) \
				- get_long(table, &table->philos[i].mtx_philo,
						&table->philos[i].last_meal) > table->t_die / 1000))
			{
				set_int(table, &table->mtx_table, &table->dinner_end, 1);
				waiter(&table->philos[i], DIED);
			}
		}
	}
	return (0);
}

//	Implements the eating process for a philosopher
static void	eat(t_philo *philo)
{
	t_table	*table;

	table = philo->table;
	mtx_set(table, philo->fork_a, MTX_LOCK);
	waiter(philo, FORK_A);
	if (philo->table->t_philos == 1)
	{
		while (!get_int(table, &philo->table->mtx_table, &table->dinner_end))
			;
		mtx_set(table, philo->fork_a, MTX_UNLOCK);
		return ;
	}
	mtx_set(table, philo->fork_b, MTX_LOCK);
	waiter(philo, FORK_B);
	set_long(table, &philo->mtx_philo, &philo->last_meal, get_time(MLSEC));
	philo->curr_meal++;
	waiter(philo, EATING);
	wait_time(table, table->t_eat);
	if (table->t_meals > 0 && philo->curr_meal == table->t_meals)
		set_int(table, &philo->mtx_philo, &philo->meal_done, 1);
	mtx_set(table, philo->fork_a, MTX_UNLOCK);
	mtx_set(table, philo->fork_b, MTX_UNLOCK);
}

//	Manages the dining process for philosophers
static void	*dining(void *vphilo)
{
	t_philo	*philo;
	t_table	*table;

	philo = (t_philo *) vphilo;
	table = philo->table;
	while (!get_int(table, &table->mtx_table, &table->ready))
		;
	set_long(table, &philo->mtx_philo, &philo->last_meal, get_time(MLSEC));
	set_long(table, &table->mtx_table, &table->th, 0);
	if ((table->t_philos % 2 == 0) && (philo->id % 2 == 0))
		wait_time(table, 30000);
	while (!get_int(table, &table->mtx_table, &table->dinner_end)
		&& !get_int(table, &philo->mtx_philo, &philo->meal_done))
	{
		eat(philo);
		waiter(philo, SLEEPING);
		wait_time(table, table->t_sleep);
		waiter(philo, THINKING);
		wait_time(table, (((table->t_philos % 2) * table->t_eat) * 2) \
			- table->t_sleep * 0.40);
	}
	return (0);
}

//	Initiates the dinner process with the given table configuration
void	dinner(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->t_philos)
		if (pthread_create(&table->philos[i].thread, NULL, \
			dining, &table->philos[i]))
			exit_error(table, THRD_CREATE);
	if (pthread_create(&table->maitre, NULL, maitre, table))
		exit_error(table, THRD_CREATE);
	table->dinner_start = get_time(MLSEC);
	set_int(table, &table->mtx_table, &table->ready, 1);
	i = -1;
	while (++i < table->t_philos)
		if (pthread_join(table->philos[i].thread, NULL))
			exit_error(table, THRD_JOIN);
	set_int(table, &table->mtx_table, &table->dinner_end, 1);
	if (pthread_join(table->maitre, NULL))
		exit_error(table, THRD_JOIN);
}
