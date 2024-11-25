#include <stdio.h>
#include <unistd.h>

#define MSG(func,line) printf("%s:%d\n",func,line)

void func_a(int *s)
{
	switch(*s)
	{
		case 0:
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