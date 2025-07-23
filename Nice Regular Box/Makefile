CC=gcc
CFLAGS=-I. -g
DEPS =
OBJ = nice_regular_box.o
TARGET=nrb.out
TESTER = test/checkoutput.sh

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $^ $(CFLAGS)

clean:
	rm -f *.o *.out

run: $(TARGET)
	./$(TARGET)

test: test-1 test-2 test-3 test-4 test-5
	@echo "All tests succeeded"

test-1: $(TARGET)
	$(TESTER) 1

test-2: $(TARGET)
	$(TESTER) 2

test-3: $(TARGET)
	$(TESTER) 3

test-4: $(TARGET)
	$(TESTER) 4

test-5: $(TARGET)
	$(TESTER) 5
