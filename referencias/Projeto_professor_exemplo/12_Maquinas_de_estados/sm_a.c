
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
	// uint32_t tmr_out;
	sm_state_t state;
	uint8_t code[NUM_DIGITS];
} sm_state_var_t;

void sm_func(sm_state_var_t *st)
{
	int c;
	uint8_t last_state = st->state;

	switch(st->state)
	{
		case SM_RESET:
			st->pos = 0;
			// st->tmr_out = 0;
			st->state = SM_WAIT_DIGITS;
			break;
		case SM_WAIT_DIGITS:
			// st->tmr_out++;
			c = getch();
			if(c >= 0)
			{
				if(c == 27 || c == 10)
					st->state = SM_RESET;
				else 
				{
					st->code[st->pos] = (uint8_t) c;
					st->pos++;
					if(st->pos >= NUM_DIGITS)
						st->state = SM_CHECK_CODE;
				}
			}
			// if(st->tmr_out > 5000 && st->pos > 0)
				// st->state = SM_RESET;
			break;
		case SM_CHECK_CODE:
			if(strncmp(st->code,PASSWORD,4) == 0)
				printf("PORTA ABERTA !\r\n");
			else
				printf("CODIGO ERRADO !\r\n");
			st->state = SM_RESET;
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
