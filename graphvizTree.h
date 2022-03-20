#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

void treeBegin      (FILE* graphfile);
void treeEnd        (FILE* graphfile);
void treeInf        (FILE* graphfile, const Tree* tree);
void addNode        (FILE* graphfile, const Node* node);
void treeRel        (FILE* graphfile, const Node* currNode);
void addAllNodes    (FILE* graphfile, const Node* currNode);

void treeBegin (FILE* graphfile)
{
    assert (graphfile != nullptr);

    fprintf (graphfile, "digraph G{\n");
}

void treeInf (FILE* graphfile, const Tree* tree)
{
    assert (graphfile != nullptr);
    assert (tree != nullptr);

     fprintf(graphfile, 
                    "   first [shape=record,label=\" tree head\\n %p | tree size\\n %d\" ];\n", 
                        tree->head, tree->size);
}

void addNode (FILE* graphfile, const Node* node)
{
    assert (graphfile != nullptr);
    assert (node != nullptr);

    fprintf(graphfile, "   Node%p  [shape = record, label = \"<a%p> address\\n %p | value %2d | <f%p> left | <ff%p> right\"];\n", 
                           node, node, node, node->data, node, node);
}

void treeRel (FILE* graphfile, const Node* currNode)
{
    assert (graphfile != nullptr);
    assert (currNode != nullptr);

    if (currNode->left)
    {
        fprintf (graphfile, "   edge[color=\"darkgreen\",fontcolor=\"blue\",fontsize=12];\n");
        fprintf (graphfile, "   Node%p:  <f%p>  -> Node%p: <a%p>;\n", 
                                currNode, currNode, currNode->left, currNode->left);
        treeRel (graphfile, currNode->left);
    }

    if (currNode->right)
    {
        fprintf (graphfile, "   edge[color=\"darkblue\",fontcolor=\"blue\",fontsize=12];\n");
        fprintf (graphfile, "   Node%p:  <ff%p>  -> Node%p: <a%p>;\n", 
                                currNode, currNode, currNode->right, currNode->right);
        treeRel (graphfile, currNode->right);
    }
}

void addAllNodes (FILE* graphfile, const Node* currNode)
{
    assert (graphfile != nullptr);
    assert (currNode != nullptr);

    addNode (graphfile, currNode);

    if (currNode->left)
        addAllNodes (graphfile, currNode->left);

    if (currNode->right)
        addAllNodes (graphfile, currNode->right);

}

void treeGraph (Tree* tree)
{
    assert (tree != nullptr);

    FILE * graphfile = fopen ("treeGr.txt", "w");

    treeBegin (graphfile);
    treeInf   (graphfile, tree);

    addAllNodes (graphfile, tree->head);

    system ("dot -Tpng treeGr.txt > treeGt.png");

    treeRel (graphfile, tree->head);
    treeEnd  (graphfile);

    fclose (graphfile);
}

void treeEnd (FILE* graphfile) 
{
    assert (graphfile != nullptr);

    fprintf (graphfile, "}");
}

#endif 