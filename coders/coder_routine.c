#include "codexion.h"

static int	coder_has_finished(t_coder *coder)
{
	int	done;

	done = FALSE;
	pthread_mutex_lock(&coder->mutex);
	if (coder->compile_count >= coder->sim->number_of_compiles_required)
		done = TRUE;
	pthread_mutex_unlock(&coder->mutex);
	return (done);
}

static void	handle_single_coder(t_coder *coder)
{
	log_taken_dongle(coder);
	while (!is_stopped(coder->sim))
		usleep(500);
}

static void	increment_compile_count(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->mutex);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder == NULL || coder->sim == NULL)
		return (NULL);
	if (coder->sim->number_of_coders == 1)
	{
		handle_single_coder(coder);
		return (NULL);
	}
	while (!is_stopped(coder->sim))
	{
		if (coder_has_finished(coder))
			break ;
		if (coder_compile(coder) == ERROR)
			break ;
		increment_compile_count(coder);
		if (coder_has_finished(coder))
			break ;
		if (coder_debug(coder) == ERROR)
			break ;
		if (coder_refactor(coder) == ERROR)
			break ;
	}
	return (NULL);
}