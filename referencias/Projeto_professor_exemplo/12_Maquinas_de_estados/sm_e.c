
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>

#define NUM_DIGITS 4
#define PASSWORD   "1234"

// X Macros
// Cria a lista de elementos que devem ser mantidos juntos. No caso, temos:
//
// 1) o indice usado na enumeracao
// 2) o ponteiro para a funcao que executa o comando
// 3) a string que representa o estado
#define XMACRO_STATE_LIST \
    X(0, SM_RESET,       sm_reset_state,       "RESET"      ) \
    X(1, SM_WAIT_DIGITS, sm_wait_digits_state, "WAIT DIGITS") \
    X(2, SM_CHECK_CODE,  sm_check_code_state,  "CHECK CODE" )

typedef enum sm_state_e
{
#define X(IDX, ENUM, FUNC, NAME) ENUM = IDX,
	XMACRO_STATE_LIST
#undef X
} sm_state_t;

static const char *sm_state_names[] = 
{
#define X(IDX, ENUM, FUNC, NAME) NAME,
	XMACRO_STATE_LIST
#undef X
};

typedef struct sm_state_var_s
{
	uint8_t pos;
	sm_state_t state;
	uint8_t code[NUM_DIGITS];
} sm_state_var_t;

// sm_state_t sm_func_t(sm_state_var_t *st);
// typedef sm_state_t (*sm_func_t)(sm_state_var_t *st);
// sm_func_t func;
// func = sm_reset_state;
// func(st) == sm_reset_state(st)

typedef sm_state_t (*sm_func_t)(sm_state_var_t *st);

typedef struct sm_state_table_s
{
	sm_func_t func;
} sm_state_table_t;

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

sm_state_table_t sm_state_table[] = 
{
#define X(IDX, ENUM, FUNC, NAME) FUNC,
	XMACRO_STATE_LIST
#undef X
};

void sm_func(sm_state_var_t *st)
{
	uint8_t last_state = st->state;

	//st->state = sm_state_table[st->state](st);

	st->state = sm_state_table[st->state].func(st);

	if(last_state != st->state)
	{
		printf("\r\nSTATE '%s' (%d) -> '%s' (%d)\r\n", 
			sm_state_names[last_state], last_state, 
			sm_state_names[st->state], st->state);
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
