#include <stdio.h>

#include "akinator.h"
#include "graphvizTree.h"
#include <stdlib.h>

int startTheGame ()
{
    int willPlay = doPlay();

    if (willPlay == 1)
        return 1;
    
    else if (willPlay == 0)
        return 0;

    else if (willPlay == -1)
    {
        printf ("Please, enter the answer correctly: yes or no\n");
        return startTheGame ();
    }

    return -1;
}

int main()
{
    //Tree* tree = begin();

    int willPlay = startTheGame ();
 
    //  добавить сравнения 
    if (willPlay == 1)
    {
        Tree* tree =  launchReadFromFile ();
        
        launch(tree->head, tree);
    }
    
    else if (willPlay == 0)
    {
        printf ("The program is ended.\n");
        //treeDstr (tree);
    }

    else 
        printf ("Unknown Error");

    return 0;
}