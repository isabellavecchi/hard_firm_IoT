
# instalar a libncurses:
# sudo apt-get install libncurses-dev

all: loop sma smb smc smd sme smf

clean:
	-rm super_loop sm_a sm_b sm_c sm_d sm_e sm_f

loop:
	gcc -g -o0 -o super_loop super_loop.c

sma:
	gcc -g -O0 -o sm_a sm_a.c -lncurses

smb:
	gcc -g -O0 -o sm_b sm_b.c -lncurses

smc:
	gcc -g -O0 -o sm_c sm_c.c -lncurses

smd:
	gcc -g -O0 -o sm_d sm_d.c -lncurses

sme:
	gcc -g -O0 -o sm_e sm_e.c -lncurses

smf:
	gcc -g -O0 -o sm_f sm_f.c -lncurses

