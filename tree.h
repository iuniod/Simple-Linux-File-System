#define ROOT_NAME "root"
#define PARENT_DIR ".."
#define PATH_SEPARATOR "/\n\0"
#define LS_ERROR "ls: cannot access '%s': No such file or directory\n"
#define CD_ERROR "cd: no such file or directory: %s\n"
#define CD_FILE_ERROR "cd: not a directory: %s\n"
#define TREE_ERROR_DIR "%s [error opening dir]\n\n0 directories, 0 files\n"
#define TREE_EMPTY_DIR "0 directories, 0 files\n"
#define TREE_DIR "\n%d directories, %d files\n"
#define MKDIR_ERROR "mkdir: cannot create directory '%s': File exists\n"
#define RM_ERROR "rm: failed to remove '%s': No such file or directory\n"
#define RM_DIR_ERROR "rm: cannot remove '%s': Is a directory\n"
#define RMDIR_NOFOLDER_ERROR \
"rmdir: failed to remove '%s': No such file or directory\n"
#define RMDIR_FILE_ERROR "rmdir: failed to remove '%s': Not a directory\n"
#define RMDIR_NOTEMPTY_ERROR \
"rmdir: failed to remove '%s': Directory not empty\n"
#define RMREC_ERROR "rmrec: failed to remove '%s': No such file or directory\n"
#define CP_DEST_ERROR "cp: failed to access '%s': Not a directory\n"
#define CP_SRC_ERROR "cp: failed to access '%s': Not a directory\n"
#define CP_REC_ERROR "cp: -r not specified; omitting directory '%s'\n"
#define MV_DEST_ERROR "mv: failed to access '%s': Not a directory\n"

typedef struct FileContent FileContent;
typedef struct FolderContent FolderContent;
typedef struct TreeNode TreeNode;
typedef struct FileTree FileTree;
typedef struct ListNode ListNode;
typedef struct List List;

enum TreeNodeType {
    FILE_NODE,
    FOLDER_NODE
};

struct FileContent {
    char* text;
};

struct FolderContent {
    List* children;
};

struct TreeNode {
    TreeNode* parent;
    char* name;
    enum TreeNodeType type;
    void* content;
};

struct FileTree {
    TreeNode* root;
};

struct ListNode {
    TreeNode* info;
    ListNode* next;
};

struct List {
    ListNode* head;
};

/**
 * @brief Initialize the tree
 * @param rootFolderName the name of the root folder
 * @return FileTree the tree
 */
FileTree createFileTree(char* rootFolderName);

/**
 * @brief Create touch command
 * @param currentDirectory current directory 
 * @param fileName file name we want to create
 * @param fileContent file content we want to add
 */
void touch(TreeNode* currentDirectory, char* fileName, char* fileContent);

/**
 * @brief Create mkdir command
 * @param currentDirectory current directory
 * @param folderName name of the folder we want to create
 */
void mkdir(TreeNode* currentDirectory, char* folderName);

/**
 * @brief Create ls command
 * @param currentDirectory the current directory
 * @param arg the argument of the command - the name of the file/directory
 * that we want to list its content or NULL
 */
void ls(TreeNode* currentDirectory, char* arg);

/**
 * @brief Create cd command
 * @param currentDirectory current directory
 * @param path path to the directory we want to go
 * @return TreeNode* the new current directory
 */
TreeNode* cd(TreeNode* currentDirectory, char* path);

/**
 * @brief Create pwd command
 * @param currentDirectory current directory
 */
void pwd(TreeNode* currentDirectory);

/**
 * @brief Create tree command
 * @param currentDirectory current directory
 * @param arg the argument of the command - the name of the directory
 * that we want to list the tree or NULL 
 */
void tree(TreeNode* currentDirectory, char* arg);

/**
 * @brief Create rm command
 * @param currentDirectory current directory 
 * @param fileName file name we want to remove
 */
void rm(TreeNode* currentDirectory, char* fileName);

/**
 * @brief Create rmdir command
 * @param currentDirectory current directory
 * @param folderName folder name we want to remove
 */
void rmdir(TreeNode* currentDirectory, char* folderName);

/**
 * @brief Create rm -r command
 * @param currentDirectory current directory
 * @param resourceName file/folder name we want to remove
 */
void rmrec(TreeNode* currentDirectory, char* resourceName);

/**
 * @brief Create cp command
 * @param currentDirectory current directory
 * @param source source path
 * @param destination destination path
 */
void cp(TreeNode* currentDirectory, char* source, char* destination);

/**
 * @brief Create mv command
 * @param currentDirectory current directory
 * @param source source path
 * @param destination destination path
 */
void mv(TreeNode* currentDirectory, char* source, char* destination);

/**
 * @brief Free the memory allocated for the program
 * @param fileTree root of the file tree
 */
void freeTree(FileTree fileTree);
