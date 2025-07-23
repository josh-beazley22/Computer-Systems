CC=gcc
CFLAGS=-I. -g
CPPFLAGS=-I. -g -std=c++11
DEPS =
ODDOBJ = oddinacci.o
MATOBJ = matrix.o
MAT0OBJ = mat0.o
MAT1OBJ = mat1.o
MAT2OBJ = mat2.o
MEASUREOBJ = measure.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	g++ -c -o $@ $< $(CPPFLAGS)

default: $(ODDOBJ) $(MATOBJ) $(MAT0OBJ) $(MAT1OBJ) $(MAT2OBJ) $(MEASUREOBJ) oddinacci_main.o mat0_main.o mat1_main.o mat2_main.o
	$(CC) -o odd oddinacci_main.o $(ODDOBJ) $(MEASUREOBJ) $(CFLAGS)
	$(CC) -o mat0 mat0_main.o $(MATOBJ) $(MAT0BJ) $(MAT0OBJ) $(MEASUREOBJ) $(CFLAGS)
	$(CC) -o mat1 mat1_main.o $(MATOBJ) $(MAT0BJ) $(MAT1OBJ) $(MEASUREOBJ) $(CFLAGS)
	$(CC) -o mat2 mat2_main.o $(MATOBJ) $(MAT0BJ) $(MAT2OBJ) $(MEASUREOBJ) $(CFLAGS)

clean:
	rm -f opt_test test/*.o mat0 mat1 mat2 oddinacci.o mat?.o test/test_main.o

run: default
	./opt

test-build: $(ODDOBJ) $(MATOBJ) $(MAT0OBJ) $(MAT1OBJ) $(MAT2OBJ) $(MEASUREOBJ) test/test_main.o
	g++ test/test_main.o $(ODDOBJ) $(MATOBJ) $(MAT0OBJ) $(MAT1OBJ) $(MAT2OBJ) $(MEASUREOBJ) \
		-o opt_test

test-odd: test-build
	./opt_test Oddinacci

test-mat0: test-build
	./opt_test Matrix0

test-mat1: test-build
	./opt_test Matrix1

test-mat2: test-build
	./opt_test Matrix2

test: test-build
	./opt_test
