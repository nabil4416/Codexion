#include "codexion.h"

static void	log_message(t_coder *coder, const char *message)
{
	t_sim	*sim;
	long	timestamp;

	if (coder == NULL || message == NULL)
		return ;
	sim = coder->sim;
	if (sim == NULL)
		return ;
	pthread_mutex_lock(&sim->log_mutex);
	if (!is_stopped(sim))
	{
		timestamp = now_ms(sim);
		printf("%ld %d %s\n", timestamp, coder->id, message);
	}
	pthread_mutex_unlock(&sim->log_mutex);
}

void	log_taken_dongle(t_coder *coder)
{
	log_message(coder, "has taken a dongle");
}

void	log_compiling(t_coder *coder)
{
	log_message(coder, "is compiling");
}

void	log_debugging(t_coder *coder)
{
	log_message(coder, "is debugging");
}

void	log_refactoring(t_coder *coder)
{
	log_message(coder, "is refactoring");
}

void	log_burnout(t_coder *coder)
{
	t_sim	*sim;
	long	timestamp;

	if (coder == NULL || coder->sim == NULL)
		return ;
	sim = coder->sim;
	pthread_mutex_lock(&sim->log_mutex);
	timestamp = now_ms(sim);
	printf("%ld %d burned out\n", timestamp, coder->id);
	pthread_mutex_unlock(&sim->log_mutex);
}