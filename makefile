all: kernel

kernel: kernel.o cd.o pwd.o pinfo.o other.o echo.o pipe.o redirect.o direct.o jobs.o
	gcc kernel.o cd.o pwd.o pinfo.o other.o echo.o pipe.o redirect.o direct.o jobs.o -o kernel

kernel.o: kernel.c 
	gcc -c kernel.c
	
cd.o: cd.c 
	gcc -c cd.c

pwd.o: pwd.c 
	gcc -c pwd.c

pinfo.o: pinfo.c 
	gcc -c pinfo.c

other.o: other.c 
	gcc -c other.c

echo.o: echo.c 
	gcc -c echo.c

direct.o: direct.c
	gcc -c direct.c

pipe.o: pipe.c
	gcc -c pipe.c

redirect.o: redirect.c
	gcc -c redirect.c

jobs.o: jobs.c
	gcc -c jobs.c

clean:
	rm -rf *o kernel
