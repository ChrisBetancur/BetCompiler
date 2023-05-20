exec = output
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g -Wall -lm -ldl -fPIC -rdynamic
path = /home/c_bet/Projects/BetCompiler

$(exec): $(objects)
	gcc -g $(objects) $(flags) -o $(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

clean_output:
	-rm output.asm
	-rm output.o
	-rm output


clean:
	-rm *.o
	#-rm *.a
	-rm src/*.o
	$(MAKE) clean_output

run:
	$(MAKE) clean_output
	#-make && ./$(exec) examples/test.bet
	-make

lint:
	clang-tidy src/*.c src/include/*.h
