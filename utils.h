#define PATH_SEPARATOR "/\n\0"
#define CURRENT_DIR "."

/**
 * @brief Create a new node
 * @param node the node we want to create
 * @param currentNode its parent node
 * @param fileName node name
 * @param fileContent node content if the node is a file and has content,
 * otherwise this parameter is NULL
 * @param type node type
 */
void create_node(ListNode **node, TreeNode* currentNode,
                 char* fileName, char* fileContent, enum TreeNodeType type);

/**
 * @brief Allocate memory for a folder content node
 * @return void* the allocated memory
 */
void *alloc_folder_content();

/**
 * @brief Print the content of a folder
 * @param node the folder node
 */
void print_folder_content(TreeNode *node);

/**
 * @brief Add to a specific content list a new file/folder
 * @param list content list
 * @param currentNode parent node
 * @param Name file/folder name
 * @param fileContent node content if the node is a file and has content,
 * otherwise this parameter is NULL
 * @param type type of the node (file/folder)
 */
void add_node(List **list, TreeNode* currentNode, char* Name,
               char* fileContent, enum TreeNodeType type);

/**
 * @brief Erase a node from a specific content list
 * @param list content list
 * @param name name of the node we want to erase
 * @param type type of the node we want to erase:
 * 0 - file; 1 - empty folder; 2 - folder with content
 */
void erase_node(List **list, char *name, int type);

/**
 * @brief Move a file/folder from a specific content list to another
 * @param list content list in which we want to move the node
 * @param src node we want to move
 */
void add_to_dest(List **list, TreeNode *src);

/**
 * @brief Remove a file/folder from a specific content list
 * @param list content list from which we want to remove the node
 * @param src node we want to remove
 */
void remove_from_dest(List **list, TreeNode *src);

/**
 * @brief Find a file/folder in a specific folder content list
 * @param currentNode folder in which we want to find the node
 * @param Name name of the file/folder
 * @return TreeNode* node with the given name or NULL if the folder is empty
 */
TreeNode* get_through_dir(TreeNode *currentNode, char *Name);

/**
 * @brief Verify if a path is correct or not and if it is a file or a folder
 * @param currentNode folder from which we want to verify the path
 * @param path the path we want to verify
 * @return int 0 - if the path is not correct, 1 - if the last element
 * of the path is a folder, 2 - if the last element of the path is a file
 */
int search_pattern(TreeNode *currentNode, char *path);

/**
 * @brief Move within the file system tree according to a given path
 * @param currentDirectory current directory
 * @param path the path we want to follow
 */
void move_in_path(TreeNode **currentDirectory, char *path);

/**
 * @brief Move in the parent directory of the path
 * @param currentDirectory current directory
 * @param path path we want to move in the parent directory
 */
void parent_path(TreeNode **currentDirectory, char *path);

/**
 * @brief Print the content of a file/folder recursively and update the number
 * of files and folders and the level of the tree we are in for the tree command
 * @param node current file/folder
 * @param cnt_file number of files
 * @param cnt_folder number of folders
 * @param level level of the tree
 */
void lsrec(TreeNode *node, int *cnt_file, int *cnt_folder, int level);

/**
 * @brief Auxiliar function for the rmrec function
 * that frees the memory recursively
 * @param node directory that needs to be freed
 */
void remove_rec(TreeNode* node);

/**
 * @brief Free the memory of a file
 * 
 * @param node file that needs to be freed
 */
void free_file_tree(ListNode **node);

/**
 * @brief Free the memory of a folder
 * @param node folder that needs to be freed
 */
void free_TreeNode(TreeNode *node);
