#include <stdio.h>
#include <unistd.h>

#define MSG(func,line) printf("%s:%d\n",func,line)

void func_a(int *s)
{
	int yield = 0;

	switch(*s)
	{
		case 0:
			MSG(__FUNCTION__,__LINE__);
			MSG(__FUNCTION__,__LINE__);
			MSG(__FUNCTION__,__LINE__);
			MSG(__FUNCTION__,__LINE__);
			yield = 1; *s = __LINE__; case __LINE__: if(yield) return;
			MSG(__FUNCTION__,__LINE__);
			MSG(__FUNCTION__,__LINE__);
			MSG(__FUNCTION__,__LINE__);
			MSG(__FUNCTION__,__LINE__);
			yield = 1; *s = __LINE__; case __LINE__: if(yield) return;
			MSG(__FUNCTION__,__LINE__);
			MSG(__FUNCTION__,__LINE__);
			MSG(__FUNCTION__,__LINE__);
			MSG(__FUNCTION__,__LINE__);
	}
	*s = 0;
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