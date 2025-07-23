CC=gcc
CPP=g++
CFLAGS=-I. -g
CPPFLAGS = -I. -g -std=c++11
DEPS =
OBJ =
TESTOBJ = test/test_gas.o
TESTTARGET=test_gas
LDFLAGS= -no-pie -z noexecstack -g

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

giza: giza.s
	$(CC) $(LDFLAGS) giza.s -o giza

wc: wc.s
	$(CC) $(LDFLAGS) wc.s -o wc

clean:
	rm -f *.o giza wc test_gas test/*.o


$(TESTTARGET): $(TESTOBJ)
	$(CPP) -o test_gas $(TESTOBJ) $(CPPFLAGS)

test-giza-1: giza $(TESTTARGET)
	./$(TESTTARGET) Giza.1

test-giza-2: giza $(TESTTARGET)
	./$(TESTTARGET) Giza.2

test-giza-5: giza $(TESTTARGET)
	./$(TESTTARGET) Giza.5

test-giza-10: giza $(TESTTARGET)
	./$(TESTTARGET) Giza.10

test-giza-21: giza $(TESTTARGET)
	./$(TESTTARGET) Giza.21

test-giza: giza $(TESTTARGET)
	./$(TESTTARGET) "[Giza]"


test-wc-empty: wc $(TESTTARGET)
	./$(TESTTARGET) Wc.empty

test-wc-one: wc $(TESTTARGET)
	./$(TESTTARGET) Wc.one

test-wc-space: wc $(TESTTARGET)
	./$(TESTTARGET) Wc.space

test-wc-multi: wc $(TESTTARGET)
	./test_gas Wc.multi


test-wc: wc $(TESTTARGET)
	./test_gas "[Wc]"
