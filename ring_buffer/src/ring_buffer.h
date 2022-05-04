#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define RING_BUFF_RET_OK (0U)
#define RING_BUFF_RET_ERR_OVERFLOW (1U)
#define RING_BUFF_RET_ERR_BUFFER_EMPTY (2U)

typedef struct
{
    uint8_t *buff_ptr;
    uint32_t head_idx;
    uint32_t tail_idx;

    uint32_t buff_capacity;
    uint32_t elems_cnt;
    bool is_empty;

} ring_buffer_t;

bool ring_buffer_init(ring_buffer_t *ring_buffer, uint8_t *buffer_ptr, uint32_t buff_capacity);

void ring_buffer_deinit(ring_buffer_t *ring_buffer);

bool ring_buffer_write(ring_buffer_t *ring_buffer, uint8_t byte);

bool ring_buffer_read(ring_buffer_t *ring_buffer, uint8_t *byte);

uint32_t ring_buffer_get_elems_cnt(ring_buffer_t *ring_buffer);

#endif // RING_BUFFER_H
