#include <stdlib.h>

#include <assert.h>

#define OFFSET 2

#define MAXANS 30

//**********************************

#include "fileFunctions.h"
#include "queue.h"

//**********************************

struct Node 
{
    char* str;

    Node* left;
    Node* right;
};

struct Tree 
{
    Node* head;

    int size;
};

//**********************************

void treeGraph      (Tree* tree);

//**********************************

Tree* treeCstr (Node* head);

//**********************************

Node* nodeCstr (char* str);

//**********************************

void treeDstr (Tree * tree);

//**********************************

void nodeDstr (Node* node);

//**********************************

Tree* treeCstr (Node* head)
{
    assert (head != nullptr);
    
    Tree* newTree = (Tree*) calloc (1, sizeof (*newTree));
    newTree->head = head;

    newTree->size = 1;

    return newTree;
}

//**********************************

Node* nodeCstr (char* str)
{
    Node* newNode = (Node* ) calloc (1, sizeof(*newNode));

    newNode->str = str;

    newNode->left = nullptr;
    newNode->right = nullptr;

    return newNode;
}

//**********************************

void treeDstr (Tree * tree)
{
    assert (tree != nullptr);

    nodeDstr (tree->head);

    free (tree);
}

//**********************************

void nodeDstr (Node* node)
{
    assert (node != nullptr);

    if (node->left)
        nodeDstr (node->left);

    if (node->right)
        nodeDstr (node->right);

    free (node->str);
    free (node);
}

//**********************************

void addNode (Tree* tree, Node* head, Node* newNode);

//**********************************

void prePrint (const Node* node);

//**********************************

void prePrintToFile (FILE* file, const Node* node);

//**********************************

void checkPrintToFile (Tree* tree);

//**********************************

int isYes (char* str);
int isNo  (char* str);

//**********************************

int doPlay();

//**********************************

void launch(Node* node, Tree* tree);

//**********************************

void fullInformation (Tree* tree, Node* node, queue* Queue);

//**********************************

Tree* buildTree (FILE* treeFile);

//**********************************

void addNode (Tree* tree, Node* head, Node* newNode)
{
    assert (head != nullptr);
    assert (newNode != nullptr);

    if (head->str < newNode->str)
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

    printf ("<%s>\n", node->str);

    if (node->left)
        prePrint (node->left);

    if (node->right)
        prePrint (node->right);
}

//**********************************

void prePrintToFile (FILE* file, const Node* node)
{
    assert (node != nullptr);
    
    fprintf (file, "%s", "{\n");
    fprintf (file, "%s\n", node->str);

    if (node->left)
    {
        prePrintToFile (file, node->left);
    }

    if (node->right)
    {
        prePrintToFile (file, node->right);
         
    }
   
    fprintf (file, "%s", "}\n");
}

//**********************************

Tree* begin()
{
    Tree* newTree = (Tree*) calloc (1, sizeof(*newTree));

    char* strHead =  (char*) calloc  (MAXANS, sizeof(*strHead));
    char* strLeft =  (char*) calloc  (MAXANS, sizeof(*strLeft));
    char* strRight = (char*) calloc  (MAXANS, sizeof(*strRight));

    sprintf (strHead , "animal?"     );
    sprintf (strLeft , "Poltorashka");
    sprintf (strRight, "Ovchinkin"  );

    Node* NodeHead =  (Node* )  calloc(1, sizeof(*NodeHead));
    Node* NodeLeft =  (Node* )  calloc(1, sizeof(*NodeLeft));
    Node* NodeRight = (Node* )  calloc(1, sizeof(*NodeRight));

    NodeHead->str  = strHead;
    NodeLeft->str  = strLeft;
    NodeRight->str = strRight;

    newTree->head = NodeHead;
    NodeHead->left = NodeLeft;
    NodeHead->right = NodeRight;

    newTree->size = 3;

    return newTree;
}

//**********************************

int doPlay()
{
    printf ("Do you want to play?\n");
    char ans[MAXANS];

    scanf ("%s", ans);

    if (isYes (ans))
        return 1;

    else if (isNo (ans))
        return 0;
    
    return -1;
}

//**********************************

