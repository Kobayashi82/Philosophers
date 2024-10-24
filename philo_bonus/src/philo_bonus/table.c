/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 23:48:19 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/07 13:35:56 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

//	Print the end result of the dinner
static void	ending(t_table *table, int status)
{
	if (status == -1)
		return ;
	if (status)
		printf(C"\n\tA philosopher "RD"died\n\n"NC);
	else
	{
		printf(C"\n\tEach philosopher has eaten "G"%ld", table->t_meals);
		if (table->t_meals > 1)
			printf(C" times\n\n"NC);
		else
			printf(C" time\n\n"NC);
	}
}

//	Cleans up resources allocated for the table
void	clean_table(t_table *table, int error)
{
	int	i;
	int	status;

	i = -1;
	status = -1;
	while (error > 13 && ++i < table->t_philos)
	{
		waitpid(-1, &status, 0);
		if (status)
		{
			i = -1;
			while (++i < table->t_philos)
				kill(table->pid[i], SIGKILL);
			break ;
		}
	}
	if (error > 13)
	{
		sem_close(table->sm_waiter);
		sem_close(table->sm_forks);
		sem_close(table->sm_table);
		sem_unlink("/sm_waiter");
		sem_unlink("/sm_forks");
		sem_unlink("/sm_table");
	}
	if (table->pid)
		free(table->pid);
	ending(table, status);
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

//	Initializes the table with parameters from command-line arguments
void	set_table(t_table *table, int argc, char **argv)
{
	table->pid = NULL;
	if (argc != 5 && argc != 6)
		exit_error(table, ARG_BAD);
	table->t_philos = ft_atol(table, argv[1], 0);
	if (table->t_philos > MAX_PHILOS)
		exit_error(table, ARG_MAX_PHILOS);
	table->t_die = ft_atol(table, argv[2], 0);
	table->t_eat = ft_atol(table, argv[3], 0);
	table->t_sleep = ft_atol(table, argv[4], 0);
	table->t_meals = -1;
	if (argc == 6)
		table->t_meals = ft_atol(table, argv[5], 1);
	table->dinner_end = 0;
	table->pid = malloc(table->t_philos * sizeof(int));
	if (!table->pid)
		exit_error(table, NO_MEMORY);
	sem_unlink("/sm_waiter");
	sem_unlink("/sm_forks");
	sem_unlink("/sm_table");
	table->sm_waiter = sem_open("/sm_waiter", O_CREAT, 0644, 1);
	table->sm_forks = sem_open("/sm_forks", O_CREAT, 0644, table->t_philos);
	table->sm_table = sem_open("/sm_table", O_CREAT, 0644, 1);
	if (table->sm_waiter == SEM_FAILED || table->sm_forks == SEM_FAILED
		|| table->sm_table == SEM_FAILED)
		exit_error(table, SM_FAILED);
}
