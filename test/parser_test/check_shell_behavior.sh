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
cat | cat | ls '>'
# ls は失敗するがその後の cat は実行される(2 回エンターを押すと終了する)
#ls: cannot access '>': No such file or directory
#

true || echo heelo | ls > abc
#true が評価されるので後続は処理されない
false && (echo heelo | ls > abc)
#false が評価されるので後続は処理されない

(unset PATH) && cat Makefile
#subshell の中の実行になるため、cat に影響を与えない

(unset PATH && cat Makefile) 
#cat はunset path されているので実行できない

(unset PATH && cat Makefile) && echo "hello"
#echo は実行されない。


(unset PATH && cat Makefile) 2>b || echo hello
#b にエラー結果が出力され、hello も出力される

(unset PATH && cat Makefile) 2>b || echo hello>>b | < b cat
#b のエラーが出力されたあとに、エラーが出る。
#bash: cat: No such file or directory
#hello

(unset PATH && cat Makefile) 2>b && echo hello>>b | < b cat
#エラー内容が b に書き込まれるだけ

(unset PATH && cat Makefile)| < /dev/stdin cat | cat | ls
#bash: cat: No such file or directory
#includes  libft  Makefile  srcs  test  tests

(false || false && true  |  echo "inside") | ls
#inside は実行されない
#ls は実行される

(sleep 5 && true | echo "inside") | cat
#5 秒後に cat が実行され、inside が表示される

simple | > a
#Command 'simple' not found, but can be installed with:
#apt install meryl
#Exception ignored in: <_io.TextIOWrapper name='<stdout>' mode='w' encoding='utf-8'>
#BrokenPipeError: [Errno 32] Broken pipe
#上記のエラーが出力される。なお、a は作成されて、echo $? の結果は 0 

echo hello | cat && false | ls
#　hello が出力され、ls も出力される。
#hello
#includes  libft  Makefile  srcs  test  tests

echo hello | (cat && false) | ls
#echo の結果は出力されない
#includes  libft  Makefile  srcs  test  tests

echo hello | cat | (cat && false) | > c ls > d > e
# e に ls の出力結果がコピーされる

 echo hello | cat | cat && false | > c ls > d > e
#hello が表示され、 e に ls の出力結果がコピーされる

echo hello | cat | cat || false | > c sleep 3 > d > e
#hello が表示されるが、false 以降のコマンドは実行されない
下記で　pipe の各コマンドのステータス確認可能
echo "cmd1: ${PIPESTATUS[0]}" echo "cmd2: ${PIPESTATUS[1]}" echo "cmd3: ${PIPESTATUS[2]}"
or 
echo "cmds: ${PIPESTATUS[@]}"

echo hello | (cat | cat) | false | > c sleep 3 > d > e


#<<e
#heredoc が始まる。エラーは生じない
#<e <<e
#heredoc が始まる。heredoc 終了後に、e not found が投げられる。

#<e << asdf | cat

#heredoc が始まる。heredoc 終了後に、e not found が投げられる。　cat は正常終了する
# echo "cmds: ${PIPESTATUS[@]}"
#cmds: 0 0

#e file が存在しない場合、out は出力されない
#<e<<eof>out

#sleep 4 && <<eof cat
#heredoc が実行されたあと、sleep　が実行され、cat が実行される

sleep 4 || <<eof cat