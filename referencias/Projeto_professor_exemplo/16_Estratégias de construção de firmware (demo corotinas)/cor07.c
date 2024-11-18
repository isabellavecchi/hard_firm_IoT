#include <stdio.h>
#include <unistd.h>

#define MSG(func,line) printf("%s:%d\n",func,line)

#define CO_BEGIN(s)   int yield = 0 ; \
					  switch(*s) \
					  { \
					  case 0: 

#define CO_END(s)  } \
				   *s = 0; 

#define C_YIELD(s)  yield = 1; *s = __LINE__; case __LINE__: if(yield) return;

void func_a(int *s)
{
	CO_BEGIN(s);

	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	C_YIELD(s);

	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	C_YIELD(s);
	
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);

	CO_END(s);
}


void func_b(void)
{
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
	MSG(__FUNCTION__,__LINE__);
}

int main(void)
{
	int sa = 0;

	while(1)
	{
		printf("\n");
		func_a(&sa);
		sleep(3);

		printf("\n");
		func_b();
		sleep(3);
	}
}