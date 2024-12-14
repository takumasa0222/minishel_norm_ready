#include "../../includes/minishell.h"
#include "../lexer_test/lexer_tester.h"
#include "../parser_test/parser_tester.h"
#include "sample.h"

ASTNode *create_filename_node(const char *filename) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) {
        return NULL;
    }
    node->type = NODE_FILENAME;
    node->data.filename.value = strdup(filename);
    return node;
}

ASTNode *create_wordlist_node(char **words, size_t word_count) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) {
        return NULL;
    }
    node->type = NODE_WORDLIST;
    node->data.wordlist.words = words;
    node->data.wordlist.word_count = word_count;
    return node;
}

ASTNode *create_redirection_node(const char *symbol, ASTNode *filename, int number) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) {
        return NULL;
    }
    node->type = NODE_REDIRECTION;
    node->data.redirection.symbol = strdup(symbol);
    node->data.redirection.filename = filename;
    node->data.redirection.number = number;
    return node;
}

ASTNode *create_simple_command_node(ASTNode **redir_before, size_t redir_before_count, ASTNode **redir_after, size_t redir_after_count, ASTNode *wordlist) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) {
        return NULL;
    }
    node->type = NODE_SIMPLE_COMMAND;
    node->data.simple_command.redirections_before = redir_before;
    node->data.simple_command.redirection_before_count = redir_before_count;
    node->data.simple_command.redirections_after = redir_after;
    node->data.simple_command.redirection_after_count = redir_after_count;
    node->data.simple_command.wordlist = wordlist;
    return node;
}

ASTNode *create_command_node(ASTNode *simple_command, ASTNode *command_tail) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) {
        return NULL;
    }
    node->type = NODE_COMMAND;
    node->data.command.simple_command = simple_command;
    node->data.command.command_tail = command_tail;
    return node;
}

ASTNode *create_command_tail_node(const char *connector, ASTNode *cmd_type_or_redir, ASTNode *next_tail) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) {
        return NULL;
    }
    node->type = NODE_COMMAND_TAIL;
    node->data.command_tail.connector = strdup(connector);
    node->data.command_tail.cmd_type_or_redir = cmd_type_or_redir;
    node->data.command_tail.next_tail = next_tail;
    return node;
}


/// @brief トークンの種類を返す

static t_token *current_token = NULL;

static void advance_token() {
    if (current_token) {
        current_token = current_token->next;
    }
}

static t_node_kind peek_kind()
{
    if (current_token == NULL) {
        return ND_EOF;
    }
    return current_token->kind;
}

// parse_start()
// parse_command()
// parse_command_tail()
// parse_cmd_type()
// parse_simple_command()
// parse_wordlist()
// parse_redirection()

/*
filename: WORD
*/
ASTNode *parse_filename()
{
    if (peek_kind() != ND_CMD) {
        return NULL;
    }
    char *filename = strdup(current_word());
    advance_token();
    return create_filename_node(filename);
}

/*
redirection:
    ">" filename
  | "<" filename
  | ">>" filename
  | "<<" filename
  | "<>" filename
  | NUMBER ">" filename
  | NUMBER "<" filename
  | NUMBER ">>" filename
  | NUMBER "<<" filename
  | NUMBER "<>" filename
*/
ASTNode *parse_redirection()
{
    int num = -1;
    if (peek_kind() == ND_FD_NUM) {
        num = atoi(current_word());
        advance_token();
    }
    if (peek_kind() != ND_REDIRECTS) {
        return NULL; // error
    }
    char *symbol = strdup(current_word());
    advance_token();
    ASTNode *filename = parse_filename();
    if (filename == NULL) {
        free(symbol);
        return NULL;
    }
    return create_redirection_node(symbol, filename, 0);
}

/*
wordlist: WORD | wordlist WORD
*/
ASTNode *parse_wordlist()
{
    size_t word_count = 0;
    char **words = NULL;
    if (peek_kind() != ND_CMD) {
        return NULL; // error: word is a mandatory
    }
    while (peek_kind() == ND_CMD) {
        words = realloc(words, sizeof(char *) * (word_count + 1));
        words[word_count++] = strdup(current_word());
        advance_token();
    }
    return create_wordlist_node(words, word_count);
}

