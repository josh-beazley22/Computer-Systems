CC=gcc
CPP=g++
CFLAGS=-I. -g
CPPFLAGS = -I. -g -std=c++11
DEPS =
OBJ = convert.o
LDFLAGS = -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

default: $(OBJ)
	$(CC) -o convert $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o *.out test/*.o test_convert convert

run: default
	./convert

TESTOBJ = test/test_convert.o

test_convert: $(TESTOBJ)
	$(CPP) -o test_convert $(TESTOBJ) $(CPPFLAGS)

test: test_convert default
	./test_convert

test-6-to-binary: test_convert default
	./test_convert Convert.6_to_binary

test-789-to-binary: test_convert default
	./test_convert Convert.789_to_binary

test-10-to-hex: test_convert default
	./test_convert Convert.10_to_hex

test-672-to-hex: test_convert default
	./test_convert Convert.672_to_hex

test-multiple-binary: test_convert default
	./test_convert Convert.multiple_binary

test-multiple-hex: test_convert default
	./test_convert Convert.multiple_hex

test-bad-args: test_convert default
	./test_convert Convert.bad_args
	./test_convert Convert.no_option

test-style: test_convert default
	./test_convert Convert.style
