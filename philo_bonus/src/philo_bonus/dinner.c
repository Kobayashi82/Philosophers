/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 23:48:01 by vzurera-          #+#    #+#             */
/*   Updated: 2024/02/25 16:31:34 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

//	Print the current action of a philosopher
static void	waiter(t_table *table, int status)
{
	long	time;

	sem_wait(table->sm_waiter);
	time = get_time(MLSEC) - table->dinner_start;
	if (!table->dinner_end)
	{
		if (status == FORK_A || status == FORK_B)
			printf(W"%ld\t"W" %3d   has taken a fork\n"NC, time, table->id);
		else if (status == EATING)
			printf(G"%ld\t"G" %3d   is eating\n"NC, time, table->id);
		else if (status == SLEEPING)
			printf(C"%ld\t"C" %3d   is sleeping\n"NC, time, table->id);
		else if (status == THINKING)
			printf(Y"%ld\t"Y" %3d   is thinking\n"NC, time, table->id);
		else if (status == DIED)
			printf(RD"%ld\t %3d   died\n"NC, time, table->id);
	}
	sem_post(table->sm_waiter);
}

//	Controls the dining process and checks for philosopher's health
static int	maitre_notes(t_table *table)
{
	if (get_time(MLSEC) - table->last_meal > table->t_die)
	{
		table->died = 1;
		waiter(table, DIED);
		sem_wait(table->sm_waiter);
		table->dinner_end = 1;
		sem_post(table->sm_waiter);
		sem_post(table->sm_table);
		return (1);
	}
	if (table->t_meals != -1 && table->curr_meal >= table->t_meals)
	{
		sem_wait(table->sm_waiter);
		table->dinner_end = 1;
		sem_post(table->sm_waiter);
		sem_post(table->sm_table);
		return (1);
	}
	return (0);
}

//	Controls the dining process and checks for philosopher's health
static void	*maitre(void *vtable)
{
	t_table	*table;
	int		dinner_end;

	table = (t_table *) vtable;
	sem_wait(table->sm_waiter);
	dinner_end = table->dinner_end;
	sem_post(table->sm_waiter);
	while (!dinner_end)
	{
		usleep(100);
		sem_wait(table->sm_table);
		if (maitre_notes(table))
			break ;
		sem_post(table->sm_table);
	}
	if (table->died)
		exit (1);
	exit (0);
}

//	Initiates and manages the dining process for philosophers
void	dinner(t_table *table)
{
	if (pthread_create(&table->maitre, NULL, &maitre, table))
		exit_error(table, THRD_CREATE);
	if (table->id % 2 == 1)
		usleep(1000);
	while (1)
	{
		waiter(table, THINKING);
		sem_wait(table->sm_forks);
		waiter(table, FORK_A);
		sem_wait(table->sm_forks);
		waiter(table, FORK_B);
		waiter(table, EATING);
		wait_time(table, table->t_eat);
		sem_wait(table->sm_table);
		table->last_meal = get_time(MLSEC);
		table->curr_meal += 1;
		sem_post(table->sm_table);
		sem_post(table->sm_forks);
		sem_post(table->sm_forks);
		waiter(table, SLEEPING);
		wait_time(table, table->t_sleep);
	}
	if (pthread_join(table->maitre, NULL))
		exit_error(table, THRD_JOIN);
}
