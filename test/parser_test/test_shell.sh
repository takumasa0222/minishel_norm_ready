#!/bin/bash

#echo "hello" && (echo "this is test" || cat || cat || ls > ./out2)
#実行結果 
#hello 
#this is test と表示されるが、out2 は出力されない

#echo "hello" || (echo "this is test" || cat || cat || ls > ./out2)
#実行結果 
#hello と表示されるのみ
#(echo "this is test" | cat | cat | ls > ./out2) && echo "hello"

#(echo "this is test" | cat | cat | ls > ./out2) && echo "hello"||
#syntax error

#(echo "this is test" | cat | cat | ls > ./out2) && echo "hello" >
#syntax error

#(echo "this is test" | cat | cat | ls > ./out2) && echo "hello" >>
#syntax error

# bash 上動作はする。
#<< cat | ls

cat | cat | ls > a >b >c
# a と　b には何もコピーされず、 c にだけコピーが発生する



