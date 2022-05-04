#include "unity.h"

#include "ring_buffer.h"
#include <string.h>

#define RING_BUFFER_SIZE (10U)

static ring_buffer_t ring_buffer;
static uint8_t buff[RING_BUFFER_SIZE];

void setUp(void)
{
    /* init ring buffer */
    ring_buffer_init(&ring_buffer, buff, RING_BUFFER_SIZE);
}

void tearDown(void)
{
    /* clear buffer at teardown */
    memset(buff, 0, RING_BUFFER_SIZE);
    ring_buffer_deinit(&ring_buffer);
}

void test_ring_buffer_init(void)
{
    bool result = ring_buffer_init(&ring_buffer, buff, sizeof(buff) / sizeof(uint8_t));

    TEST_ASSERT_EQUAL(true, result);
}

void test_ring_buffer_is_empty(void)
{
    TEST_ASSERT_EQUAL(true, ring_buffer.is_empty);
}

void test_ring_buffer_write_result_true(void)
{
    uint8_t elem = 0xAA;
    bool result = ring_buffer_write(&ring_buffer, elem);

    TEST_ASSERT_EQUAL(true, result);
}

void test_ring_buffer_not_empty(void)
{
    uint8_t elem = 0xAA;
    ring_buffer_write(&ring_buffer, elem);

    TEST_ASSERT_EQUAL(false, ring_buffer.is_empty);
}

void test_ring_buffer_test_full(void)
{
    uint8_t write_elems[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10};

    /* write ring buffer until its full */
    for (int i = 0; i < 10; i++)
    {
        ring_buffer_write(&ring_buffer, write_elems[i]);
    }

    TEST_ASSERT_EQUAL_UINT32(ring_buffer.buff_capacity, ring_buffer_get_elems_cnt(&ring_buffer));
}

void test_ring_buffer_test_full_write_read(void)
{
    uint8_t write_elems[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10};
    uint8_t read_elems[10] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

    /* write ring buffer until its full */
    for (int i = 0; i < 10; i++)
    {
        ring_buffer_write(&ring_buffer, write_elems[i]);
    }

    /* read all elements */
    for (int i = 0; i < 10; i++)
    {
        ring_buffer_read(&ring_buffer, &read_elems[i]);
    }

    TEST_ASSERT_EQUAL_UINT8_ARRAY(write_elems, read_elems, 10);
}

void test_ring_buffer_elems_cnt_after_multiple_write_read(void)
{
    uint8_t elems[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10};

    for (int i = 0; i < 10; i++)
    {
        ring_buffer_write(&ring_buffer, elems[i]);
    }

    uint8_t byte = 0x00;

    for (int i = 0; i < 5; i++)
    {
        ring_buffer_read(&ring_buffer, &byte);
    }

    TEST_ASSERT_EQUAL_UINT32(5U, ring_buffer_get_elems_cnt(&ring_buffer));
}

void test_ring_buffer_empty_after_full_write_read(void)
{
    uint8_t write_elems[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10};
    uint8_t read_elems[10] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

    /* write ring buffer until its full */
    for (int i = 0; i < 10; i++)
    {
        ring_buffer_write(&ring_buffer, write_elems[i]);
    }

    /* read all elements */
    for (int i = 0; i < 10; i++)
    {
        ring_buffer_read(&ring_buffer, &read_elems[i]);
    }

    TEST_ASSERT_EQUAL(true, ring_buffer.is_empty);
}

void test_ring_buffer_write_read_multiple_times(void)
{
    uint8_t write_elems[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10};

    uint8_t read_byte = 0x00;

    /* write 5 bytes to ring buffer */

    for (int i = 0; i < 5; i++)
    {
        ring_buffer_write(&ring_buffer, write_elems[i]);
    }

    /* now we have 0x01, 0x02, 0x03, 0x04, 0x05 in buffer - read these bytes. Buffer should be empty, tail and head idx are at address 5/10 */

    for (int i = 0; i < 5; i++)
    {
        ring_buffer_read(&ring_buffer, &read_byte);
    }

    /* write 6 bytes - exceed buffer, head should wind up to the 0 idx of array  */

    for (int i = 0; i < 6; i++)
    {
        ring_buffer_write(&ring_buffer, write_elems[i]);
    }

    for (int i = 0; i < 6; i++)
    {
        ring_buffer_read(&ring_buffer, &read_byte);
    }

    /* last read byte should be 0x06 */

    TEST_ASSERT_EQUAL_UINT8(0x06, read_byte);
}