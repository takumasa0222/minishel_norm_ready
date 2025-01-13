#!/bin/bash

source ./combination_test/test_assert.sh

# redirection
assert echo "Hello Builtin" > out_builtin.txt
assert cat out_builtin.txt

assert echo "First line" > out_builtin.txt
assert echo "Second line" >> out_builtin.txt
assert cat out_builtin.txt

assert echo "line1" > in_builtin.txt
assert echo "line2" >> in_builtin.txt
assert cat < in_builtin.txt

assert echo "Testing" > out_builtin.txt
assert pwd > out_builtin.txt
assert cat out_builtin.txt

assert echo "Hello Pipe" | cat > out_pipe.txt
assert cat out_pipe.txt

assert echo "Line A" > in_pipe.txt
assert echo "Line B" >> in_pipe.txt
assert cat < in_pipe.txt | grep "B"

assert echo "1 2 3" | tr ' ' '\n' >> out_pipe2.txt
assert cat out_pipe2.txt
assert echo "Hello AND" > out_and.txt && cat out_and.txt

assert nosuchcmd > out_err.txt && echo "OK"
assert cat < no_such_file.txt && echo "This should not appear"

nosuchcmd > out_or.txt || echo "Fallback" 
# => "Fallback" が表示される
# out_or.txt がどうなるか(空ファイルなど)も確認

assert echo "A B C" > out_ok.txt || echo "This should not appear"
assert cat out_ok.txt

assert echo "OK" > in_ok.txt
assert cat < in_ok.txt || echo "This should not appear"