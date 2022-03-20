#include <stdlib.h>

#include <assert.h>

#define OFFSET 2

//**********************************

struct Node 
{
    int data;

    Node* left;
    Node* right;
};

struct Tree 
{
    Node* head;

    int size;
};

Tree* treeCstr (Node* head)
{
    assert (head != nullptr);
    
    Tree* newTree = (Tree*) calloc (1, sizeof (*newTree));
    newTree->head = head;

    newTree->size = 1;

    return newTree;
}

Node* nodeCstr (int val)
{
    Node* newNode = (Node* ) calloc (1, sizeof(*newNode));

    newNode->data = val;

    newNode->left = nullptr;
    newNode->right = nullptr;

    return newNode;
}

//**********************************

void addNode (Tree* tree, Node* head, Node* newNode)
{
    assert (head != nullptr);
    assert (newNode != nullptr);

    if (head->data < newNode->data)
    {
        if (head->right == nullptr) 
        {
            head->right = newNode;
            ++tree->size;
        }

        else
            addNode (tree, head->right, newNode);
    }

    else
    {
        if (head->left == nullptr) 
        {
            head->left = newNode;
            ++tree->size;
        }

        else
            addNode (tree, head->left, newNode);
    }
}

//**********************************

void prePrint (const Node* node)
{
    assert (node != nullptr);

    printf ("%d\n", node->data);

    if (node->left)
        prePrint (node->left);

    if (node->right)
        prePrint (node->right);
}

//**********************************

void prePrintToFile (FILE* file, const Node* node, int n)
{
    assert (node != nullptr);
    
    fprintf (file, "%*s", n, "{\n");
    fprintf (file, "%*d\n", n, node->data);

    if (node->left)
    {
        prePrintToFile (file, node->left, n + OFFSET);
    }

    if (node->right)
    {
        prePrintToFile (file, node->right, n + OFFSET);
         
    }
   
    fprintf (file, "%*s", n, "}\n");
}

//**********************************

Node* find (Node* currNode, int val)
{
    assert (currNode != nullptr);

    while (!currNode)
    {
        if (currNode->data == val)
            return currNode;
        
        else if (currNode->data < val)
            currNode = currNode->right;

        else 
            currNode = currNode->right;
    }

    return nullptr;
}

//**********************************

void treeDstr (Tree * tree)
{
    assert (tree != nullptr);

    free (tree);
}

void nodeDstr (Node* node)
{
    assert (node != nullptr);

    free (node);
}

//**********************************

void buildTree (FILE* treeFile)
{
    assert (treeFile != nullptr);

    
} 

//**********************************