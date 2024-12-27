exec = bin/betc
sources = $(wildcard src/*.c)
objects = $(patsubst src/%.c, obj/%.o, $(sources))
flags = -g -Wall -fPIC -rdynamic
path = /home/c_bet/Projects/BetCompiler

# Define cross-compilation settings for Raspberry Pi
ifeq ($(PLATFORM),raspberry)
    CC = arm-linux-gnueabihf-gcc
    CXX = arm-linux-gnueabihf-g++
    CFLAGS += -I/usr/include
    LDFLAGS += -L/usr/lib -lm -ldl
else
    CC = gcc
    CXX = g++
    CFLAGS += -I/usr/include
    LDFLAGS += -lm -ldl
endif

$(exec): $(objects)
	$(CC) $(objects) $(flags) -o $(exec) $(LDFLAGS)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@

clean_output:
	-rm -rf bin
	-rm -rf target
	-rm -rf obj

clean:
	$(MAKE) clean_output

run:
	$(MAKE) clean_output
	make

lint:
	clang-tidy src/*.c src/include/*.h

$(shell mkdir -p bin)
$(shell mkdir -p target)

.PHONY: clean_output clean run lint
