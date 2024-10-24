/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 23:48:15 by vzurera-          #+#    #+#             */
/*   Updated: 2024/02/25 15:19:48 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

//	Prints error messages related to command-line arguments
static void	error_args(int error)
{
	if (error == ARG_BAD)
	{
		printf(Y"\n\tERROR: "C"wrong number of arguments\n\n");
		printf(Y"\t./philo " C"NP TD TE TS " M"NM\n\n");
		printf(G"\tNP: " C"Number of philosofers\n");
		printf(G"\tTD: " C"Time to die\n");
		printf(G"\tTE: " C"Time to eat\n");
		printf(G"\tTS: " C"Time to sleep\n");
		printf(G"\tNM: " M"Number of meals (optional)\n\n");
	}
	if (error == ARG_NDIG)
		printf(Y"\n\tERROR: "C"input is not a correct digit\n\n"NC);
	if (error == ARG_NEG)
		printf(Y"\n\tERROR: "C"input can't be zero or negative\n\n"NC);
	if (error == ARG_MINT)
		printf(Y"\n\tERROR: "C"input is greater than "
			UN C"INT_MAX\n\n"NC);
	if (error == ARG_TIMES)
		printf(Y"\n\tERROR: "C"times for die, eat and sleep must "
			"be equal or shorter than 200 ms\n\n"NC);
	if (error == ARG_MAX_PHILOS)
		printf(Y"\n\tERROR: "C"The maximum number of philosophers must be"
			" equal or lower than 200\n\n"NC);
}

//	Prints error messages and cleans up resources before exiting
void	exit_error(t_table *table, int error)
{
	if (error == NO_MEMORY)
		printf(Y"\n\tERROR: "C"No memory left in the device\n\n"NC);
	if (error == SM_FAILED)
		printf(Y"\n\tERROR: "C"Semaphore creation failed\n\n"NC);
	error_args(error);
	clean_table(table, error);
	exit(1);
}

//	Retrieves the current time in different modes
long	get_time(int mode)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	if (mode == SEC)
		return (time.tv_sec + time.tv_usec / 1000000);
	if (mode == MLSEC)
		return (time.tv_sec * 1000 + time.tv_usec / 1000);
	if (mode == MCRSEC)
		return (time.tv_sec * 1000000 + time.tv_usec);
	return (0);
}

//	Delays execution for a specified amount of time
void	wait_time(t_table *table, long time)
{
	long	t_time;
	int		dinner_end;

	t_time = get_time(MLSEC);
	sem_wait(table->sm_waiter);
	dinner_end = table->dinner_end;
	sem_post(table->sm_waiter);
	while (!dinner_end && get_time(MLSEC) - t_time < time)
		usleep(500);
}

//	Entry point of the program
int	main(int argc, char **argv)
{
	t_table	table;
	int		i;

	i = -1;
	set_table(&table, argc, argv);
	table.dinner_start = get_time(MLSEC);
	while (table.t_philos > 0 && table.t_meals && ++i < table.t_philos)
	{
		table.pid[i] = fork();
		if (table.pid[i] == -1)
			exit_error(&table, NO_FORK);
		if (table.pid[i] == 0)
		{
			if (table.pid)
				free(table.pid);
			table.pid = NULL;
			table.id = i + 1;
			table.last_meal = get_time(MLSEC);
			table.curr_meal = 0;
			table.died = 0;
			dinner(&table);
		}
	}
	clean_table(&table, END);
}
