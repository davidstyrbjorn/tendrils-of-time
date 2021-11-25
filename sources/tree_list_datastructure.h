#ifndef TREE_LIST_DATASTRUCTURE_H
#define TREE_LIST_DATASTRUCTURE_H


typedef struct Node {
    struct Node* next;
    void* data;
} Node;

typedef struct TreeList {
    Node* root; // Dummy root, no data
} TreeList;

void AddNode(TreeList* list, void* data);
void RemoveNode(TreeList* list, Node* nodeToRemove);

#endif