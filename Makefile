include config.mk

all: learn-colour learn-compass

learn-%: learn-%.o window.o vector.o 
	$(CC) -o $@ $^ $(LDFLAGS)


%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)


clean:
	rm -f learn-compass learn-colour
	find -name "*.o" -delete


.PHONY: clean all
