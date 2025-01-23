# minishell FB 1/19
# commitid : f850b542484c08e688a80a9a3d316e67cec50877
# How to FB
```bash
$ valgrind --leak-check=full --trace-children=yes --track-fds=yes ./minishell
```
# Order to fix
## 1. crash
## 2. memory leaks
## 3.(option) fd leaks

# agenda:
## process minishell
### cmd excute
#### relative path
```bash
minishell$ ls
==2102562== Memcheck, a memory error detector
==2102562== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2102562== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2102562== Command: /usr/bin/ls
==2102562== 
 Makefile	    get_next_line   ls_test.c   out1   out4	 test
 combination_test   includes	    minishell   out2   outfile	 tests
 fb_kfukuhar.md     libft	    out.txt     out3   srcs	'tree -p 2102277'
==2102562== 
==2102562== FILE DESCRIPTORS: 3 open (1 std) at exit.
==2102562== Open file descriptor 4: /dev/pts/3
==2102562==    <inherited from parent>
==2102562== 
==2102562== Open file descriptor 3: /dev/pts/3
==2102562==    <inherited from parent>
==2102562== 
==2102562==
``` 
#### absolute path
same above
#### file path does not exist
```
minishell$ aaaa
aaaa: No such file or directory
==2102614== 
==2102614== FILE DESCRIPTORS: 5 open (3 std) at exit.
==2102614== Open file descriptor 4: /dev/pts/3
==2102614==    at 0x49D8FEB: dup (syscall-template.S:120)
==2102614==    by 0x407F98: backup_std_fds (execute_builtin.c:38)
==2102614==    by 0x4016A1: clear_ctx (main.c:138)
==2102614==    by 0x4015A2: start_exec (main.c:83)
==2102614==    by 0x40146E: main (main.c:68)
==2102614== 
==2102614== Open file descriptor 3: /dev/pts/3
==2102614==    at 0x49D8FEB: dup (syscall-template.S:120)
==2102614==    by 0x407F82: backup_std_fds (execute_builtin.c:37)
==2102614==    by 0x4016A1: clear_ctx (main.c:138)
==2102614==    by 0x4015A2: start_exec (main.c:83)
==2102614==    by 0x40146E: main (main.c:68)
==2102614== 
==2102614== 
==2102614== HEAP SUMMARY:
==2102614==     in use at exit: 232,469 bytes in 442 blocks
==2102614==   total heap usage: 922 allocs, 480 frees, 262,946 bytes allocated
==2102614== 
==2102614== 147 (96 direct, 51 indirect) bytes in 4 blocks are definitely lost in loss record 38 of 87
==2102614==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==2102614==    by 0x4017E8: create_token (lexer_helper.c:34)
==2102614==    by 0x401F50: fetch_fst_word_token (lexer.c:81)
==2102614==    by 0x401D15: lexer (lexer.c:42)
==2102614==    by 0x401588: start_exec (main.c:81)
==2102614==    by 0x40146E: main (main.c:68)
==2102614== 
==2102614== LEAK SUMMARY:
==2102614==    definitely lost: 96 bytes in 4 blocks
==2102614==    indirectly lost: 51 bytes in 6 blocks
==2102614==      possibly lost: 0 bytes in 0 blocks
==2102614==    still reachable: 232,322 bytes in 432 blocks
==2102614==         suppressed: 0 bytes in 0 blocks
==2102614== Reachable blocks (those to which a pointer was found) are not shown.
==2102614== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==2102614== 
==2102614== For lists of detected and suppressed errors, rerun with: -s
==2102614== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```
#### file path has no permission
```
bash$ touch aaaa && chmod 000 aaaa
minishell$ unset PATH
minishell$ /home/ssoeno/minishell/norm_ready/aaaa
/home/ssoeno/minishell/norm_ready/aaaa: Permission denied
==2102662== 
==2102662== FILE DESCRIPTORS: 5 open (3 std) at exit.
==2102662== Open file descriptor 4: /dev/pts/3
==2102662==    at 0x49D8FEB: dup (syscall-template.S:120)
==2102662==    by 0x407F98: backup_std_fds (execute_builtin.c:38)
==2102662==    by 0x4016A1: clear_ctx (main.c:138)
==2102662==    by 0x4015A2: start_exec (main.c:83)
==2102662==    by 0x40146E: main (main.c:68)
==2102662== 
==2102662== Open file descriptor 3: /dev/pts/3
==2102662==    at 0x49D8FEB: dup (syscall-template.S:120)
==2102662==    by 0x407F82: backup_std_fds (execute_builtin.c:37)
==2102662==    by 0x4016A1: clear_ctx (main.c:138)
==2102662==    by 0x4015A2: start_exec (main.c:83)
==2102662==    by 0x40146E: main (main.c:68)
==2102662== 
==2102662== 
==2102662== HEAP SUMMARY:
==2102662==     in use at exit: 232,460 bytes in 437 blocks
==2102662==   total heap usage: 935 allocs, 498 frees, 429,336 bytes allocated
==2102662== 
==2102662== 177 (96 direct, 81 indirect) bytes in 4 blocks are definitely lost in loss record 36 of 82
==2102662==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==2102662==    by 0x4017E8: create_token (lexer_helper.c:34)
==2102662==    by 0x401F50: fetch_fst_word_token (lexer.c:81)
==2102662==    by 0x401D15: lexer (lexer.c:42)
==2102662==    by 0x401588: start_exec (main.c:81)
==2102662==    by 0x40146E: main (main.c:68)
==2102662== 
==2102662== LEAK SUMMARY:
==2102662==    definitely lost: 96 bytes in 4 blocks
==2102662==    indirectly lost: 81 bytes in 6 blocks
==2102662==      possibly lost: 0 bytes in 0 blocks
==2102662==    still reachable: 232,283 bytes in 427 blocks
==2102662==         suppressed: 0 bytes in 0 blocks
==2102662== Reachable blocks (those to which a pointer was found) are not shown.
==2102662== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==2102662== 
==2102662== For lists of detected and suppressed errors, rerun with: -s
==2102662== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```
#### sigle excute after unsetting PATH
```bash
c6r3s14% valgrind --leak-check=full --trace-children=yes --track-fds=yes ./minishell
==2102649== Memcheck, a memory error detector
==2102649== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2102649== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2102649== Command: ./minishell
==2102649== 
minishell$ unset PATH
minishell$ ls
ls: No such file or directory
==2102653== 
==2102653== FILE DESCRIPTORS: 5 open (3 std) at exit.
==2102653== Open file descriptor 4: /dev/pts/3
==2102653==    at 0x49D8FEB: dup (syscall-template.S:120)
==2102653==    by 0x407F98: backup_std_fds (execute_builtin.c:38)
==2102653==    by 0x4016A1: clear_ctx (main.c:138)
==2102653==    by 0x4015A2: start_exec (main.c:83)
==2102653==    by 0x40146E: main (main.c:68)
==2102653== 
==2102653== Open file descriptor 3: /dev/pts/3
==2102653==    at 0x49D8FEB: dup (syscall-template.S:120)
==2102653==    by 0x407F82: backup_std_fds (execute_builtin.c:37)
==2102653==    by 0x4016A1: clear_ctx (main.c:138)
==2102653==    by 0x4015A2: start_exec (main.c:83)
==2102653==    by 0x40146E: main (main.c:68)
==2102653== 
==2102653== 
==2102653== HEAP SUMMARY:
==2102653==     in use at exit: 232,182 bytes in 427 blocks
==2102653==   total heap usage: 781 allocs, 354 frees, 260,716 bytes allocated
==2102653== 
==2102653== 86 (48 direct, 38 indirect) bytes in 2 blocks are definitely lost in loss record 30 of 82
==2102653==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==2102653==    by 0x4017E8: create_token (lexer_helper.c:34)
==2102653==    by 0x401F50: fetch_fst_word_token (lexer.c:81)
==2102653==    by 0x401D15: lexer (lexer.c:42)
==2102653==    by 0x401588: start_exec (main.c:81)
==2102653==    by 0x40146E: main (main.c:68)
==2102653== 
==2102653== LEAK SUMMARY:
==2102653==    definitely lost: 48 bytes in 2 blocks
==2102653==    indirectly lost: 38 bytes in 4 blocks
==2102653==      possibly lost: 0 bytes in 0 blocks
==2102653==    still reachable: 232,096 bytes in 421 blocks
==2102653==         suppressed: 0 bytes in 0 blocks
==2102653== Reachable blocks (those to which a pointer was found) are not shown.
==2102653== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==2102653== 
==2102653== For lists of detected and suppressed errors, rerun with: -s
==2102653== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```
### pipe
#### multi cmd excute
```bash
c6r3s14% valgrind --leak-check=full --trace-children=yes --track-fds=yes ./minishell
==2102699== Memcheck, a memory error detector
==2102699== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2102699== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2102699== Command: ./minishell
==2102699== 
minishell$ /bin/ls | wc -l
==2102723== Memcheck, a memory error detector
==2102723== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2102723== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2102723== Command: /bin/ls
==2102723== 
==2102724== Memcheck, a memory error detector
==2102724== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2102724== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2102724== Command: /usr/bin/wc -l
==2102724== 
==2102723== 
==2102723== FILE DESCRIPTORS: 3 open (1 std) at exit.
==2102723== Open file descriptor 4: /dev/pts/3
==2102723==    <inherited from parent>
==2102723== 
==2102723== Open file descriptor 3: /dev/pts/3
==2102723==    <inherited from parent>
==2102723== 
==2102723== 
==2102723== HEAP SUMMARY:
==2102723==     in use at exit: 20,859 bytes in 24 blocks
==2102723==   total heap usage: 258 allocs, 234 frees, 80,580 bytes allocated
==2102723== 
==2102723== LEAK SUMMARY:
==2102723==    definitely lost: 0 bytes in 0 blocks
==2102723==    indirectly lost: 0 bytes in 0 blocks
==2102723==      possibly lost: 0 bytes in 0 blocks
==2102723==    still reachable: 20,859 bytes in 24 blocks
==2102723==         suppressed: 0 bytes in 0 blocks
==2102723== Reachable blocks (those to which a pointer was found) are not shown.
==2102723== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==2102723== 
==2102723== For lists of detected and suppressed errors, rerun with: -s
==2102723== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
19
==2102724== 
==2102724== FILE DESCRIPTORS: 2 open (0 std) at exit.
==2102724== Open file descriptor 4: /dev/pts/3
==2102724==    <inherited from parent>
==2102724== 
==2102724== Open file descriptor 3: /dev/pts/3
==2102724==    <inherited from parent>
==2102724== 
==2102724== 
==2102724== HEAP SUMMARY:
==2102724==     in use at exit: 0 bytes in 0 blocks
==2102724==   total heap usage: 242 allocs, 242 frees, 23,011 bytes allocated
==2102724== 
==2102724== All heap blocks were freed -- no leaks are possible
==2102724== 
==2102724== For lists of detected and suppressed errors, rerun with: -s
==2102724== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
### redirect
#### only heredoc
```bash
minishell$ << eof
> aaaa
> fjeakljfaleks
> 
> 
> 
> eof
minishell$ exit
==2102699== 
==2102699== FILE DESCRIPTORS: 5 open (3 std) at exit.
==2102699== Open file descriptor 4: /dev/pts/3
==2102699==    at 0x49D8FEB: dup (syscall-template.S:120)
==2102699==    by 0x407F98: backup_std_fds (execute_builtin.c:38)
==2102699==    by 0x4016A1: clear_ctx (main.c:138)
==2102699==    by 0x4015A2: start_exec (main.c:83)
==2102699==    by 0x40146E: main (main.c:68)
==2102699== 
==2102699== Open file descriptor 3: /dev/pts/3
==2102699==    at 0x49D8FEB: dup (syscall-template.S:120)
==2102699==    by 0x407F82: backup_std_fds (execute_builtin.c:37)
==2102699==    by 0x4016A1: clear_ctx (main.c:138)
==2102699==    by 0x4015A2: start_exec (main.c:83)
==2102699==    by 0x40146E: main (main.c:68)
==2102699== 
==2102699== 
==2102699== HEAP SUMMARY:
==2102699==     in use at exit: 232,234 bytes in 438 blocks
==2102699==   total heap usage: 847 allocs, 409 frees, 261,475 bytes allocated
==2102699== 
==2102699== 55 (24 direct, 31 indirect) bytes in 1 blocks are definitely lost in loss record 26 of 82
==2102699==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==2102699==    by 0x4017E8: create_token (lexer_helper.c:34)
==2102699==    by 0x401C1B: fetch_fst_ope_token (lexer_operator_checker.c:56)
==2102699==    by 0x401CE6: lexer (lexer.c:40)
==2102699==    by 0x401588: start_exec (main.c:81)
==2102699==    by 0x40146E: main (main.c:68)
==2102699== 
==2102699== 141 (48 direct, 93 indirect) bytes in 2 blocks are definitely lost in loss record 34 of 82
==2102699==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==2102699==    by 0x4017E8: create_token (lexer_helper.c:34)
==2102699==    by 0x401F50: fetch_fst_word_token (lexer.c:81)
==2102699==    by 0x401D15: lexer (lexer.c:42)
==2102699==    by 0x401588: start_exec (main.c:81)
==2102699==    by 0x40146E: main (main.c:68)
==2102699== 
==2102699== LEAK SUMMARY:
==2102699==    definitely lost: 72 bytes in 3 blocks
==2102699==    indirectly lost: 124 bytes in 11 blocks
==2102699==      possibly lost: 0 bytes in 0 blocks
==2102699==    still reachable: 232,038 bytes in 424 blocks
==2102699==         suppressed: 0 bytes in 0 blocks
==2102699== Reachable blocks (those to which a pointer was found) are not shown.
==2102699== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==2102699== 
==2102699== For lists of detected and suppressed errors, rerun with: -s
==2102699== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
```
#### <
```bash
minishell$ cat < Makefile 
==2102895== Memcheck, a memory error detector
==2102895== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2102895== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2102895== Command: /usr/bin/cat
==2102895== 
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/25 21:34:48 by shokosoeno        #+#    #+#              #
#    Updated: 2025/01/19 15:28:29 by ssoeno           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Configure settings according to the OS
UNAME_S := $(shell uname -s)

