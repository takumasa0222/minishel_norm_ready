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

ASTNode *create_simple_command_node(ASTNode *redir_before, size_t redir_before_count, ASTNode *redir_after, size_t redir_after_count, ASTNode *wordlist) {
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

ASTNode *create_subshell_node(ASTNode *inner_command, ASTNode *command_tails, size_t command_tail_count) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) {
        return NULL;
    }
    node->type = NODE_SUBSHELL;
    node->data.subshell.inner_command = inner_command;
    node->data.subshell.command_tails = command_tails;
    node->data.subshell.command_tail_count = command_tail_count;
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

ASTNode *parse_command() {
    ASTNode *command_type_node = parse_cmd_type(); // simple command or subshell
    ASTNode *command_tail_node = parse_command_tail();
    ASTNode *command_node = malloc(sizeof(ASTNode));
    command_node->type = NODE_COMMAND;
    command_node->data.command.type = command_type_node;
    command_node->data.command.tail = command_tail_node;
    return command_node;

}

/*
最初にredirectionをできるだけ読む（0回以上）、続いてwordlistを読めれば読む、その後再度redirectionを0回以上読む
*/
ASTNode *parse_simple_command()
{
    ASTNode **redir_before = NULL;
    size_t redir_before_count = 0;
    while (next_token_is_redirection()) {
        // parse redirectionでredirection nodeを作成
        ASTNode *redir = parse_redirection();
        redir_before = realloc(redir_before, sizeof(ASTNode *) * (redir_before_count + 1));
        redir_before[redir_before_count++] = redir;
    }
    // wordlistがあれば読む（なければNULLを返す）
    ASTNode *wordlist = parse_wordlist();
    // redirectionの後半
    ASTNode **redir_after = NULL;
    size_t redir_after_count = 0;
    while (next_token_is_redirection()) {
        ASTNode *redir = parse_redirection();
        redir_after = realloc(redir_after, sizeof(ASTNode *) * (redir_after_count + 1));
        redir_after[redir_after_count++] = redir;
    }

    // simple command nodeを作成
    ASTNode *simple_command = create_simple_command_node(redir_before, redir_before_count, redir_after, redir_after_count, wordlist);
    return simple_command;
}