#include "codexion.h"

static void	swap_requests(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	heap_init(t_heap *heap, int capacity)
{
	if (heap == NULL || capacity <= 0)
		return (ERROR);
	heap->data = (t_request *)malloc(sizeof(t_request) * capacity);
	if (heap->data == NULL)
		return (ERROR);
	heap->size = 0;
	heap->capacity = capacity;
	return (SUCCESS);
}

void	heap_destroy(t_heap *heap)
{
	if (heap == NULL)
		return ;
	free(heap->data);
	heap->data = NULL;
	heap->size = 0;
	heap->capacity = 0;
}

static void	heapify_up(t_heap *heap, int index, t_sim *sim)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (request_cmp(sim, heap->data[index], heap->data[parent]) >= 0)
			break ;
		swap_requests(&heap->data[index], &heap->data[parent]);
		index = parent;
	}
}

static void	heapify_down(t_heap *heap, int index, t_sim *sim)
{
	int	left;
	int	right;
	int	smallest;

	while (TRUE)
	{
		left = (index * 2) + 1;
		right = (index * 2) + 2;
		smallest = index;
		if (left < heap->size
			&& request_cmp(sim, heap->data[left], heap->data[smallest]) < 0)
			smallest = left;
		if (right < heap->size
			&& request_cmp(sim, heap->data[right], heap->data[smallest]) < 0)
			smallest = right;
		if (smallest == index)
			break ;
		swap_requests(&heap->data[index], &heap->data[smallest]);
		index = smallest;
	}
}

int	heap_push(t_heap *heap, t_request req, t_sim *sim)
{
	if (heap == NULL || heap->data == NULL || sim == NULL)
		return (ERROR);
	if (heap->size >= heap->capacity)
		return (ERROR);
	heap->data[heap->size] = req;
	heapify_up(heap, heap->size, sim);
	heap->size++;
	return (SUCCESS);
}

int	heap_pop(t_heap *heap, t_request *out, t_sim *sim)
{
	if (heap == NULL || out == NULL || sim == NULL)
		return (ERROR);
	if (heap->size <= 0)
		return (ERROR);
	*out = heap->data[0];
	heap->size--;
	if (heap->size > 0)
	{
		heap->data[0] = heap->data[heap->size];
		heapify_down(heap, 0, sim);
	}
	return (SUCCESS);
}

t_request	heap_peek(t_heap *heap)
{
	t_request	empty;

	empty.coder_id = 0;
	empty.request_time_ms = 0;
	empty.deadline_ms = 0;
	empty.seq_num = 0;
	if (heap == NULL || heap->size <= 0)
		return (empty);
	return (heap->data[0]);
}

int	heap_is_empty(t_heap *heap)
{
	if (heap == NULL)
		return (TRUE);
	return (heap->size == 0);
}