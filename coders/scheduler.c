#include "codexion.h"

static int	compare_fifo(t_request a, t_request b)
{
	if (a.seq_num < b.seq_num)
		return (-1);
	if (a.seq_num > b.seq_num)
		return (1);
	if (a.coder_id < b.coder_id)
		return (-1);
	if (a.coder_id > b.coder_id)
		return (1);
	return (0);
}

static int	compare_edf(t_request a, t_request b)
{
	if (a.deadline_ms < b.deadline_ms)
		return (-1);
	if (a.deadline_ms > b.deadline_ms)
		return (1);
	if (a.seq_num < b.seq_num)
		return (-1);
	if (a.seq_num > b.seq_num)
		return (1);
	if (a.coder_id < b.coder_id)
		return (-1);
	if (a.coder_id > b.coder_id)
		return (1);
	return (0);
}

int	request_cmp(t_sim *sim, t_request a, t_request b)
{
	if (sim == NULL)
		return (0);
	if (sim->scheduler == SCHED_TYPE_FIFO)
		return (compare_fifo(a, b));
	return (compare_edf(a, b));
}