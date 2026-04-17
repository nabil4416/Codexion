#include "codexion.h"

static void	remove_top_request(t_dongle *dongle, t_sim *sim)
{
	t_request	tmp;

	if (dongle == NULL || sim == NULL)
		return ;
	if (!heap_is_empty(&dongle->waiting_queue))
		heap_pop(&dongle->waiting_queue, &tmp, sim);
}

static int	is_request_owner_on_top(t_dongle *dongle, t_coder *coder)
{
	t_request	top;

	if (dongle == NULL || coder == NULL)
		return (FALSE);
	if (heap_is_empty(&dongle->waiting_queue))
		return (FALSE);
	top = heap_peek(&dongle->waiting_queue);
	return (top.coder_id == coder->id);
}

static int	register_request_if_needed(t_coder *coder, t_dongle *dongle,
		int *registered)
{
	t_request	req;

	if (*registered == TRUE)
		return (SUCCESS);
	req.coder_id = coder->id;
	req.request_time_ms = now_ms(coder->sim);
	req.deadline_ms = get_request_deadline(coder);
	req.seq_num = next_request_seq(coder->sim);
	if (heap_push(&dongle->waiting_queue, req, coder->sim) == ERROR)
		return (ERROR);
	*registered = TRUE;
	return (SUCCESS);
}

int	can_take_dongle(t_coder *coder, t_dongle *dongle)
{
	long	now;

	if (coder == NULL || dongle == NULL)
		return (FALSE);
	now = now_ms(coder->sim);
	if (dongle->is_taken == TRUE)
		return (FALSE);
	if (dongle->cooldown_until_ms > now)
		return (FALSE);
	if (!is_request_owner_on_top(dongle, coder))
		return (FALSE);
	return (TRUE);
}

int	try_take_dongle(t_coder *coder, t_dongle *dongle)
{
	if (coder == NULL || dongle == NULL)
		return (ERROR);
	if (!can_take_dongle(coder, dongle))
		return (ERROR);
	dongle->is_taken = TRUE;
	dongle->holder_id = coder->id;
	remove_top_request(dongle, coder->sim);
	log_taken_dongle(coder);
	return (SUCCESS);
}

static int	wait_and_take_dongle(t_coder *coder, t_dongle *dongle)
{
	int	registered;

	if (coder == NULL || dongle == NULL)
		return (ERROR);
	registered = FALSE;
	while (!is_stopped(coder->sim))
	{
		pthread_mutex_lock(&dongle->mutex);
		if (register_request_if_needed(coder, dongle, &registered) == ERROR)
		{
			pthread_mutex_unlock(&dongle->mutex);
			return (ERROR);
		}
		if (try_take_dongle(coder, dongle) == SUCCESS)
		{
			pthread_mutex_unlock(&dongle->mutex);
			return (SUCCESS);
		}
		pthread_mutex_unlock(&dongle->mutex);
		usleep(500);
	}
	return (ERROR);
}

static void	unlock_if_same_order(t_dongle *first, t_dongle *second)
{
	(void)first;
	(void)second;
}

int	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder == NULL || coder->sim == NULL)
		return (ERROR);
	first = coder->left_dongle;
	second = coder->right_dongle;
	if (first == NULL || second == NULL)
		return (ERROR);
	if (first->id > second->id)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (wait_and_take_dongle(coder, first) == ERROR)
		return (ERROR);
	if (first == second)
		return (SUCCESS);
	if (wait_and_take_dongle(coder, second) == ERROR)
	{
		pthread_mutex_lock(&first->mutex);
		first->is_taken = FALSE;
		first->holder_id = 0;
		first->cooldown_until_ms = now_ms(coder->sim)
			+ coder->sim->dongle_cooldown;
		pthread_mutex_unlock(&first->mutex);
		pthread_cond_broadcast(&first->cond);
		return (ERROR);
	}
	unlock_if_same_order(first, second);
	return (SUCCESS);
}

static void	release_one_dongle(t_coder *coder, t_dongle *dongle)
{
	if (coder == NULL || dongle == NULL)
		return ;
	pthread_mutex_lock(&dongle->mutex);
	if (dongle->holder_id == coder->id)
	{
		dongle->is_taken = FALSE;
		dongle->holder_id = 0;
		dongle->cooldown_until_ms = now_ms(coder->sim)
			+ coder->sim->dongle_cooldown;
	}
	pthread_mutex_unlock(&dongle->mutex);
	pthread_cond_broadcast(&dongle->cond);
}

void	release_dongles(t_coder *coder)
{
	if (coder == NULL)
		return ;
	release_one_dongle(coder, coder->left_dongle);
	if (coder->right_dongle != coder->left_dongle)
		release_one_dongle(coder, coder->right_dongle);
}