#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "utils.h"

FileTree createFileTree(char* rootFolderName) {
    FileTree file;
    file.root = calloc(1, sizeof(TreeNode));

    file.root->name = rootFolderName;
    file.root->type = FOLDER_NODE;

    return file;
}

void touch(TreeNode* currentDirectory, char* fileName, char* fileContent) {
    // verify if the file/folder name already exists, if no, add file
    if (search_pattern(currentDirectory, fileName) == 0) {
        // move in parent folder and alloc memory for content if it's NULL
        TreeNode *parent = currentDirectory;
        parent_path(&parent, fileName);
        if (parent->content == NULL) {
            parent->content = alloc_folder_content(FOLDER_NODE);
        }
        // create the new file
        List *list = ((FolderContent*)parent->content)->children;
        add_node(&list, currentDirectory, fileName, fileContent, FILE_NODE);
    }

    // free the name of the file and the content
    free(fileName);
    free(fileContent);
}

void mkdir(TreeNode* currentDirectory, char* folderName) {
    // vefify if the folder name already exists, if yes, print error and exit
    if (search_pattern(currentDirectory, folderName) != 0) {
        printf(MKDIR_ERROR, folderName);
        return;
    }
    // move in parent folder and alloc memory for content if it's NULL
    TreeNode *parent = currentDirectory;
    parent_path(&parent, folderName);
    if (parent->content == NULL) {
        parent->content = alloc_folder_content(FOLDER_NODE);
    }
    // create the new folder
    List *list = ((FolderContent*)parent->content)->children;
    add_node(&list, parent, folderName, NULL, FOLDER_NODE);

    // free the name of the folder
    free(folderName);
}

void ls(TreeNode* currentDirectory, char* arg) {
    // verify iy we have argument, if not, list the current directory content
    if (strcmp(arg, "\0") == 0) {
        // if we have content, list it
        if (currentDirectory->content != NULL) {
            print_folder_content(currentDirectory);
        }
        return;
    }

    /* search if there is a file/directory with the given name
    if we don't have it, print error and exit the function */
    if (search_pattern(currentDirectory, arg) == 0) {
        printf(LS_ERROR, arg);
        return;
    }

    TreeNode *node = currentDirectory;
    move_in_path(&node, arg);
    // case in which we print a file content
    if (node->type == FILE_NODE) {
        printf("%s: %s\n", arg, ((FileContent*)node->content)->text);
    } else {
        // case in which we print a directory content
        node->content == NULL ? printf("\n") : print_folder_content(node);
    }
}

TreeNode* cd(TreeNode* currentDirectory, char* path) {
    // verify if the path is correct, if not, print error and exit the function
    int correctPath = search_pattern(currentDirectory, path);
    if (correctPath == 0) {
        printf(CD_ERROR, path);
        return currentDirectory;
    }

    // if it is a file, print error and exit the function
    if (correctPath == 2) {
        printf(CD_ERROR, path);
        return currentDirectory;
    }

    move_in_path(&currentDirectory, path);
    return currentDirectory;
}

void pwd(TreeNode* currentDirectory) {
    if (strcmp(currentDirectory->name, ROOT_NAME) != 0) {
        pwd(currentDirectory->parent);
        printf("/%s", currentDirectory->name);
    } else {
        printf(ROOT_NAME);
    }
}

void tree(TreeNode* currentDirectory, char* arg) {
    // verify if we have argument
    if (strcmp(arg, "\0") != 0) {
        /* verify if the path is correct and we have a directory,
        if not, print error and exit the function */
        int type_tail = search_pattern(currentDirectory, arg);
        if (type_tail != 1) {
            printf(TREE_ERROR_DIR, arg);
            return;
        }
        // change the current directory to the one we want to list the tree
        move_in_path(&currentDirectory, arg);
    }

    // case in which we have a directory without content
    if (currentDirectory->content == NULL) {
        printf(TREE_EMPTY_DIR);
        return;
    }

    /* go through the directory and print the content recursively,
       calculating the number of files/directories in the same time */
    int cnt_folder = 0, cnt_file = 0;
    List *list = ((FolderContent*) currentDirectory->content)->children;
    ListNode *curr = list->head, *tmp;

    while (curr != NULL) {
        tmp = curr->next;
        lsrec(curr->info, &cnt_file, &cnt_folder, 0);
        curr = tmp;
    }

    printf(TREE_DIR, cnt_folder, cnt_file);
}

