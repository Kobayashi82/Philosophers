/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 21:26:53 by vzurera-          #+#    #+#             */
/*   Updated: 2024/06/13 12:38:28 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	if (error == THRD_CREATE)
		printf(Y"\n\tERROR: "C"Thread creation failed\n\n"NC);
	if (error == THRD_DETACH)
		printf(Y"\n\tERROR: "C"Thread detachment failed\n\n"NC);
	if (error == THRD_JOIN)
		printf(Y"\n\tERROR: "C"Thread join failed\n\n"NC);
	if (error == MTX_INIT)
		printf(Y"\n\tERROR: "C"Mutex initialization failed\n\n"NC);
	if (error == MTX_LOCK)
		printf(Y"\n\tERROR: "C"Mutex lock failed\n\n"NC);
	if (error == MTX_UNLOCK)
		printf(Y"\n\tERROR: "C"Mutex unlock failed\n\n"NC);
	if (error == MTX_DESTROY)
		printf(Y"\n\tERROR: "C"Mutex destruction failed\n\n"NC);
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
	long	start;
	long	remaining;

	if (time < 0)
		return ;
	start = get_time(MCRSEC);
	while (get_time(MCRSEC) - start < time)
	{
		if (get_int(table, &table->mtx_table, &table->dinner_end))
			break ;
		remaining = time - (get_time(MCRSEC) - start);
		if (remaining > 10000)
			usleep(remaining / 2);
		else
			while (get_time(MCRSEC) - start < time)
				;
	}
}

//	Entry point of the program
int	main(int argc, char **argv)
{
	t_table	table;
	int		i;

	i = -1;
	table.philos = NULL;
	table.forks = NULL;
	set_table(&table, argc, argv);
	if (table.t_philos > 0 && table.t_meals)
	{
		dinner(&table);
		while (++i < table.t_philos
			&& table.philos[i].curr_meal == table.t_meals)
			;
		if (i != table.t_philos)
			printf(C"\n\tA philosopher "RD"died\n\n"NC);
		else
		{
			printf(C"\n\tEach philosopher has eaten "G"%ld", table.t_meals);
			if (table.t_meals > 1)
				printf(C" times\n\n"NC);
			else
				printf(C" time\n\n"NC);
		}
	}
	clean_table(&table, END);
}
