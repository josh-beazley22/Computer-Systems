#!/bin/bash

FILEPATH=$(dirname $0)
INPUT=$FILEPATH/input-$1.txt
EXPECTED=$FILEPATH/expected-$1.txt
ACTUAL=$FILEPATH/out
TARGET=nrb.out

echo "Running test $1"

echo "----> Input is"
cat $INPUT

echo ""

echo "----> Expected output is:"
cat $EXPECTED

echo ""

./nrb.out < $INPUT > $ACTUAL

echo "----> Your output is:"
cat $ACTUAL

echo ""

echo "----> Comparing actual with expected"
diff $ACTUAL $EXPECTED
if [ $? == 0 ]; then
    echo "Success"
    exit 0
else
    echo "Failed"
    exit 1
fi
