PREFIX?=/usr/X11R6
CFLAGS?=-Os -pedantic -Wall

all:
	$(CC) $(CFLAGS) -I$(PREFIX)/include fraunwm.c -L$(PREFIX)/lib -lX11 -o fraunwm

clean:
	rm -f fraunwm

