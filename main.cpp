#include <stdio.h>

#include "akinator.h"
#include "fileFunctions.h"
#include "graphvizTree.h"

void checkPrintToFile (Tree* tree);

int main()
{
    Node* node1 = nodeCstr(5);

    Tree* tree = treeCstr(node1);

    Node* node2 = nodeCstr(10);
    Node* node3 = nodeCstr(15);
    Node* node4 = nodeCstr(3);
 
    addNode (tree, tree->head, node2);
    addNode (tree, tree->head, node3);
    addNode (tree, tree->head, node4);

    prePrint (tree->head);
    checkPrintToFile (tree);
    treeGraph (tree);

    treeDstr (tree);
    nodeDstr (node1);
    nodeDstr (node2);
    nodeDstr (node3);
    nodeDstr (node4);

    return 0;
}

void checkPrintToFile (Tree* tree)
{
    char* fileName = (char*) calloc(FILENAME_MAX, sizeof(*fileName));
    scanInputName(fileName);
    FILE* text = fopen(fileName, "w");

    prePrintToFile (text, tree->head, 0);

    free (fileName);

    fclose (text);
}