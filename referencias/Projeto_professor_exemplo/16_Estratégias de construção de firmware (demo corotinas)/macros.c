#include <stdio.h>

#define MSG(file,func,line) printf("Estamos na função %s, linha %d (%s)\n",func,line,file)

void print_line(int line)
{
	printf("Função %s chamada.\nVoltando agora para a linha %d\n",__FUNCTION__,line);
	MSG(__FILE__,__FUNCTION__,__LINE__);
}

int main(void)
{
	printf("Executando arquivo %s\n",__FILE__);
	print_line(__LINE__);
	MSG(__FILE__,__FUNCTION__,__LINE__);
	
	return 0;
}