/*
simple_command: redirection* wordlist redirection*
              | redirection+
*/
static int is_redirection_start() {
    // ND_REDIRECTSやND_FD_NUM + ND_REDIRECTSパターンを考慮
    t_node_kind k = peek_kind();
    if (k == ND_REDIRECTS) return 1;
    if (k == ND_FD_NUM) {
        t_token *next = current_token->next;
        if (next && next->kind == ND_REDIRECTS) return 1;
    }
    return 0;
}

ASTNode *parse_simple_command() {
    // 前部redirection読み込み
    ASTNode **redir_before = NULL;
    size_t redir_before_count = 0;
    while (is_redirection_start()) {
        ASTNode *r = parse_redirection();
        if (!r) return NULL;
        redir_before = realloc(redir_before, sizeof(ASTNode*)*(redir_before_count+1));
        redir_before[redir_before_count++] = r;
    }
    // wordlistチェック
    ASTNode *wordlist = NULL;
    if (peek_kind() == ND_CMD) {
        wordlist = parse_wordlist();
        if (!wordlist) return NULL;
    }
    // 後部redirection読み込み
    ASTNode **redir_after = NULL;
    size_t redir_after_count = 0;
    while (is_redirection_start()) {
        ASTNode *r = parse_redirection();
        if (!r) return NULL;
        redir_after = realloc(redir_after, sizeof(ASTNode*)*(redir_after_count+1));
        redir_after[redir_after_count++] = r;
    }
    // wordlistも前後redirectionもない場合はエラー
    if (!wordlist && redir_before_count == 0 && redir_after_count == 0) {
        // エラー: 空のsimple_commandになってしまう
        return NULL;
    }

    // redirectionリストをまとめる処理が必要であれば行う
    // 簡略化のため、ここではNULL渡し。実際には
    // redirectionリスト用のノード作成などが必要。
    return create_simple_command_node(NULL, redir_before_count, NULL, redir_after_count, wordlist);
}


/*
command_tail:
    "|" cmd_type command_tail
  | ("&&" | "||") cmd_type command_tail
  | redirection
  |
*/
ASTNode *parse_command_tail() {
    t_node_kind k = peek_kind();

    // "|"の場合
    if (k == ND_PIPE) {
        advance_token(); // "|"
        ASTNode *simple_cmd_node = parse_simple_command();
        if (!simple_cmd_node) return NULL;
        ASTNode *next_tail = parse_command_tail();
        return create_command_tail_node("|", simple_cmd_node, next_tail);
    }
    // "&&"
    if (k == ND_AND_OP) {
        advance_token(); // "&&"
        ASTNode *simple_cmd_node = parse_simple_command();
        if (!simple_cmd_node) return NULL;
        ASTNode *next_tail = parse_command_tail();
        return create_command_tail_node("&&", simple_cmd_node, next_tail);
    }
    // "||"
    if (k == ND_OR_OP) {
        advance_token(); // "||"
        ASTNode *simple_cmd_node = parse_simple_command();
        if (!simple_cmd_node) return NULL;
        ASTNode *next_tail = parse_command_tail();
        return create_command_tail_node("||", simple_cmd_node, next_tail);
    }
    // redirectionの場合
    if (k == ND_FD_NUM || k == ND_REDIRECTS) {
        ASTNode *redir = parse_redirection();
        if (!redir) return NULL;
        return create_command_tail_node(NULL, redir, NULL);
    }
    // ε(空)
    return NULL;
}

ASTNode *parse_command() {
    ASTNode *simple_cmd_node = parse_simple_command();
    if (!simple_cmd_node) return NULL;
    ASTNode *tail_node = parse_command_tail();
    return create_command_node(simple_cmd_node, tail_node);
}

ASTNode *parse_start(t_token *token_list) {
    current_token = token_list;
    ASTNode *root = parse_command();
    // 最後にND_EOFを期待してもいい(任意)
    return root;
}

int main() {
    char *input = "echo hello | grep h && cat < input.txt";
    t_token *tokens = lexer(input);
    ASTNode *ast = parse_start(tokens);

    // 生成されたastを処理する ...
    // astを解放する処理も必要

    return 0;
}

