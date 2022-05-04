#include "ring_buffer.h"

static void ring_buffer_update_elems_cnt(ring_buffer_t *ring_buffer);

bool ring_buffer_init(ring_buffer_t *ring_buffer, uint8_t *buffer_ptr, uint32_t buff_capacity)
{
    if (ring_buffer == NULL)
        return false;

    ring_buffer->buff_ptr = buffer_ptr;
    ring_buffer->buff_capacity = buff_capacity;

    ring_buffer_deinit(ring_buffer);

    return true;
}

void ring_buffer_deinit(ring_buffer_t *ring_buffer)
{
    ring_buffer->elems_cnt = 0U;
    ring_buffer->head_idx = 0U;
    ring_buffer->tail_idx = 0U;
    ring_buffer->is_empty = true;
}

bool ring_buffer_write(ring_buffer_t *ring_buffer, uint8_t byte)
{
    bool result = true;

    if (result)
    {
        ring_buffer->buff_ptr[ring_buffer->head_idx] = byte;

        if ((ring_buffer->head_idx + 1) % ring_buffer->buff_capacity == 0U)
        {
            ring_buffer->head_idx = 0;
        }
        else
        {
            ring_buffer->head_idx++;
        }

        ring_buffer->is_empty = false;
        ring_buffer_update_elems_cnt(ring_buffer);
    }

    return result;
}

bool ring_buffer_read(ring_buffer_t *ring_buffer, uint8_t *byte)
{
    bool result = true;

    if (ring_buffer->elems_cnt == 0U)
    {
        result = false;
    }

    if (result)
    {
        *byte = ring_buffer->buff_ptr[ring_buffer->tail_idx];

        if ((ring_buffer->tail_idx + 1) % ring_buffer->buff_capacity == 0U)
        {
            ring_buffer->tail_idx = 0;
        }
        else
        {
            ring_buffer->tail_idx++;
        }

        if (!ring_buffer->is_empty)
        {
            if (ring_buffer->tail_idx == ring_buffer->head_idx)
            {
                ring_buffer->is_empty = true;
            }
        }

        ring_buffer_update_elems_cnt(ring_buffer);
    }

    return result;
}

uint32_t ring_buffer_get_elems_cnt(ring_buffer_t *ring_buffer)
{
    return ring_buffer->elems_cnt;
}

static void ring_buffer_update_elems_cnt(ring_buffer_t *ring_buffer)
{
    if (ring_buffer->head_idx > ring_buffer->tail_idx)
    {
        ring_buffer->elems_cnt = ring_buffer->head_idx - ring_buffer->tail_idx;
    }
    else if (ring_buffer->head_idx < ring_buffer->tail_idx)
    {
        ring_buffer->elems_cnt = ring_buffer->buff_capacity - ring_buffer->tail_idx + ring_buffer->head_idx;
    }
    else
    {
        if (!ring_buffer->is_empty)
        {
            ring_buffer->elems_cnt = ring_buffer->buff_capacity;
        }
        else
        {
            ring_buffer->elems_cnt = 0U;
        }
    }
}