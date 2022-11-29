#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "utils.h"

void create_node(ListNode **node, TreeNode* currentNode,
                 char* fileName, char* fileContent, enum TreeNodeType type) {
    // alloc memory for the node
    (*node)->info = calloc(1, sizeof(TreeNode));

    // add the necessary information
    (*node)->info->parent = currentNode;
    (*node)->info->name = malloc(strlen(fileName) + 1);
    memcpy((*node)->info->name, fileName, strlen(fileName) + 1);
    (*node)->info->type = type;

    // if it's a file, alloc and add the content
    if (type == FILE_NODE) {
        (*node)->info->content = calloc(1, sizeof(FileContent));
        FileContent *content = (*node)->info->content;
        content->text = malloc(strlen(fileContent) + 1);
        memcpy(((FileContent*)(*node)->info->content)->text,
               fileContent, strlen(fileContent) + 1);
    }
}

void *alloc_folder_content() {
    FolderContent *content = calloc(1, sizeof(FolderContent));
    content->children = calloc(1, sizeof(List));

    return content;
}

void print_folder_content(TreeNode *node) {
    List *list = ((FolderContent*)node->content)->children;
    ListNode *curr = list->head;
    // print the name of each child
    while (curr != NULL) {
        printf("%s\n", curr->info->name);
        curr = curr->next;
    }
}

void add_node(List **list, TreeNode* currentNode, char* Name,
               char* fileContent, enum TreeNodeType type) {
    // the parent's content is NULL and needs to be allocated
    if (*list == NULL) {
        *list = calloc(1, sizeof(List));
    }

    // the list is empty and the node is the first one
    if ((*list)->head == NULL) {
        (*list)->head = calloc(1, sizeof(ListNode));
        create_node(&((*list)->head), currentNode, Name, fileContent, type);
    } else {
        ListNode *new = calloc(1, sizeof(ListNode));
        create_node(&new, currentNode, Name, fileContent, type);
        new->next = (*list)->head;
        (*list)->head = new;
    }
}

void erase_node(List **list, char *name, int type) {
    ListNode *curr = (*list)->head, *prev = NULL;
    // iterate through the list
    while (curr != NULL) {
        // find the node we want to erase
        if (strcmp(((TreeNode*) curr->info)->name, name) == 0) {
            // erase the link between the previous node and the current one
            if (prev == NULL) {
                (*list)->head = (*list)->head->next;
            } else {
                prev->next = curr->next;
            }

            // free the memory according to the type of the node
            if (type == 0) {
                free_file_tree(&curr);
            } else if (type == 1) {
                free(curr->info->name);
                free(curr->info);
                free(curr);
            } else if (type == 2) {
                remove_rec(curr->info);
                free(curr);
            }
            return;
        }

    prev = curr;
    curr = curr->next;
}
}

void add_to_dest(List **list, TreeNode *src) {
    if ((*list)->head == NULL) {
        (*list)->head = calloc(1, sizeof(ListNode));
        (*list)->head->info = src;
    } else {
        ListNode *new = calloc(1, sizeof(ListNode));
        new->info = calloc(1, sizeof(TreeNode));
        new->info = src;
        new->next = (*list)->head;
        (*list)->head = new;
    }
}

