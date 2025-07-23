CC = gcc
CPP = g++
CFLAGS = -I. -g
CPPFLAGS = -I. -g -std=c++17
DEPS =
OBJ = shell.o
TESTOBJ = test/test_main.o
TESTTARGET = test_shell

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	g++ -c -o $@ $< $(CPPFLAGS)

default: $(OBJ)
	$(CC) -o shell $^ $(CFLAGS)

clean:
	rm -f *.o *.out shell

run: default
	./shell

$(TESTTARGET): $(TESTOBJ)
	$(CPP) $(CPPFLAGS) -o $(TESTTARGET) $(TESTOBJ)

test-ls: default $(TESTTARGET)
	./$(TESTTARGET) [Ls]

test-ls-no-args: default $(TESTTARGET)
	./$(TESTTARGET) Shell.LsNoArgs

test-ls-one-arg: default $(TESTTARGET)
	./$(TESTTARGET) Shell.LsOneArg

test-cd: default $(TESTTARGET)
	./$(TESTTARGET) [Cd]

test-cd-one-arg: default $(TESTTARGET)
	./$(TESTTARGET) Shell.CdOneArg

test-cd-no-args: default $(TESTTARGET)
	./$(TESTTARGET) Shell.CdNoArgs

test-last4: default $(TESTTARGET)
	./$(TESTTARGET) [Last4]

test-last4-long: default $(TESTTARGET)
	./$(TESTTARGET) Shell.Last4Long

test-last4-short: default $(TESTTARGET)
	./$(TESTTARGET) Shell.Last4Short

test: default $(TESTTARGET)
	./$(TESTTARGET)
