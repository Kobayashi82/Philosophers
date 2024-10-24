/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 21:26:59 by vzurera-          #+#    #+#             */
/*   Updated: 2024/02/18 19:49:09 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//	Cleans up resources allocated for the table
void	clean_table(t_table *table, int error)
{
	int		i;

	i = -1;
	if (error > 13)
	{
		while (++i < table->t_philos)
			pthread_mutex_destroy(&table->philos[i].mtx_philo);
		pthread_mutex_destroy(&table->mtx_table);
		pthread_mutex_destroy(&table->mtx_waiter);
	}
	if (table->forks)
		free(table->forks);
	if (table->philos)
		free(table->philos);
}

//	Converts a string to a long integer
static long	ft_atol(t_table *table, char *str, int is_meal)
{
	long	n;

	n = 0;
	if (str == NULL)
		return (0);
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '-' && *(str + 1) == '0')
		str++;
	if (*str == '-')
		exit_error(table, ARG_NEG);
	if (*str == '+')
		str++;
	if (!(*str >= '0' && *str <= '9'))
		exit_error(table, ARG_NDIG);
	while (*str >= '0' && *str <= '9')
	{
		n = (n * 10) + (*str - '0');
		if (n > INT_MAX)
			exit_error(table, ARG_MINT);
		str++;
	}
	if (n == 0 && is_meal == 0)
		exit_error(table, ARG_NEG);
	return (n);
}

//	Assigns forks to a philosopher
static void	set_forks(t_philo *philo)
{
	if (philo->id % 2)
	{
		philo->fork_a = &philo->table->forks[philo->id - 1];
		if (philo->id == philo->table->t_philos)
			philo->fork_b = &philo->table->forks[0];
		else
			philo->fork_b = &philo->table->forks[philo->id];
	}
	else
	{
		if (philo->id == philo->table->t_philos)
			philo->fork_a = &philo->table->forks[0];
		else
			philo->fork_a = &philo->table->forks[philo->id];
		philo->fork_b = &philo->table->forks[philo->id - 1];
	}
}

//	Initializes philosopher structs in the table
static void	set_philos(t_table *table)
{
	int		i;

	i = -1;
	table->dinner_end = 0;
	table->ready = 0;
	table->th = 0;
	table->philos = malloc(table->t_philos * sizeof(t_philo));
	table->forks = malloc(table->t_philos * sizeof(t_mtx));
	if (!table->philos || !table->forks)
		exit_error(table, NO_MEMORY);
	mtx_set(table, &table->mtx_table, MTX_INIT);
	mtx_set(table, &table->mtx_waiter, MTX_INIT);
	while (++i < table->t_philos)
		mtx_set(table, &table->forks[i], MTX_INIT);
	i = -1;
	while (++i < table->t_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].meal_done = 0;
		table->philos[i].curr_meal = 0;
		mtx_set(table, &table->philos[i].mtx_philo, MTX_INIT);
		table->philos[i].table = table;
		set_forks(&table->philos[i]);
	}
}

//	Initializes the table with parameters from command-line arguments
void	set_table(t_table *table, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		exit_error(table, ARG_BAD);
	table->t_philos = ft_atol(table, argv[1], 0);
	if (table->t_philos > MAX_PHILOS)
		exit_error(table, ARG_MAX_PHILOS);
	table->t_die = ft_atol(table, argv[2], 0) * 1000;
	table->t_eat = ft_atol(table, argv[3], 0) * 1000;
	table->t_sleep = ft_atol(table, argv[4], 0) * 1000;
	table->t_meals = -1;
	if (argc == 6)
		table->t_meals = ft_atol(table, argv[5], 1);
	table->dinner_end = 0;
	set_philos(table);
}
