#include"tree_list_datastructure.h"

#include<stdlib.h>

// Append a node to the list
void AddNode(TreeList* list, void* data){
    // Create new node
    Node* newNode = malloc(sizeof(Node));
    newNode->data = data;

    // Traverse to end of list
    Node* curr = list->root; 
    while(curr->next != NULL) { curr = curr->next; }

    // Attach node to end
    curr->next = newNode;
}

// Remove specified node from the list
void RemoveNode(TreeList* list, Node* nodeToRemove){
    //if(IsListEmpty(list)) return;

    Node* prev = NULL;
    Node* curr = list->root;
    while(curr->next != NULL){
        if(curr == nodeToRemove){
            free(curr);
            break;
        }
        
        prev = curr;
        curr = curr->next;
    }

}