void addNewSubj (Node* node) // узел, на котором не было отгадано 
{
    printf ("Please, enter the thing you wished: It is:\n");
    char* subj = (char*) calloc (MAXANS, sizeof (*subj));

    scanf ("%s", subj);

    Node* newNodeLeft = nodeCstr(subj);

    printf ("Please, enter the difference between this things (persons). Your thing (person) is:\n");
    char* diff = (char*) calloc (MAXANS, sizeof (*diff));

    scanf ("%s", diff);

    sprintf (diff + strlen(diff), "?");
    Node* newNodeRight = nodeCstr(node->str);
    node->str = diff; 

    node->right = newNodeRight;
    node->left  = newNodeLeft;
}

//**********************************

void launch(Node* node, Tree* tree)
{
    struct queue Queue;
    queueCstr (&Queue);

    while (node->left != nullptr && node->right != nullptr)
    {
        printf ("He (she, it) is : %s?\n", node->str);

        char ans[MAXANS];
        scanf ("%s", ans);

        if (isYes (ans))
        {
            node = node->left;
            qPush (&Queue, YES);
        }

        else if (isNo (ans))
        {
            node = node->right;
            qPush (&Queue, NO);
        }

        else 
        {
            printf ("Please, enter yes or no:\n");
            launch (tree->head, tree);   
        }
    }   

    if (node->left == nullptr && node->right == nullptr)
    {
        printf ("I think it is: %s\n", node->str);
        printf ("Is it correct answer?\n");

        char ans[MAXANS];
        scanf ("%s", ans);

        if (isYes (ans))
        {
            printf ("I won the game! The game is ended!\n");
            printf ("The full information:\n");
            fullInformation (tree, tree->head, &Queue);

            treeGraph (tree);

            FILE* textFile = fopen ("tree.txt", "w");

            prePrintToFile (textFile, tree->head);

            treeDstr (tree);
            queueDstr (&Queue);

            exit(1);
        }

        else if (isNo (ans))
        {
            addNewSubj (node);
            launch (tree->head, tree);
        }

        else
        {
            printf ("Please, enter yes or no.\n");
            launch (tree->head, tree);
        }
        
    }

}

//**********************************

int isYes (char* str)
{
    return strcmp(str, "yes") == 0 || strcmp(str, "Yes") == 0;
}

//**********************************

int isNo (char* str)
{
    return strcmp(str, "no") == 0 || strcmp(str, "No") == 0;
}

//**********************************

void checkPrintToFile (Tree* tree)
{
    char* fileName = (char*) calloc(FILENAME_MAX, sizeof(*fileName));
    scanInputName(fileName);
    FILE* text = fopen(fileName, "w");

    prePrintToFile (text, tree->head);

    free (fileName);

    fclose (text);
}

//**********************************

void fullInformation (Tree* tree, Node* node, queue* Queue)
{
    assert (tree  != nullptr);
    assert (node  != nullptr);
    assert (Queue != nullptr);

    while (Queue->size > 0)
    {
        int val = qPop (Queue);

        if (val == YES)
        {
            printf ("Your thing (person) is %s\n", node->str);
            node = node->left;
        }
        
        else if (val == NO)
            node = node->right;
    }
}

//**********************************

Node* readNode (FILE* treeFile, char** arrayOfPtrOnStr, int* currLine);

//**********************************

void addNodeFromFile (Node* PrevNode, Node* currNode, int* currLine, char** arrayOfptrOnStrings, FILE* textfile);

//**********************************

Node* readNode (FILE* treeFile, char** arrayOfPtrOnStr, int* currLine)
{
    assert (treeFile);
    assert (arrayOfPtrOnStr);
    assert (currLine);

    if (strcmp (*(arrayOfPtrOnStr + *currLine), "{") == 0)
    {
        ++*currLine;
        char* nodeStr = *(arrayOfPtrOnStr + *currLine);
        ++*currLine; 

        Node * newNode = nodeCstr (nodeStr);

        return newNode;
    }

    return nullptr;
}

//**********************************

