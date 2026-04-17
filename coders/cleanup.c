#include "codexion.h"

void	destroy_coders(t_sim *sim)
{
	int	i;

	if (sim == NULL || sim->coders == NULL)
		return ;
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_destroy(&sim->coders[i].mutex);
		i++;
	}
	free(sim->coders);
	sim->coders = NULL;
}

void	destroy_dongles(t_sim *sim)
{
	int	i;

	if (sim == NULL || sim->dongles == NULL)
		return ;
	i = 0;
	while (i < sim->number_of_coders)
	{
		heap_destroy(&sim->dongles[i].waiting_queue);
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
	free(sim->dongles);
	sim->dongles = NULL;
}

void	cleanup_sim(t_sim *sim)
{
	if (sim == NULL)
		return ;
	destroy_coders(sim);
	destroy_dongles(sim);
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_mutex_destroy(&sim->seq_mutex);
}