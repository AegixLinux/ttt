/* test_ttt.c: unit tests for ttt's pure logic (dial.h, input.h) */
#include <math.h>
#include <stdio.h>

#include "dial.h"
#include "input.h"

static int fails;

#define OK(cond) do { \
	if (!(cond)) { \
		printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
		fails++; \
	} \
} while (0)

#define NEAR(a, b) (fabs((a) - (b)) < 1e-6)

static void
test_angfrac(void)
{
	OK(NEAR(angfrac(0.0, -1.0), 0.0));   /* top, 12 o'clock */
	OK(NEAR(angfrac(-1.0, 0.0), 0.25));  /* left, 15 min mark */
	OK(NEAR(angfrac(0.0, 1.0), 0.5));    /* bottom, 30 min mark */
	OK(NEAR(angfrac(1.0, 0.0), 0.75));   /* right, 45 min mark */
}

static void
test_in_wedge(void)
{
	/* 30 minutes remaining: left half is red, right half is not */
	OK(in_wedge(-1.0, 0.0, 0.5) == 1);
	OK(in_wedge(1.0, 0.0, 0.5) == 0);
	/* just past the top, counterclockwise side, always in while running */
	OK(in_wedge(-0.05, -1.0, 0.1) == 1);
	/* clockwise of the wedge edge at 10 min remaining */
	OK(in_wedge(0.0, 1.0, 1.0 / 6.0) == 0);
	/* timer at zero: nothing is red, not even the top line */
	OK(in_wedge(0.0, -1.0, 0.0) == 0);
	/* full hour: everything is red */
	OK(in_wedge(1.0, 0.0, 1.0) == 1);
	OK(in_wedge(0.3, -0.7, 1.0) == 1);
}

static void
test_input_two_digits_commit(void)
{
	Input in;
	input_init(&in);
	OK(input_feed(&in, '2', 10.0) == 0);
	OK(input_feed(&in, '5', 10.2) == 25); /* second digit commits */
}

static void
test_input_enter_commits_single_digit(void)
{
	Input in;
	input_init(&in);
	OK(input_feed(&in, '5', 10.0) == 0);
	OK(input_feed(&in, '\r', 10.2) == 5);
}

static void
test_input_newline_commits_too(void)
{
	Input in;
	input_init(&in);
	OK(input_feed(&in, '7', 10.0) == 0);
	OK(input_feed(&in, '\n', 10.2) == 7);
}

static void
test_input_timeout_commits_single_digit(void)
{
	Input in;
	input_init(&in);
	OK(input_feed(&in, '9', 10.0) == 0);
	OK(input_tick(&in, 10.0 + INPUT_TIMEOUT / 2) == 0); /* too soon */
	OK(input_tick(&in, 10.0 + INPUT_TIMEOUT + 0.1) == 9);
	OK(input_tick(&in, 10.0 + INPUT_TIMEOUT + 0.2) == 0); /* no repeat */
}

static void
test_input_rejects_out_of_range(void)
{
	Input in;
	input_init(&in);
	OK(input_feed(&in, '7', 10.0) == 0);
	OK(input_feed(&in, '0', 10.1) == 0); /* 70 is over 60, discarded */
	OK(input_feed(&in, '0', 20.0) == 0);
	OK(input_feed(&in, '\r', 20.1) == 0); /* 0 is invalid, discarded */
	/* after a discard, fresh entry still works */
	OK(input_feed(&in, '6', 30.0) == 0);
	OK(input_feed(&in, '0', 30.1) == 60);
}

static void
test_input_enter_with_empty_buffer_is_noop(void)
{
	Input in;
	input_init(&in);
	OK(input_feed(&in, '\r', 10.0) == 0);
	OK(input_tick(&in, 20.0) == 0);
}

int
main(void)
{
	test_angfrac();
	test_in_wedge();
	test_input_two_digits_commit();
	test_input_enter_commits_single_digit();
	test_input_newline_commits_too();
	test_input_timeout_commits_single_digit();
	test_input_rejects_out_of_range();
	test_input_enter_with_empty_buffer_is_noop();

	if (fails) {
		printf("%d assertion(s) failed\n", fails);
		return 1;
	}
	puts("all tests passed");
	return 0;
}