Tree* buildTree (FILE* treeFile)
{
    size_t fileSize     = sizeOfFile(treeFile);
    char* str = (char*) calloc(fileSize, sizeof(*str));
    readTextFromFile(treeFile, str, fileSize);


    size_t nLines = symbolAmount(str, fileSize, '\n');

    char** arrayOfptrOnStrings = (char**) calloc (nLines, sizeof(*arrayOfptrOnStrings));
    fillArrayOfPtrOnStrings(arrayOfptrOnStrings, str, fileSize);

    // Сейчас есть фактически массив указателей на считанные строки 

    int currLine = 0;

    Node* headTree = readNode (treeFile, arrayOfptrOnStrings, &currLine);
    Tree* newTree = treeCstr (headTree);
    
    Node * currNode = newTree->head;

    addNodeFromFile (newTree->head, currNode, &currLine, arrayOfptrOnStrings, treeFile);

    return newTree;
}

//**********************************

void addNodeFromFile (Node* PrevNode, Node* currNode, int* currLine, char** arrayOfptrOnStrings, FILE* textfile)
{
    assert (PrevNode != nullptr);
    assert (currNode);
    assert (currLine);
    assert (arrayOfptrOnStrings);
    assert (textfile);

    if (strcmp ("{", *(arrayOfptrOnStrings + *currLine)) == 0)
    {
        Node* prevNode = currNode;

        if (currNode->left == nullptr)
        {
            printf ("First checker: <<%s>, <%p>, <%p>, Nexr str: <%s>\n", 
                    currNode->str, currNode->left, currNode->right, *(arrayOfptrOnStrings + *currLine + 1));

            Node* newNode = readNode (textfile, arrayOfptrOnStrings, currLine);
            currNode->left = newNode;

            printf ("First checker: <<%s>, <%p>, <%p>, Nexr str: <%s>\n", 
                    currNode->str, currNode->left, currNode->right, *(arrayOfptrOnStrings + *currLine + 1));
            addNodeFromFile (newNode, currNode->left, currLine, arrayOfptrOnStrings, textfile);
        }

        if (currNode->left != nullptr && currNode->right == nullptr)
        {
            printf ("Second checker: <<%s>, <%p>, <%p>, Nexr str: <%s>\n",
                    currNode->str, currNode->left, currNode->right, *(arrayOfptrOnStrings + *currLine + 1));
            Node* newNode = readNode (textfile, arrayOfptrOnStrings, currLine);
            currNode->right = newNode;
            printf ("Second checker: <<%s>, <%p>, <%p>, Nexr str: <%s>, curr str: <%s>\n",
                    currNode->str, currNode->left, currNode->right, *(arrayOfptrOnStrings + *currLine + 1), *(arrayOfptrOnStrings + *currLine));
            addNodeFromFile (newNode, currNode->right, currLine, arrayOfptrOnStrings, textfile);
        }
    }

    else if (*(arrayOfptrOnStrings + *currLine + 1) != nullptr &&
             strcmp ("}", *(arrayOfptrOnStrings + *currLine)) == 0)
    {
        printf ("\n\n Dangermoment*: Curr str: <%s>, Nexr str: <%s>\n", 
                     *(arrayOfptrOnStrings + *currLine), *(arrayOfptrOnStrings + *currLine + 1));
        ++*currLine;

        if (*(arrayOfptrOnStrings + *currLine + 1) != nullptr)
        {
             printf ("\n\n Dangermoment*: Curr str: <%s>, Nexr str: <%s>\n", 
                     *(arrayOfptrOnStrings + *currLine), *(arrayOfptrOnStrings + *currLine + 1));
        }

        if (*(arrayOfptrOnStrings + *currLine + 1) != nullptr &&
            strcmp ("{", *(arrayOfptrOnStrings + *currLine + 1)) == 0)
        {
            printf("\n\nI was here\n\n");
            *currLine += 1;

            return;
        }

        else if (*(arrayOfptrOnStrings + *currLine + 1) != nullptr &&
                strcmp ("}", *(arrayOfptrOnStrings + *currLine + 1)) == 0)
        {   
            ++*currLine;

            printf("\n\nI was here\n\n");

            if (*(arrayOfptrOnStrings + *currLine + 1) != nullptr)
            {
                printf("\n\nI was here TEEEST\n\n");
                printf ("Danger moment: <<%s>, Nexr str: <%s>>\n", 
                    currNode->str,  *(arrayOfptrOnStrings + *currLine + 1));
                printf("\n\nI was here\n\n");
            }

            return;
        }
    }

}

void launchReadFromFile ()
{
    FILE* textTree = fopen ("tree.txt", "r");

    Tree* tree = buildTree (textTree);
    treeGraph (tree);
}