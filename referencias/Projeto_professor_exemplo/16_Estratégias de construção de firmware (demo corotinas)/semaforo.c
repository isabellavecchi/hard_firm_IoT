
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#undef NOOB

typedef enum sm_state_e
{
	SM_RESET  = 0,
	SM_RED    = 1,
	SM_GREEN  = 2,
	SM_YELLOW = 3,
	SM_WALK   = 4
} sm_state_t;

typedef enum sm_output_e
{
	OUT_NONE   = 0x00,
	OUT_RED    = 0x01,
	OUT_GREEN  = 0x02,
	OUT_YELLOW = 0x04,
	OUT_WALK   = 0x08
} sm_output_t;

typedef struct sm_state_var_s
{
	uint32_t tmr;
	uint32_t pdr;
	sm_state_t state;
	sm_output_t out;	
} sm_state_var_t;

char *state_names[] = { "RESET", "RED", "GREEN", "YELLOW", "WALK" };
typedef sm_state_t (*sm_func_t)(sm_state_var_t *st);

typedef struct sm_state_table_s
{
	sm_func_t func;
} sm_state_table_t;

sm_state_t sm_reset_state(sm_state_var_t *st)
{
	sm_state_t next_state = SM_RED;

	st->tmr = 0;
	st->pdr = 0;

	return next_state;
}

sm_state_t sm_red_state(sm_state_var_t *st)
{
	sm_state_t next_state = SM_RED;

	st->out = OUT_RED;
	st->tmr++;

	if(st->tmr >= 45)
	{
		st->tmr = 0;
		next_state = SM_GREEN;
	}

	return next_state;
}

sm_state_t sm_green_state(sm_state_var_t *st)
{
	sm_state_t next_state = SM_GREEN;

	st->out = OUT_GREEN;
	st->tmr++;

	if(st->tmr >= 60)
	{
		st->tmr = 0;
		next_state = SM_YELLOW;
	}

	return next_state;
}

sm_state_t sm_yellow_state(sm_state_var_t *st)
{
	sm_state_t next_state = SM_YELLOW;

	st->out = OUT_YELLOW;
	st->tmr++;

	if(st->tmr >= 5)
	{
		st->tmr = 0;

		if(st->pdr)
			next_state = SM_WALK;
		else
			next_state = SM_RED;
	}

	return next_state;
}

sm_state_t sm_walk_state(sm_state_var_t *st)
{
	sm_state_t next_state = SM_WALK;

	st->out = OUT_WALK | OUT_RED;
	st->tmr++;

	if(st->tmr >= 30)
	{
		st->tmr = 0;
		st->pdr = 0;
		next_state = SM_RED;
	}

	return next_state;
}

sm_state_table_t sm_state_table[] = 
{
	sm_reset_state,  // SM_RESET  = 0,
	sm_red_state,    // SM_RED    = 1,
	sm_green_state,  // SM_GREEN  = 2,
	sm_yellow_state, // SM_YELLOW = 3,
	sm_walk_state    // SM_WALK   = 4
};


void sm_func(sm_state_var_t *st)
{
	uint8_t last_state = st->state;

	st->state = sm_state_table[st->state].func(st);

	if(last_state != st->state)
	{
		printf("STATE %s -> %s\r\n", state_names[last_state], state_names[st->state]);
		fflush(stdout);
	}
}

int main(void)
{
	sm_state_var_t st;

	st.state = SM_RESET;

	while(1)
	{
		// update_inputs();
		sm_func(&st);
		// update_output();
		usleep(100000); // 10x faster
	}

	return 0;
}

#ifdef NOOB
void loop(void)
{
	// pdr é uma indicação de sinal de pedestre acionado, 
	// deve vir de alguma interrupção, por exemplo
	digitalWrite(red,HIGH);
	digitalWrite(green,LOW);
	digitalWrite(yellow,LOW);
	digitalWrite(walk,LOW);
	delay(45000);

	digitalWrite(red,LOW);
	digitalWrite(green,HIGH);
	digitalWrite(yellow,LOW);
	digitalWrite(walk,LOW);
	delay(60000);

	digitalWrite(red,LOW);
	digitalWrite(green,LOW);
	digitalWrite(yellow,HIGH);
	digitalWrite(walk,LOW);
	delay(5000);

	if(pdr)
	{
		digitalWrite(red,HIGH);
		digitalWrite(green,LOW);
		digitalWrite(yellow,LOW);
		digitalWrite(walk,HIGH);
		delay(5000);
		pdr = 0;
	}
}
#endif
