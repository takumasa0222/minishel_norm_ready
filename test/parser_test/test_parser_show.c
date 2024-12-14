#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ノード構造体
typedef struct TreeNode {
    char *kind;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// ノードを作成する関数
TreeNode* create_node(const char *kind) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    node->kind = strdup(kind); // ノードのデータをコピー
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 木を図示する関数


// メモリ解放の関数
void free_tree(TreeNode *node) {
    if (node == NULL) {
        return;
    }
    free_tree(node->left);
    free_tree(node->right);
    free(node->kind);
    free(node);
}

int main() {
    // 例: 木の構築
    TreeNode *root = create_node("kind");
    root->left = create_node("kind");
    root->right = create_node("kind");
    root->left->left = create_node("kind");
    root->left->right = create_node("kind");
    root->right->left = create_node("kind");
    root->right->right = create_node("kind");

    // 木を図示
    printf("Binary Tree:\n");
    print_tree(root, 0, "root");

    // メモリ解放
    free_tree(root);

    return 0;
}
