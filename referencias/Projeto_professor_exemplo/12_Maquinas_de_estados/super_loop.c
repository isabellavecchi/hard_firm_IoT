
#include<stdint.h>

volatile uint32_t setup_ctrl;

void read_input(void)
{
	return;
}

void write_output(void)
{
	return;
}

void process(void)
{
	return;
}

void communication(void)
{
	return;
}

void load_config(void)
{
	return;
}

void save_config(void)
{
	return;
}

void setup()
{
	return;
}

void enter_setup()
{
	setup_ctrl = 1;
}

void leave_setup()
{
	setup_ctrl = 0;
}

int main(void)
{
	uint32_t comm_cycle = 0;
	setup_ctrl = 0;

	load_config();

	while(1)
	{
		if(!setup_ctrl)
		{
			read_input();
			process();
			write_output();

			if(comm_cycle > 10)
			{
				communication();
				comm_cycle = 0;
			}
		}
		else
		{
			setup();
			communication();
		}

	}
}