void remove_from_dest(List **list, TreeNode *src) {
    ListNode *curr = (*list)->head;
    ListNode *prev = (*list)->head;
    while (curr != NULL) {
        // find the node we want to erase
        if (curr->info == src) {
            if (curr == (*list)->head) {
                (*list)->head = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

TreeNode* get_through_dir(TreeNode *currentNode, char *Name) {
    // verify if we have content
    if (currentNode->content == NULL)
        return NULL;

    // searching for the file/folder
    List* list = ((FolderContent*) currentNode->content)->children;
    ListNode *curr  = list->head;
    while (curr != NULL) {
        if (strcmp(((TreeNode*)curr->info)->name, Name) == 0)
            return (TreeNode*)curr->info;
        curr = curr->next;
    }

    // not found
    return NULL;
}

int search_pattern(TreeNode *currentNode, char *path) {
    // create an auxiliary variable to store the path
    char *aux_path = malloc(strlen(path) + 1);
    memcpy(aux_path, path, strlen(path) + 1);

    int type_tail = 0;

    /* break the path into strings and check if the parent has or not
       a subfile/subdirectory with the respective name, and in case
       affirmative, the parent moves */
    char *pointer = strtok(aux_path, PATH_SEPARATOR);
    while (pointer != NULL) {
        TreeNode *node = get_through_dir(currentNode, pointer);
        // when the child is not found
        if (node == NULL && strcmp(pointer, PARENT_DIR) != 0) {
            free(aux_path);
            return 0;
        } else {
            // case when we need to go back
            if (strcmp(pointer, PARENT_DIR) == 0) {
                currentNode = currentNode->parent;
                type_tail = 1;
            } else {
                currentNode = node;
                // store the type of the last node
                type_tail = (node->type == FILE_NODE ? 2 : 1);
            }
        }
        pointer = strtok(NULL, PATH_SEPARATOR);
    }

    free(aux_path);
    return type_tail;
}

void move_in_path(TreeNode **currentDirectory, char *path) {
    // create an auxiliary variable to store the path
    char *aux_path = malloc(strlen(path) + 1);
    memcpy(aux_path, path, strlen(path) + 1);

    // break the path into tokens and move like in search_pattern
    char *pointer = strtok(aux_path, PATH_SEPARATOR);
    while (pointer != NULL) {
        if (strcmp(pointer, PARENT_DIR) == 0) {
            *currentDirectory = (*currentDirectory)->parent;
        } else {
            *currentDirectory = get_through_dir(*currentDirectory, pointer);
        }
        pointer = strtok(NULL, PATH_SEPARATOR);
    }

    // free the auxiliary variable and return the new current directory
    free(aux_path);
}

void parent_path(TreeNode **currentDirectory, char *path) {
    // create an auxiliary variable to store the path
    char *aux_path = malloc(strlen(path) + 1);
    memcpy(aux_path, path, strlen(path) + 1);

    // erase the last token from the path
    int p = strlen(aux_path) - 1;
    while (p >= 0 && aux_path[p] != '/') {
        aux_path[p] = '\0';
        p--;
    }
    aux_path[p] = '\0';

    // if the path is empty, set the current directory to .
    if (strlen(aux_path) == 0) {
        return;
    } else {
        move_in_path(currentDirectory, aux_path);
    }
}

void lsrec(TreeNode *node, int *cnt_file, int *cnt_folder, int level) {
    if (node == NULL) {
        return;
    }

    // print the number of tabs needed for the tree function
    for (int i = 0; i < level; i++) {
        printf("\t");
    }
    printf("%s\n", node->name);

    // update the number of files/folders
    if (node->type == FILE_NODE) {
        (*cnt_file)++;
    } else {
        (*cnt_folder)++;

        // go through the content and print it recursively
        if (node->content != NULL) {
            List *list = ((FolderContent*) node->content)->children;
            ListNode *curr = list->head;

            while (curr != NULL) {
                lsrec(curr->info, cnt_file, cnt_folder, level + 1);
                curr = curr->next;
            }
        }
    }
}

void remove_rec(TreeNode* node) {
    // go through the content and use rmrec to delete each element
    List *list = ((FolderContent*) node->content)->children;
    ListNode *curr = list->head;
    ListNode *tmp;
    while (curr !=NULL) {
        tmp = curr->next;
        rmrec(node, curr->info->name);
        curr = tmp;
    }

    // free the memory used to create the folder
    free(list);
    if (node->content)
        free(node->content);
    free(node->name);
    free(node);
}

void free_file_tree(ListNode **node) {
    free((*node)->info->name);
    free(((FileContent*)(*node)->info->content)->text);
    free((*node)->info->content);
    free((*node)->info);
    free(*node);
}

void free_TreeNode(TreeNode *node) {
    if (node->content != NULL) {
        List *list = ((FolderContent*) node->content)->children;
        ListNode *curr = list->head;
        ListNode *tmp;
        while (curr !=NULL) {
            tmp = curr->next;
            rmrec(node, curr->info->name);
            curr = tmp;
        }
        free(list);
        if (node->content)
            free(node->content);
    }
}
