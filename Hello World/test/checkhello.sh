#!/bin/bash
echo "Compiling your code..."
gcc hello.c -o hello
echo "Running your code..."
./hello > actual.txt
echo "Checking actual versus expected..."
echo "Actual:["
cat actual.txt
echo "]"
echo "Expected: ["
cat test/expected.txt
echo "]"
cmp actual.txt test/expected.txt
if [ $? -eq 0 ]; then
    echo "Success!"
    exit 0
else
    echo "Failed :("
    exit 1
fi
