
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>

#define NUM_DIGITS 4
#define PASSWORD   "1234"

typedef enum sm_state_e
{
	SM_RESET = 0,
	SM_WAIT_DIGITS,
	SM_CHECK_CODE
} sm_state_t;

typedef struct sm_state_var_s
{
	uint8_t pos;
	sm_state_t state;
	uint8_t code[NUM_DIGITS];
} sm_state_var_t;

sm_state_t sm_reset_state(sm_state_var_t *st)
{
	sm_state_t next_state = SM_WAIT_DIGITS;

	st->pos = 0;

	return next_state;
}

sm_state_t sm_wait_digits_state(sm_state_var_t *st)
{
	int c;
	sm_state_t next_state = st->state;

	c = getch();
	if(c >= 0)
	{
		if(c == 27 || c == 10)
			next_state = SM_RESET;
		else
		{
			st->code[st->pos] = (uint8_t) c;
			st->pos++;
			if(st->pos >= NUM_DIGITS)
				next_state = SM_CHECK_CODE;
		}
	}

	return next_state;
}
sm_state_t sm_check_code_state(sm_state_var_t *st)
{
	sm_state_t next_state = SM_RESET;

	if(strncmp(st->code,PASSWORD,4) == 0)
		printf("PORTA ABERTA !\r\n");
	else
		printf("CODIGO ERRADO !\r\n");

	return next_state;
}

void sm_func(sm_state_var_t *st)
{
	uint8_t last_state = st->state;

	switch(st->state)
	{
		case SM_RESET:
			st->state = sm_reset_state(st);
			break;
		case SM_WAIT_DIGITS:
			st->state = sm_wait_digits_state(st);
			break;
		case SM_CHECK_CODE:
			st->state = sm_check_code_state(st);
			break;
		default:
			st->state = SM_RESET;
			break;
	}

	if(last_state != st->state)
	{
		printf("\r\nSTATE %d -> %d\r\n", last_state, st->state);
		fflush(stdout);
	}
}

int main(void)
{
	sm_state_var_t st;

	st.state = SM_RESET;

	initscr();
	cbreak();

	while(1)
	{
		sm_func(&st);
		usleep(1000);
	}

	endwin();

	return 0;
}

// gcc -o sm_a sm_a.c -lcurses -g -O0
