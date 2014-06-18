all: Monitor Controller

Monitor: PSD.o Monitor.o
	gcc -o Monitor PSD.o Monitor.o

Controller: PSD.o Controller.o
	gcc -o Controller PSD.o Controller.o

Monitor.o: Monitor.c PSD.h
	gcc -c Monitor.c

PSD.o: PSD.c PSD.h
	gcc -c PSD.c

Controller.o: Controller.c PSD.h
	gcc -c Controller.c

clean:
	$(RM) Monitor Controller

