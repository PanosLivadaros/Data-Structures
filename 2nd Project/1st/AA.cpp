#include <stdio.h>
#include <stdlib.h>

#define __min(a,b) (((a) < (b)) ? (a) : (b))

typedef int treeData;
typedef struct _AATreeNode
{
    treeData        data;
    int             key;
    int             level;
    _AATreeNode    *left;
    _AATreeNode    *right;
}AATreeNode, *AATree;

void aat_insertData  (AATree *, int, const treeData *);
void aat_deleteData  (AATree *, int);
void aat_searchData  (AATree,   int, treeData **);
void aat_deleteTree  (AATree *);
void aat_printTree   (FILE   *, AATree, int);

static AATreeNode *mallocNode()
{                                                           // Function that allocates the memory needed for the node.
    AATreeNode *p;
    p = (AATreeNode*)malloc(sizeof(AATreeNode));
    return p;
}

static AATree Skew(AATree t)
{                                                           // Skew function.
    AATree l;
    if (t == NULL)
        return NULL;
    if (t->left == NULL)
        return t;
    if (t->left->level == t->level)
    {
        l = t->left;
        t->left = l->right;
        l->right = t;
        return l;
    }
    return t;
}

static AATree Split(AATree t)
{                                                           // Split function.
    AATree r;
    if (t == NULL)
        return NULL;
    if (t->right == NULL || t->right->right == NULL)
        return t;
    if (t->right->right->level == t->level)
    {
        r = t->right;
        t->right = r->left;
        r->left = t;
        r->level++;
        return r;
    }
    return t;
}

static AATreeNode *Predecessor(AATreeNode *curNode)
{                                                           // Function that manages predecessor of node.
    curNode = curNode->left;
    while (curNode->right != NULL)
    {
        curNode = curNode->right;
    }
    return curNode;
}

static AATreeNode *Successor(AATreeNode *curNode)
{                                                           // Function that manages successor of node.
    curNode = curNode->right;
    while (curNode->left != NULL)
    {
        curNode = curNode->left;
    }
    return curNode;
}

static AATree Decrease_level(AATree t)
{                                                           // Function that decreases the level of the AA Tree.
    int wdo;
    if (t->left != NULL && t->right != NULL)
    {
        wdo = __min(t->left->level, t->right->level) + 1;
        if (wdo < t->level)
        {
            t->level = wdo;
            if (t->right != NULL && wdo < t->right->level)
            {
                t->right->level = wdo;
            }
        }
    }
    return t;
}

static AATree _InsertData(AATree t, int key, const treeData *data)
{
    if (t == NULL)
    {
        t = mallocNode();                                   // An empty node, we can allocate it.
        t->key = key;
        t->data = *data;
        t->left = NULL;
        t->right = NULL;
        t->level = 1;
        return t;
    }
    else if (key < t->key)                                  // Find position to insert node.
        t->left = _InsertData(t->left, key, data);
    else if (key > t->key)
        t->right = _InsertData(t->right, key, data);
    t = Skew(t);                                            // Follow the path from the new node to node.
    t = Split(t);                                           // We must perform Skew and Split.
    return t;
}

static AATree _DeleteData(AATree t, int key)
{
    AATree m;
    if (t == NULL)
        return NULL;
    if (key < t->key)                                       // Firstly, find the node that you want to delete.
    {
        t->left = _DeleteData(t->left, key);
    }
    else if (key > t->key)
    {
        t->right = _DeleteData(t->right, key);
    }
    else {                                                  // key == t->key
        if (t->left == NULL && t->right == NULL)            // The node has no child tree. We can remove it.
        {
            free(t);
            return NULL;                                    // Empty
        }
        if (t->left == NULL)                                // Only has a right child tree.
        {
            AATree l;
            l = Successor(t);
            t->key = l->key;                                // Exchange position.
            t->data = l->data;
            t->right = _DeleteData(t->right, l->key);       // Delete successor node.
        }
        else {                                              // Only has a left child tree.
            AATree l;
            l = Predecessor(t);
            t->key = l->key;                                // Exchange position.
            t->data = l->data;
            t->left = _DeleteData(t->left, l->key);         // Delete successor node.
        }
    }
    t = Decrease_level(t);
    t = Skew(t);
    m = t->right;
    t->right = Skew(m);
    if (m != NULL && m->right != NULL)
    {
        t->right->right = Skew(m->right);
    }
    t = Split(t);
    t->right = Split(m);
    return t;
}

void aat_insertData(AATree *t, int key, const treeData *data)
{
    *t = _InsertData(*t, key, data);                        // Function that calls _InsertData.
}

void aat_deleteData(AATree *t, int key)
{
    *t = _DeleteData(*t, key);                              // Function that calls _DeleteData.
}

void aat_searchData(AATree bst, int key, treeData **data)
{
    AATree  i;                                              // Search function.
    i = bst;
    while (i != NULL)
    {
        if (key == i->key)
        {
            *data = &(i->data);
            return;
        }
        if (key > i->key)
            i = i->right;
        else
            i = i->left;
    }
    *data = NULL;                                           // Not found.
}

void aat_deleteTree(AATree *t)
{                                                           // Function that deletes the entire AA Tree.
    if (*t == NULL)
        return;
    aat_deleteTree(&(*t)->left);
    aat_deleteTree(&(*t)->right);
    free(*t);
    *t = NULL;
}

void aat_printTree(FILE *f, AATree tree, int level)
{                                                           // Function that prints the AA Tree.
    int i;
    if (!tree)
        return;
    aat_printTree(f, tree->right, level + 1);
    for (i = 0; i < level; i++)
        fprintf(f, "    ");
    fprintf(f, "%d\r\n", tree->key);
    aat_printTree(f, tree->left, level + 1);
}

int main()
{                                                           // Main function to test program.
    AATree aat = NULL;
    int k, i, n, dat, **data;
    printf("How many elements do you want to insert?\n");
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        printf("Give the element you want to insert:\n");
        scanf("%d", &dat);
        aat_insertData(&aat, i, &dat);
    }
    printf("How many elements do you want to delete?\n");
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        printf("Give the key you want to delete:\n");
        scanf("%d", &k);
        aat_deleteData(&aat, k);
    }
    printf("How many elements do you want to search for?\n");
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        printf("Give the element you want to search for:\n");
        data = new int*;
        scanf("%d", &data);
        aat_searchData(aat, i, data);
    }
    aat_printTree(stdout, aat, 0);
    aat_deleteTree(&aat);
    return 0;
}