exec = a.out
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g -Wall -lm -ldl -fPIC -rdynamic

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

clean:
	-rm *.out
	-rm *.o
	-rm *.a
	-rm src/*.o
	-rm output.s
	-rm output.o
	-rm output

run:
	make && ./a.out examples/func_call.bet

lint:
	clang-tidy src/*.c src/include/*.h
