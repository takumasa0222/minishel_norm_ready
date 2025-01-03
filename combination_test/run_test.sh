#!/bin/bash

make
if [ $? -ne 0 ]; then
	echo "make is failed"
	exit 1
else 
	cd combination_test || { echo "Failed to change directory to combination_test"; exit 1; }
	source ./test_assert.sh || { echo "Failed to source test_assert.sh"; exit 1; }
	echo "expand test"
	./expand_test.sh
	echo "builtin test"
	./builtin_test.sh
	echo "test is completed"
	cleanup || { echo "cleanup failed"; exit 1; }
fi