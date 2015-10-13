CFLAGS += \
	-I/usr/include/SDL2 \
	-Wall \
	-Wextra \
	-Werror
LDFLAGS += \
	-lm \
	-lSDL2

all: colour-ai compass

compass: \
	compass.o \
	window.o\
	vector.o
	$(CC) -o $@ $^ $(LDFLAGS)

colour-ai:\
	colour-ai.o \
	window.o \
	vector.o
	$(CC) -o $@ $^ $(LDFLAGS)


%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)


clean:
	rm -f colour-ai compass
	find -name "*.o" -delete


.PHONY: clean all
