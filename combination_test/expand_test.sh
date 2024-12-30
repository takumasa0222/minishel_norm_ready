#!/bin/bash

source ./test_assert.sh

assert 'echo $USER | cat'
assert 'echo $USER$HOME | cat'
assert 'echo $USERA | cat'
assert "echo $USER'A' | cat"
#assert 'echo $USER | cat'
