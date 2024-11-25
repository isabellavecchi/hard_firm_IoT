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

volatile int new_frame;
volatile int frame_proc;

void proc(int *s)
{
	CO_BEGIN(s);

	MSG("Esperando frame ",__FUNCTION__,__LINE__);

	CO_WAIT_UNTIL(s,new_frame);

	frame_proc = 0;

	MSG("Processando frame a ... ",__FUNCTION__,__LINE__);
	C_YIELD(s);	

	MSG("Processando frame b ... ",__FUNCTION__,__LINE__);
	C_YIELD(s);	

	MSG("Processando frame c ... ",__FUNCTION__,__LINE__);
	C_YIELD(s);	

	MSG("Frame processado ! ",__FUNCTION__,__LINE__);
	frame_proc = 1;
	C_YIELD(s);

	CO_END(s);
}

void recv(int *s)
{
	CO_BEGIN(s);

	CO_WAIT_UNTIL(s,frame_proc);

	new_frame = 0;

	MSG("Recebendo parte 1 ... ",__FUNCTION__,__LINE__);
	C_YIELD(s);	

	MSG("Recebendo parte 2 ... ",__FUNCTION__,__LINE__);
	C_YIELD(s);	

	MSG("Recebendo parte 3 ... ",__FUNCTION__,__LINE__);
	C_YIELD(s);	

	MSG("Novo frame ! ",__FUNCTION__,__LINE__);
	new_frame = 1;
	C_YIELD(s);

	CO_END(s);
}

int main(void)
{
	int srecv = 0, sproc = 0;
	new_frame = 0;
	frame_proc = 1;

	while(1)
	{
		proc(&sproc);
		sleep(2);

		recv(&srecv);
		sleep(2);
	}
}