#include <stdio.h>
#include <unistd.h>

#define MSG(m,func,line) printf("%s:%d " m "\n",func,line)

#define CO_BEGIN(s)   { int yield = 0 ; \
					  switch(*s) \
					  { \
					  case 0: 

#define CO_END(s)  } \
				   *s = 0; }

#define C_YIELD(s)  yield = 1; *s = __LINE__; case __LINE__: if(yield) return;

#define CO_WAIT_UNTIL(s,cond) *s = __LINE__; case __LINE__: if(!cond) return;

volatile int process;

void proc(int *s)
{
	CO_BEGIN(s);

	MSG("Esperando ... ",__FUNCTION__,__LINE__);

	CO_WAIT_UNTIL(s,process);

	MSG("Processando a ... ",__FUNCTION__,__LINE__);
	C_YIELD(s);	

	MSG("Processando b ... ",__FUNCTION__,__LINE__);
	C_YIELD(s);	

	MSG("Processando c ... ",__FUNCTION__,__LINE__);
	C_YIELD(s);	

	MSG("Processado ! ",__FUNCTION__,__LINE__);
	process = 0;
	C_YIELD(s);

	CO_END(s);
}

void gene(int *s)
{
	CO_BEGIN(s);

	process = 0;
	MSG("Gerando dados 1 ... ",__FUNCTION__,__LINE__);
	C_YIELD(s);	

	MSG("Gerando dados 2 ... ",__FUNCTION__,__LINE__);
	process = 1;
	CO_WAIT_UNTIL(s,!process);
	C_YIELD(s);	

	MSG("Gerando dados 3 ... ",__FUNCTION__,__LINE__);
	C_YIELD(s);	

	MSG("Gerando dados 4 ... ",__FUNCTION__,__LINE__);
	process = 1;
	CO_WAIT_UNTIL(s,!process);
	C_YIELD(s);	

	CO_END(s);
}

int main(void)
{
	int srecv = 0, sproc = 0;
	process = 0;

	while(1)
	{
		proc(&sproc);
		sleep(2);

		gene(&srecv);
		sleep(2);
	}
}