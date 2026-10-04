#include "unity.h"
#include "avr/io.h"
#include "io_macros.h"

void setUp(void) {}
void tearDown(void) {}


#define LED_B5 B, 5
#define LED_D7 D, 7

static void reset(void) { fake_io_reset(); }

// IO_MODE sets or clears the DDR bit for the given pin.
static void io_mode_when_output_should_set_ddr_bit(void)
{
    reset();
    IO_MODE(LED_B5, IO_OUTPUT);
    TEST_ASSERT_EQUAL_UINT8(0x20, DDRB);
}

static void io_mode_when_input_should_clear_ddr_bit(void)
{
    reset();
    DDRB = 0xFF;
    IO_MODE(LED_B5, IO_INPUT);
    TEST_ASSERT_EQUAL_UINT8(0xDF, DDRB);
}

// IO_WRITE writes the PORT bit without touching neighbours.
static void io_write_high_should_only_set_one_bit(void)
{
    reset();
    PORTB = 0x01;
    IO_WRITE(LED_B5, IO_HIGH);
    TEST_ASSERT_EQUAL_UINT8(0x21, PORTB);
}

static void io_write_low_should_only_clear_one_bit(void)
{
    reset();
    PORTB = 0xFF;
    IO_WRITE(LED_B5, IO_LOW);
    TEST_ASSERT_EQUAL_UINT8(0xDF, PORTB);
}

// IO_READ returns true/false for the PIN register.
static void io_read_should_reflect_pin_register(void)
{
    reset();
    PINB = 0x20;
    TEST_ASSERT_TRUE(IO_READ(LED_B5));
    PINB = 0x00;
    TEST_ASSERT_FALSE(IO_READ(LED_B5));
}

// IO_TOGGLE flips the PORT bit each call, leaves others alone.
static void io_toggle_should_flip_only_target_bit(void)
{
    reset();
    PORTB = 0xA5;
    IO_TOGGLE(LED_B5);
    TEST_ASSERT_EQUAL_UINT8(0x85, PORTB);
    IO_TOGGLE(LED_B5);
    TEST_ASSERT_EQUAL_UINT8(0xA5, PORTB);
}

// IO_MODE_TOGGLE flips the DDR bit.
static void io_mode_toggle_should_flip_ddr_bit(void)
{
    reset();
    DDRD = 0x00;
    IO_MODE_TOGGLE(LED_D7);
    TEST_ASSERT_EQUAL_UINT8(0x80, DDRD);
    IO_MODE_TOGGLE(LED_D7);
    TEST_ASSERT_EQUAL_UINT8(0x00, DDRD);
}

// IO_BIT_SET / CLEAR / TOGGLE / IS_SET on a plain register.
static void io_bit_ops_should_round_trip(void)
{
    uint8_t reg = 0;
    IO_BIT_SET(reg, 3);
    TEST_ASSERT_EQUAL_UINT8(0x08, reg);
    TEST_ASSERT_TRUE(IO_BIT_IS_SET(reg, 3));
    IO_BIT_CLEAR(reg, 3);
    TEST_ASSERT_EQUAL_UINT8(0x00, reg);
    TEST_ASSERT_FALSE(IO_BIT_IS_SET(reg, 3));
    IO_BIT_TOGGLE(reg, 7);
    TEST_ASSERT_EQUAL_UINT8(0x80, reg);
}

// Regression: parenthesisation of macro arguments. The old header expanded
// BitCheck(a|b, 3) as `a | b & mask` which gave the wrong answer.
static void io_bit_is_set_should_handle_expression_arguments(void)
{
    uint8_t a = 0x01;
    uint8_t b = 0x08;
    TEST_ASSERT_TRUE(IO_BIT_IS_SET(a | b, 3));
    TEST_ASSERT_FALSE(IO_BIT_IS_SET(a & b, 3));
}

// Regression: no 32-bit shift when the bit index fits in uint8_t.
static void io_bit_set_should_fit_in_uint8(void)
{
    uint8_t reg = 0;
    for (uint8_t bit = 0; bit < 8; bit++) {
        reg = 0;
        IO_BIT_SET(reg, bit);
        TEST_ASSERT_EQUAL_UINT8((uint8_t)(1u << bit), reg);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(io_mode_when_output_should_set_ddr_bit);
    RUN_TEST(io_mode_when_input_should_clear_ddr_bit);
    RUN_TEST(io_write_high_should_only_set_one_bit);
    RUN_TEST(io_write_low_should_only_clear_one_bit);
    RUN_TEST(io_read_should_reflect_pin_register);
    RUN_TEST(io_toggle_should_flip_only_target_bit);
    RUN_TEST(io_mode_toggle_should_flip_ddr_bit);
    RUN_TEST(io_bit_ops_should_round_trip);
    RUN_TEST(io_bit_is_set_should_handle_expression_arguments);
    RUN_TEST(io_bit_set_should_fit_in_uint8);
    return UNITY_END();
}