NAME = minishell
CC = $(if $(findstring Darwin, $(UNAME_S)), cc, clang)
INCLUDES = -I includes -I $(LIBFT_DIR)
CFLAGS = -Wall -Wextra -Werror -g $(INCLUDES)

ifeq ($(UNAME_S),Darwin)
	# for macOS
	RLDIR := $(shell brew --prefix readline)
	LIBS := -L$(RLDIR)/lib -lreadline
	INCLUDES += -I$(RLDIR)/include
else
	# for Linux
	LIBS := -lreadline
endif

# add libft
LIBFT_DIR = ./libft
LIBFT = ${LIBFT_DIR}/libft.a

SRCS = srcs/main.c \
srcs/lexer/lexer_helper.c \
srcs/lexer/lexer_token_checker.c \
srcs/lexer/lexer_token_quote_checker.c \
srcs/lexer/lexer_operator_checker.c \
srcs/lexer/lexer.c \
srcs/parser/parser.c \
srcs/parser/parser_cmd_tail.c \
srcs/parser/parser_redirect_word.c \
srcs/parser/parser_token_checker.c \
srcs/parser/parser_node_operator.c \
srcs/parser/parser_node_counter.c \
srcs/utils/error.c \
srcs/utils/free_functions.c \
srcs/utils/ft_strcmp.c \
srcs/utils/init_env.c \
srcs/utils/xmalloc.c \
srcs/expand/expand_handler.c \
srcs/expand/expand_helper.c \
srcs/expand/expand_variable_helper.c \
srcs/expand/expand_variable.c \
srcs/expand/remove_quotes.c \
srcs/builtin/builtin_cd.c \
srcs/builtin/builtin_cd_utils.c \
srcs/builtin/builtin_exit.c \
srcs/builtin/builtin_pwd.c \
srcs/builtin/builtin_export.c \
srcs/builtin/builtin_export_utils.c \
srcs/builtin/builtin_unset.c \
srcs/builtin/builtin_echo.c \
srcs/builtin/builtin_env.c \
srcs/builtin/lookup_builtin.c \
srcs/map/item_utils.c \
srcs/map/map_get.c \
srcs/map/map_new.c \
srcs/map/map_put.c \
srcs/map/map_set.c \
srcs/map/map_size.c \
srcs/map/map_unset.c \
srcs/signal/signal.c \
srcs/signal/signal_hook.c \
srcs/execution/execute_fd_controller.c \
srcs/execution/execute_logical_operator.c \
srcs/execution/execute_utils.c \
srcs/execution/execute.c \
srcs/execution/execute_external.c \
srcs/execution/execute_builtin.c \
srcs/execution/execute_error.c \
srcs/execution/execute_search_path_env.c \
srcs/heredoc/heredoc.c \
srcs/heredoc/heredoc_read.c \
srcs/redirect/redirect.c \
srcs/redirect/redirect_utils.c \
get_next_line/get_next_line.c \
get_next_line/get_next_line_utils.c 


