#include "codexion.h"

int	is_stopped(t_sim *sim)
{
	int	stopped;

	if (sim == NULL)
		return (TRUE);
	pthread_mutex_lock(&sim->stop_mutex);
	stopped = sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (stopped);
}

void	set_stop(t_sim *sim)
{
	if (sim == NULL)
		return ;
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = TRUE;
	pthread_mutex_unlock(&sim->stop_mutex);
}

void	set_burnout_stop(t_sim *sim)
{
	if (sim == NULL)
		return ;
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = TRUE;
	sim->someone_burned_out = TRUE;
	pthread_mutex_unlock(&sim->stop_mutex);
}

void	set_coder_state(t_coder *coder, t_coder_state state)
{
	if (coder == NULL)
		return ;
	pthread_mutex_lock(&coder->mutex);
	coder->state = state;
	pthread_mutex_unlock(&coder->mutex);
}

t_coder_state	get_coder_state(t_coder *coder)
{
	t_coder_state	state;

	if (coder == NULL)
		return (STATE_BURNED_OUT);
	pthread_mutex_lock(&coder->mutex);
	state = coder->state;
	pthread_mutex_unlock(&coder->mutex);
	return (state);
}

long	get_last_compile_start(t_coder *coder)
{
	long	value;

	if (coder == NULL)
		return (0);
	pthread_mutex_lock(&coder->mutex);
	value = coder->last_compile_start_ms;
	pthread_mutex_unlock(&coder->mutex);
	return (value);
}

void	set_last_compile_start(t_coder *coder, long value)
{
	if (coder == NULL)
		return ;
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_start_ms = value;
	pthread_mutex_unlock(&coder->mutex);
}

long	next_request_seq(t_sim *sim)
{
	long	value;

	if (sim == NULL)
		return (0);
	pthread_mutex_lock(&sim->seq_mutex);
	sim->request_seq++;
	value = sim->request_seq;
	pthread_mutex_unlock(&sim->seq_mutex);
	return (value);
}

long	get_request_deadline(t_coder *coder)
{
	t_sim	*sim;
	long	last_start;

	if (coder == NULL || coder->sim == NULL)
		return (0);
	sim = coder->sim;
	last_start = get_last_compile_start(coder);
	return (last_start + sim->time_to_burnout);
}

int	all_coders_done(t_sim *sim)
{
	int	i;
	int	done;

	if (sim == NULL)
		return (FALSE);
	i = 0;
	done = TRUE;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].mutex);
		if (sim->coders[i].compile_count < sim->number_of_compiles_required)
			done = FALSE;
		pthread_mutex_unlock(&sim->coders[i].mutex);
		if (done == FALSE)
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	start_simulation(t_sim *sim)
{
	int	i;

	if (sim == NULL)
		return (ERROR);
	i = 0;
	while (i < sim->number_of_coders)
	{
		set_last_compile_start(&sim->coders[i], 0);
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]) != 0)
			return (print_error("Failed to create coder thread"), ERROR);
		i++;
	}
	if (pthread_create(&sim->monitor_thread, NULL,
			monitor_routine, sim) != 0)
		return (print_error("Failed to create monitor thread"), ERROR);
	return (SUCCESS);
}

int	join_simulation(t_sim *sim)
{
	int	i;

	if (sim == NULL)
		return (ERROR);
	i = 0;
	while (i < sim->number_of_coders)
	{
		if (pthread_join(sim->coders[i].thread, NULL) != 0)
			return (print_error("Failed to join coder thread"), ERROR);
		i++;
	}
	if (pthread_join(sim->monitor_thread, NULL) != 0)
		return (print_error("Failed to join monitor thread"), ERROR);
	return (SUCCESS);
}