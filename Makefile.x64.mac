SOURCES:=$(wildcard src/*.c) $(wildcard src/**/*.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=./bubbles

all: build
	$(EXECUTABLE)

build: $(OBJECTS)
	gcc -o $(EXECUTABLE) -F/Library/Frameworks -framework SDL2 -framework SDL2_image -framework SDL2_mixer $(CFLAGS) $^

.c.o:
	gcc -c -Wall -Wextra -o $@ $(CFLAGS) -F/Library/Frameworks $<

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)