OBJS = $(SRCS:.c=.o)

# General rules

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

test: all
	./test.sh

.PHONY: all clean fclean re test==2102895== 
==2102895== FILE DESCRIPTORS: 2 open (0 std) at exit.
==2102895== Open file descriptor 4: /dev/pts/3
==2102895==    <inherited from parent>
==2102895== 
==2102895== Open file descriptor 3: /dev/pts/3
==2102895==    <inherited from parent>
==2102895== 
==2102895== 
==2102895== HEAP SUMMARY:
==2102895==     in use at exit: 0 bytes in 0 blocks
==2102895==   total heap usage: 228 allocs, 228 frees, 156,326 bytes allocated
==2102895== 
==2102895== All heap blocks were freed -- no leaks are possible
==2102895== 
==2102895== For lists of detected and suppressed errors, rerun with: -s
==2102895== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
#### >
```bash

```
#### <<
#### >>
#### multi redirects
```bash
c6r3s14% valgrind --leak-check=full --trace-children=yes --track-fds=yes ./minishell
==2102857== Memcheck, a memory error detector
==2102857== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2102857== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2102857== Command: ./minishell
==2102857== 
minishell$ cat << eof << eof2
> this is first
> eof
> this is second
> eof2
==2102865== Memcheck, a memory error detector
==2102865== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2102865== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2102865== Command: /usr/bin/cat
==2102865== 
this is second
==2102865== 
==2102865== FILE DESCRIPTORS: 4 open (0 std) at exit.
==2102865== Open file descriptor 6:
==2102865==    <inherited from parent>
==2102865== 
==2102865== Open file descriptor 5:
==2102865==    <inherited from parent>
==2102865== 
==2102865== Open file descriptor 4: /dev/pts/3
==2102865==    <inherited from parent>
==2102865== 
==2102865== Open file descriptor 3: /dev/pts/3
==2102865==    <inherited from parent>
==2102865== 
==2102865== 
==2102865== HEAP SUMMARY:
==2102865==     in use at exit: 0 bytes in 0 blocks
==2102865==   total heap usage: 228 allocs, 228 frees, 156,326 bytes allocated
==2102865== 
==2102865== All heap blocks were freed -- no leaks are possible
==2102865== 
==2102865== For lists of detected and suppressed errors, rerun with: -s
==2102865== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
#### redirect file permission error
```bash
c6r3s14% valgrind --leak-check=full --trace-children=yes --track-fds=yes ./minishell
==2103098== Memcheck, a memory error detector
==2103098== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2103098== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2103098== Command: ./minishell
==2103098== 
minishell$ cat < aaaa
Error: function name: redirect_in, Error message: Permission denied
==2103102== 
==2103102== FILE DESCRIPTORS: 5 open (3 std) at exit.
==2103102== Open file descriptor 4: /dev/pts/3
==2103102==    at 0x49D8FEB: dup (syscall-template.S:120)
==2103102==    by 0x407F98: backup_std_fds (execute_builtin.c:38)
==2103102==    by 0x4016A1: clear_ctx (main.c:138)
==2103102==    by 0x4015A2: start_exec (main.c:83)
==2103102==    by 0x40146E: main (main.c:68)
==2103102== 
==2103102== Open file descriptor 3: /dev/pts/3
==2103102==    at 0x49D8FEB: dup (syscall-template.S:120)
==2103102==    by 0x407F82: backup_std_fds (execute_builtin.c:37)
==2103102==    by 0x4016A1: clear_ctx (main.c:138)
==2103102==    by 0x4015A2: start_exec (main.c:83)
==2103102==    by 0x40146E: main (main.c:68)
==2103102== 
==2103102== 
==2103102== HEAP SUMMARY:
==2103102==     in use at exit: 232,124 bytes in 428 blocks
==2103102==   total heap usage: 786 allocs, 358 frees, 294,417 bytes allocated
==2103102== 
==2103102== 83 (24 direct, 59 indirect) bytes in 1 blocks are definitely lost in loss record 34 of 85
==2103102==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==2103102==    by 0x4017E8: create_token (lexer_helper.c:34)
==2103102==    by 0x401F50: fetch_fst_word_token (lexer.c:81)
==2103102==    by 0x401D15: lexer (lexer.c:42)
==2103102==    by 0x401588: start_exec (main.c:81)
==2103102==    by 0x40146E: main (main.c:68)
==2103102== 
==2103102== LEAK SUMMARY:
==2103102==    definitely lost: 24 bytes in 1 blocks
==2103102==    indirectly lost: 59 bytes in 5 blocks
==2103102==      possibly lost: 0 bytes in 0 blocks
==2103102==    still reachable: 232,041 bytes in 422 blocks
==2103102==         suppressed: 0 bytes in 0 blocks
==2103102== Reachable blocks (those to which a pointer was found) are not shown.
==2103102== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==2103102== 
==2103102== For lists of detected and suppressed errors, rerun with: -s
==2103102== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
minishell$ cat Makefile > aaaa
Error: function name: redirect_out, Error message: Permission denied
==2103141== 
==2103141== FILE DESCRIPTORS: 5 open (3 std) at exit.
==2103141== Open file descriptor 4: /dev/pts/3
==2103141==    at 0x49D8FEB: dup (syscall-template.S:120)
==2103141==    by 0x407F98: backup_std_fds (execute_builtin.c:38)
==2103141==    by 0x4016A1: clear_ctx (main.c:138)
==2103141==    by 0x4015A2: start_exec (main.c:83)
==2103141==    by 0x40146E: main (main.c:68)
==2103141== 
==2103141== Open file descriptor 3: /dev/pts/3
==2103141==    at 0x49D8FEB: dup (syscall-template.S:120)
==2103141==    by 0x407F82: backup_std_fds (execute_builtin.c:37)
==2103141==    by 0x4016A1: clear_ctx (main.c:138)
==2103141==    by 0x4015A2: start_exec (main.c:83)
==2103141==    by 0x40146E: main (main.c:68)
==2103141== 
==2103141== 
==2103141== HEAP SUMMARY:
==2103141==     in use at exit: 232,593 bytes in 445 blocks
==2103141==   total heap usage: 846 allocs, 401 frees, 328,423 bytes allocated
==2103141== 
==2103141== 199 (48 direct, 151 indirect) bytes in 2 blocks are definitely lost in loss record 47 of 93
==2103141==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==2103141==    by 0x4017E8: create_token (lexer_helper.c:34)
==2103141==    by 0x401F50: fetch_fst_word_token (lexer.c:81)
==2103141==    by 0x401D15: lexer (lexer.c:42)
==2103141==    by 0x401588: start_exec (main.c:81)
==2103141==    by 0x40146E: main (main.c:68)
==2103141== 
==2103141== LEAK SUMMARY:
==2103141==    definitely lost: 48 bytes in 2 blocks
==2103141==    indirectly lost: 151 bytes in 12 blocks
==2103141==      possibly lost: 0 bytes in 0 blocks
==2103141==    still reachable: 232,394 bytes in 431 blocks
==2103141==         suppressed: 0 bytes in 0 blocks
==2103141== Reachable blocks (those to which a pointer was found) are not shown.
==2103141== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==2103141== 
==2103141== For lists of detected and suppressed errors, rerun with: -s
==2103141== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```
#### refirect file nosuch file
```bash
minishell$ cat < nosuchfile
Error: function name: redirect_in, Error message: Permission denied
==2103166== 
==2103166== FILE DESCRIPTORS: 5 open (3 std) at exit.
==2103166== Open file descriptor 4: /dev/pts/3
==2103166==    at 0x49D8FEB: dup (syscall-template.S:120)
==2103166==    by 0x407F98: backup_std_fds (execute_builtin.c:38)
==2103166==    by 0x4016A1: clear_ctx (main.c:138)
==2103166==    by 0x4015A2: start_exec (main.c:83)
==2103166==    by 0x40146E: main (main.c:68)
==2103166== 
==2103166== Open file descriptor 3: /dev/pts/3
==2103166==    at 0x49D8FEB: dup (syscall-template.S:120)
==2103166==    by 0x407F82: backup_std_fds (execute_builtin.c:37)
==2103166==    by 0x4016A1: clear_ctx (main.c:138)
==2103166==    by 0x4015A2: start_exec (main.c:83)
==2103166==    by 0x40146E: main (main.c:68)
==2103166== 
==2103166== 
==2103166== HEAP SUMMARY:
==2103166==     in use at exit: 232,148 bytes in 428 blocks
==2103166==   total heap usage: 765 allocs, 337 frees, 261,274 bytes allocated
==2103166== 
==2103166== 89 (24 direct, 65 indirect) bytes in 1 blocks are definitely lost in loss record 34 of 85
==2103166==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==2103166==    by 0x4017E8: create_token (lexer_helper.c:34)
==2103166==    by 0x401F50: fetch_fst_word_token (lexer.c:81)
==2103166==    by 0x401D15: lexer (lexer.c:42)
==2103166==    by 0x401588: start_exec (main.c:81)
==2103166==    by 0x40146E: main (main.c:68)
==2103166== 
==2103166== LEAK SUMMARY:
==2103166==    definitely lost: 24 bytes in 1 blocks
==2103166==    indirectly lost: 65 bytes in 5 blocks
==2103166==      possibly lost: 0 bytes in 0 blocks
==2103166==    still reachable: 232,059 bytes in 422 blocks
==2103166==         suppressed: 0 bytes in 0 blocks
==2103166== Reachable blocks (those to which a pointer was found) are not shown.
==2103166== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==2103166== 
==2103166== For lists of detected and suppressed errors, rerun with: -s
==2103166== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```
### signal
#### empty prompt
```bash
c6r3s14% valgrind --leak-check=full --trace-children=yes --track-fds=yes ./minishell
==2103187== Memcheck, a memory error detector
==2103187== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2103187== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2103187== Command: ./minishell
==2103187== 
minishell$ ^C
minishell$ ^C
minishell$ ^C
minishell$ ^C
minishell$ ^C
minishell$ ^C
minishell$ ## Ctrl + D(EOF) 
==2103187== 
==2103187== FILE DESCRIPTORS: 3 open (3 std) at exit.
==2103187== 
==2103187== HEAP SUMMARY:
==2103187==     in use at exit: 227,780 bytes in 409 blocks
==2103187==   total heap usage: 767 allocs, 358 frees, 256,040 bytes allocated
==2103187== 
==2103187== 23,621 (20,048 direct, 3,573 indirect) bytes in 1 blocks are definitely lost in loss record 66 of 67
==2103187==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==2103187==    by 0x4014B1: init_ctx (main.c:111)
==2103187==    by 0x4012EE: main (main.c:31)
==2103187== 
==2103187== LEAK SUMMARY:
==2103187==    definitely lost: 20,048 bytes in 1 blocks
==2103187==    indirectly lost: 3,573 bytes in 187 blocks
==2103187==      possibly lost: 0 bytes in 0 blocks
==2103187==    still reachable: 204,159 bytes in 221 blocks
==2103187==         suppressed: 0 bytes in 0 blocks
==2103187== Reachable blocks (those to which a pointer was found) are not shown.
==2103187== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==2103187== 
==2103187== For lists of detected and suppressed errors, rerun with: -s
==2103187== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```
#### during prompt
OK!
#### during excute cmd
```bash
c6r3s14% valgrind --leak-check=full --trace-children=yes --track-fds=yes ./minishell
==2103225== Memcheck, a memory error detector
==2103225== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2103225== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2103225== Command: ./minishell
==2103225== 
minishell$ sleep 10
==2103239== Memcheck, a memory error detector
==2103239== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2103239== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2103239== Command: /usr/bin/sleep 10
==2103239== 
^C==2103239== 
==2103239== Process terminating with default action of signal 2 (SIGINT)
==2103239==    at 0x495578A: clock_nanosleep@@GLIBC_2.17 (clock_nanosleep.c:78)
==2103239==    by 0x495A676: nanosleep (nanosleep.c:25)
==2103239==    by 0x10A9FF: ??? (in /usr/bin/sleep)
==2103239==    by 0x4899D8F: (below main) (libc_start_call_main.h:58)
==2103239== 
==2103239== FILE DESCRIPTORS: 5 open (3 std) at exit.
==2103239== Open file descriptor 4: /dev/pts/3
==2103239==    <inherited from parent>
==2103239== 
==2103239== Open file descriptor 3: /dev/pts/3
==2103239==    <inherited from parent>
==2103239== 
==2103239== 
==2103239== HEAP SUMMARY:
==2103239==     in use at exit: 13,422 bytes in 145 blocks
==2103239==   total heap usage: 227 allocs, 82 frees, 21,159 bytes allocated
==2103239== 
==2103239== LEAK SUMMARY:
==2103239==    definitely lost: 0 bytes in 0 blocks
==2103239==    indirectly lost: 0 bytes in 0 blocks
==2103239==      possibly lost: 0 bytes in 0 blocks
==2103239==    still reachable: 13,422 bytes in 145 blocks
==2103239==         suppressed: 0 bytes in 0 blocks
==2103239== Reachable blocks (those to which a pointer was found) are not shown.
==2103239== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==2103239== 
==2103239== For lists of detected and suppressed errors, rerun with: -s
==2103239== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
#### during excute blocking
OK!
ex) catコマンドの実行で標準入力を待っているときにSIGINT等
#### during heredoc
- empty prompt : ok!
- during prompt : 
```bash
minishell$ cat << eof
> aaaa^C
==2103354== Warning: invalid file descriptor -2 in syscall close()
==2103422== Memcheck, a memory error detector
==2103422== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2103422== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2103422== Command: /usr/bin/cat
==2103422== 
==2103422== 
==2103422== FILE DESCRIPTORS: 4 open (0 std) at exit.
==2103422== Open file descriptor 6:
==2103422==    <inherited from parent>
==2103422== 
==2103422== Open file descriptor 5:
==2103422==    <inherited from parent>
==2103422== 
==2103422== Open file descriptor 4: /dev/pts/3
==2103422==    <inherited from parent>
==2103422== 
==2103422== Open file descriptor 3: /dev/pts/3
==2103422==    <inherited from parent>
==2103422== 
==2103422== 
==2103422== HEAP SUMMARY:
==2103422==     in use at exit: 0 bytes in 0 blocks
==2103422==   total heap usage: 228 allocs, 228 frees, 156,326 bytes allocated
==2103422== 
==2103422== All heap blocks were freed -- no leaks are possible
==2103422== 
==2103422== For lists of detected and suppressed errors, rerun with: -s
==2103422== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
- at least already input 1 line :
```bash
minishell$ cat << eof
> aaaa
> ^C
==2103354== Warning: invalid file descriptor -2 in syscall close()
==2103441== Memcheck, a memory error detector
==2103441== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2103441== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2103441== Command: /usr/bin/cat
==2103441== 
==2103441== 
==2103441== FILE DESCRIPTORS: 5 open (0 std) at exit.
==2103441== Open file descriptor 7:
==2103441==    <inherited from parent>
==2103441== 
==2103441== Open file descriptor 6:
==2103441==    <inherited from parent>
==2103441== 
==2103441== Open file descriptor 5:
==2103441==    <inherited from parent>
==2103441== 
==2103441== Open file descriptor 4: /dev/pts/3
==2103441==    <inherited from parent>
==2103441== 
==2103441== Open file descriptor 3: /dev/pts/3
==2103441==    <inherited from parent>
==2103441== 
==2103441== 
==2103441== HEAP SUMMARY:
==2103441==     in use at exit: 0 bytes in 0 blocks
==2103441==   total heap usage: 228 allocs, 228 frees, 156,326 bytes allocated
==2103441== 
==2103441== All heap blocks were freed -- no leaks are possible
==2103441== 
==2103441== For lists of detected and suppressed errors, rerun with: -s
==2103441== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
minishell$ cat << eof
> aaaa
> 
==2103442== Memcheck, a memory error detector
==2103442== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2103442== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2103442== Command: /usr/bin/cat
==2103442== 
aaaa
==2103442== 
==2103442== FILE DESCRIPTORS: 6 open (0 std) at exit.
==2103442== Open file descriptor 8:
==2103442==    <inherited from parent>
==2103442== 
==2103442== Open file descriptor 7:
==2103442==    <inherited from parent>
==2103442== 
==2103442== Open file descriptor 6:
==2103442==    <inherited from parent>
==2103442== 
==2103442== Open file descriptor 5:
==2103442==    <inherited from parent>
==2103442== 
==2103442== Open file descriptor 4: /dev/pts/3
==2103442==    <inherited from parent>
==2103442== 
==2103442== Open file descriptor 3: /dev/pts/3
==2103442==    <inherited from parent>
==2103442== 
==2103442== 
==2103442== HEAP SUMMARY:
==2103442==     in use at exit: 0 bytes in 0 blocks
==2103442==   total heap usage: 228 allocs, 228 frees, 156,326 bytes allocated
==2103442== 
==2103442== All heap blocks were freed -- no leaks are possible
==2103442== 
==2103442== For lists of detected and suppressed errors, rerun with: -s
==2103442== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
### expand
#### normal process
ok
#### unclosed quatations
```bash
c6r3s14% valgrind --leak-check=full --trace-children=yes --track-fds=yes ./minishell
==2105099== Memcheck, a memory error detector
==2105099== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2105099== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2105099== Command: ./minishell
==2105099== 
minishell$ echo $USER
ssoeno
minishell$ echo '$USER
Error: function name: move_to_next_quotation, Error message: squote not closed==2105099== 
==2105099== FILE DESCRIPTORS: 3 open (3 std) at exit.
==2105099== 
==2105099== HEAP SUMMARY:
==2105099==     in use at exit: 232,005 bytes in 425 blocks
==2105099==   total heap usage: 780 allocs, 355 frees, 261,400 bytes allocated
==2105099== 
==2105099== 59 (24 direct, 35 indirect) bytes in 1 blocks are definitely lost in loss record 30 of 82
==2105099==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==2105099==    by 0x4017E8: create_token (lexer_helper.c:34)
==2105099==    by 0x401F50: fetch_fst_word_token (lexer.c:81)
==2105099==    by 0x401D15: lexer (lexer.c:42)
==2105099==    by 0x401588: start_exec (main.c:81)
==2105099==    by 0x40146E: main (main.c:68)
==2105099== 
==2105099== LEAK SUMMARY:
==2105099==    definitely lost: 24 bytes in 1 blocks
==2105099==    indirectly lost: 35 bytes in 3 blocks
==2105099==      possibly lost: 0 bytes in 0 blocks
==2105099==    still reachable: 231,946 bytes in 421 blocks
==2105099==         suppressed: 0 bytes in 0 blocks
==2105099== Reachable blocks (those to which a pointer was found) are not shown.
==2105099== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==2105099== 
==2105099== For lists of detected and suppressed errors, rerun with: -s
==2105099== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
c6r3s14% valgrind --leak-check=full --trace-children=yes --track-fds=yes ./minishell
==2105112== Memcheck, a memory error detector
==2105112== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2105112== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2105112== Command: ./minishell
==2105112== 
minishell$ echo "$USER
Error: function name: move_to_next_quotation, Error message: dquote not closed==2105112== 
==2105112== FILE DESCRIPTORS: 3 open (3 std) at exit.
==2105112== 
==2105112== HEAP SUMMARY:
==2105112==     in use at exit: 231,885 bytes in 416 blocks
==2105112==   total heap usage: 753 allocs, 337 frees, 261,035 bytes allocated
==2105112== 
==2105112== LEAK SUMMARY:
==2105112==    definitely lost: 0 bytes in 0 blocks
==2105112==    indirectly lost: 0 bytes in 0 blocks
==2105112==      possibly lost: 0 bytes in 0 blocks
==2105112==    still reachable: 231,885 bytes in 416 blocks
==2105112==         suppressed: 0 bytes in 0 blocks
==2105112== Reachable blocks (those to which a pointer was found) are not shown.
==2105112== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==2105112== 
==2105112== For lists of detected and suppressed errors, rerun with: -s
==2105112== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
#### abnormal
```bash
c6r3s14% valgrind --leak-check=full --trace-children=yes --track-fds=yes ./minishell
==2105150== Memcheck, a memory error detector
==2105150== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2105150== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2105150== Command: ./minishell
==2105150== 
minishell$ echo '$USER'
$USER
minishell$ echo "$USER"
ssoeno
minishell$ echo '"$USER"'
"$USER"
minishell$ echo "'$USER'"
'ssoeno'
minishell$ echo '"$USER'"
Error: function name: move_to_next_quotation, Error message: dquote not closed==2105150== 
==2105150== FILE DESCRIPTORS: 3 open (3 std) at exit.
==2105150== 
==2105150== HEAP SUMMARY:
==2105150==     in use at exit: 232,577 bytes in 449 blocks
==2105150==   total heap usage: 920 allocs, 471 frees, 263,833 bytes allocated
==2105150== 
==2105150== 248 (96 direct, 152 indirect) bytes in 4 blocks are definitely lost in loss record 39 of 85
==2105150==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==2105150==    by 0x4017E8: create_token (lexer_helper.c:34)
==2105150==    by 0x401F50: fetch_fst_word_token (lexer.c:81)
==2105150==    by 0x401D15: lexer (lexer.c:42)
==2105150==    by 0x401588: start_exec (main.c:81)
==2105150==    by 0x40146E: main (main.c:68)
==2105150== 
==2105150== LEAK SUMMARY:
==2105150==    definitely lost: 96 bytes in 4 blocks
==2105150==    indirectly lost: 152 bytes in 12 blocks
==2105150==      possibly lost: 0 bytes in 0 blocks
==2105150==    still reachable: 232,329 bytes in 433 blocks
==2105150==         suppressed: 0 bytes in 0 blocks
==2105150== Reachable blocks (those to which a pointer was found) are not shown.
==2105150== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==2105150== 
==2105150== For lists of detected and suppressed errors, rerun with: -s
==2105150== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```
#### set -> unset case
OK!

## code
### syntax error
```bash
minishell$ cat <
Error: function name: count_nodes_for_redirect, Error message: invalid syntax==2102857== 
==2102857== FILE DESCRIPTORS: 4 open (3 std) at exit.
==2102857== Open file descriptor 5:
==2102857==    at 0x49D907D: pipe (pipe.c:29)
==2102857==    by 0x4087CA: read_heredoc (heredoc_read.c:29)
==2102857==    by 0x408568: call_heredoc (heredoc.c:45)
==2102857==    by 0x40846B: heredoc_handler (heredoc.c:26)
==2102857==    by 0x408440: heredoc_handler (heredoc.c:23)
==2102857==    by 0x4015AF: start_exec (main.c:84)
==2102857==    by 0x40146E: main (main.c:68)
==2102857== 
==2102857== 
==2102857== HEAP SUMMARY:
==2102857==     in use at exit: 232,130 bytes in 434 blocks
==2102857==   total heap usage: 909 allocs, 475 frees, 296,345 bytes allocated
==2102857== 
==2102857== 139 (24 direct, 115 indirect) bytes in 1 blocks are definitely lost in loss record 39 of 87
==2102857==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==2102857==    by 0x4017E8: create_token (lexer_helper.c:34)
==2102857==    by 0x401F50: fetch_fst_word_token (lexer.c:81)
==2102857==    by 0x401D15: lexer (lexer.c:42)
==2102857==    by 0x401588: start_exec (main.c:81)
==2102857==    by 0x40146E: main (main.c:68)
==2102857== 
==2102857== LEAK SUMMARY:
==2102857==    definitely lost: 24 bytes in 1 blocks
==2102857==    indirectly lost: 115 bytes in 9 blocks
==2102857==      possibly lost: 0 bytes in 0 blocks
==2102857==    still reachable: 231,991 bytes in 424 blocks
==2102857==         suppressed: 0 bytes in 0 blocks
==2102857== Reachable blocks (those to which a pointer was found) are not shown.
==2102857== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==2102857== 
==2102857== For lists of detected and suppressed errors, rerun with: -s
==2102857== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```
### data structure
- わからん 
### code structure
- わからん
### NITS FB
- INCLUDES = -I includes -I $(LIBFT_DIR), CFLAGS = -Wall -Wextra -Werror -g $(INCLUDES)が設定されているので"minishell.h"のインクルード記述でOK
- exit statusはenumかマクロ定義推奨
- 福原のチームはclose_fdsみたいな関数で雑に全部閉じるといった解決策を適用しました
