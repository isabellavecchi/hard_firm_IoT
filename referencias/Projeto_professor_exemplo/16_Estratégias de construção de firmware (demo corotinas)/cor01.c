#include <stdio.h>
#include <unistd.h>


#define MSG(func,line) printf("%s:%d\n",func,line)

void func_a(void)
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
	while(1)
	{
		printf("\n");
		func_a();
		sleep(3);

		printf("\n");
		func_b();
		sleep(3);
	}
}