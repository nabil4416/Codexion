#include "codexion.h"

int	coder_compile(t_coder *coder)
{
	if (coder == NULL || coder->sim == NULL)
		return (ERROR);
	if (take_dongles(coder) == ERROR)
		return (ERROR);
	set_last_compile_start(coder, now_ms(coder->sim));
	set_coder_state(coder, STATE_COMPILING);
	log_compiling(coder);
	smart_sleep(coder->sim, coder->sim->time_to_compile);
	release_dongles(coder);
	if (is_stopped(coder->sim))
		return (ERROR);
	return (SUCCESS);
}

int	coder_debug(t_coder *coder)
{
	if (coder == NULL || coder->sim == NULL)
		return (ERROR);
	set_coder_state(coder, STATE_DEBUGGING);
	log_debugging(coder);
	smart_sleep(coder->sim, coder->sim->time_to_debug);
	if (is_stopped(coder->sim))
		return (ERROR);
	return (SUCCESS);
}

int	coder_refactor(t_coder *coder)
{
	if (coder == NULL || coder->sim == NULL)
		return (ERROR);
	set_coder_state(coder, STATE_REFACTORING);
	log_refactoring(coder);
	smart_sleep(coder->sim, coder->sim->time_to_refactor);
	if (is_stopped(coder->sim))
		return (ERROR);
	return (SUCCESS);
}