void rm(TreeNode* currentDirectory, char* fileName) {
    // verity if the file exists, if not, print error and exit the function
    if (search_pattern(currentDirectory, fileName) == 0) {
        printf(RM_ERROR, fileName);
        return;
    }

    TreeNode *node = currentDirectory;
    move_in_path(&node, fileName);
    // verify if its a file, if no, print error and exit the function
    if (node->type == FOLDER_NODE) {
        printf(RM_DIR_ERROR, fileName);
        return;
    }

    // remove the file
    TreeNode *parent = node;
    move_in_path(&parent, PARENT_DIR);
    List *list = ((FolderContent*) parent->content)->children;
    erase_node(&list, node->name, 0);
}

void rmdir(TreeNode* currentDirectory, char* folderName) {
    // verify if the folder exists, if not, print error and exit the function
    if (search_pattern(currentDirectory, folderName) == 0) {
        printf(RMDIR_NOFOLDER_ERROR, folderName);
        return;
    }

    TreeNode *node = currentDirectory;
    move_in_path(&node, folderName);
    // verify if its a folder, if no, print error and exit the function
    if (node->type == FILE_NODE) {
        printf(RMDIR_FILE_ERROR, folderName);
        return;
    }

    // verify if the folder is empty, if not, print error and exit the function
    if (node->content != NULL) {
        printf(RMDIR_NOTEMPTY_ERROR, folderName);
        return;
    }

    // remove the folder
    TreeNode *parent = node;
    move_in_path(&parent, PARENT_DIR);
    List *list = ((FolderContent*) parent->content)->children;
    erase_node(&list, node->name, 1);
}

void rmrec(TreeNode* currentDirectory, char* resourceName) {
    // verify if the file/folder exists, if not, print error and exit
    if (search_pattern(currentDirectory, resourceName) == 0) {
        printf(RMREC_ERROR, resourceName);
        return;
    }

    TreeNode *node = currentDirectory;
    move_in_path(&node, resourceName);
    if (node->type == FILE_NODE) {
        // file case
        rm(currentDirectory, resourceName);
    } else if (node->content == NULL) {
        // empty folder case
        rmdir(currentDirectory, resourceName);
    } else {
        // folder with content case
        TreeNode *parent = node;
        move_in_path(&parent, PARENT_DIR);
        List *list = ((FolderContent*) parent->content)->children;
        erase_node(&list, node->name, 2);
    }
}

void cp(TreeNode *currentDirectory, char *source, char *destination) {
    // particular cases
    if (search_pattern(currentDirectory, destination) == 0) {
        printf(CP_DEST_ERROR, destination);
        return;
    }
    int correctPathSource = search_pattern(currentDirectory, source);
    if (correctPathSource == 0) {
        printf(CP_SRC_ERROR, source);
        return;
    }
    if (correctPathSource == 1) {
        printf(CP_REC_ERROR, source);
        return;
    }
    TreeNode *src = currentDirectory, *dest = currentDirectory;

    move_in_path(&src, source);
    move_in_path(&dest, destination);

    // copy source to destination
    char *filecont = ((FileContent*)src->content)->text;
    if (dest->type == FILE_NODE) {
        FileContent *content = (FileContent*)dest->content;
        content->text = realloc(content->text, strlen(filecont) + 1);
        memcpy(content->text, filecont, strlen(filecont) + 1);
        return;
    } else if (dest->type == FOLDER_NODE) {
        if (search_pattern(dest, src->name) == 0) {
            if (dest->content == NULL) {
                dest->content = alloc_folder_content(FOLDER_NODE);
            }
            List *list = ((FolderContent*)dest->content)->children;
            add_node(&list, dest, source, filecont, FILE_NODE);
        }
    }
}

void mv(TreeNode* currentDirectory, char* source, char* destination) {
    // particular case when the destination is a file or doesn't exist
    if (search_pattern(currentDirectory, destination) == 0) {
        printf(MV_DEST_ERROR, destination);
        return;
    }

    TreeNode *src = currentDirectory, *dest = currentDirectory;
    move_in_path(&src, source);
    move_in_path(&dest, destination);

    // if the source is a folder, move it to the destination
    if (search_pattern(currentDirectory, source) == 1) {
        if (dest->content == NULL) {
            dest->content = alloc_folder_content(FOLDER_NODE);
        }

        List *list_dest = ((FolderContent*)dest->content)->children;
        List *list_src = ((FolderContent*)currentDirectory->content)->children;

        add_to_dest(&list_dest, src);
        remove_from_dest(&list_src, src);

    } else {
        // if the source is a file, copy it to the destination
        cp(currentDirectory, source, destination);
        // remove the source with rm
        TreeNode *parent = src;
        move_in_path(&parent, PARENT_DIR);
        rm(parent, src->name);
    }
}

void freeTree(FileTree fileTree) {
    TreeNode* currentFolder = fileTree.root;

    // verify if the root has content to be freed and free it recursively
    free_TreeNode(currentFolder);

    free(currentFolder->name);
    free(currentFolder);
}
