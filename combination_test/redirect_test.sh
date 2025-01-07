#!/bin/bash

source ./combination_test/test_assert.sh

assert 'echo $USER << abc | cat'
assert 'cat << end Makefile'
# assert 'echo $USER$HOME | cat'
# assert 'echo $USERA | cat'
# assert "echo $USER'A' | cat"
# assert "echo $USER$123 | cat"
# assert "echo $USER$ | cat"
# assert "echo $USER'$'$USER | cat"
# assert "echo $USE'R' | cat"
# assert "echo $USER$ | cat"
# assert "echo $USER$ | cat"