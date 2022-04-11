

#include <cstdlib>
#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <cstring>

#ifndef PROTECTION_H_INCLUDED
#define PROTECTION_H_INCLUDED

enum keyOfMistake {
        nullStackPtr = -100,
        nullPtrOnStack,
        negativeSize,
        noMistake,
        leftCanary,
        rightCanary,
        arrayLeftCanary,
        arrayRightCanary,
        hashStackMistake,
        hashArrayMistake
    };

char* strOfMistakes[] = {"nullStackPtr"    , "nullPtrOnStack"  ,
                         "negativeSize"    , "noMistake"       ,
                         "leftCanary"      , "rightCanary"     ,
                         "arrayLeftCanary" , "arrayRightCanary",
                         "hashStackMistake", "hashArrayMistake"};

static const unsigned long long CANARY_LEFT  = 117;
static const unsigned long long CANARY_RIGHT = 119;

static FILE* logfile = fopen("logfile.txt", "w");

#endif // PROTECTION_H_INCLUDED

#define INFORMATION __FILE__, __LINE__, __PRETTY_FUNCTION__
#define printInformation printf("\x1b[32mFilename: %s, line of program: %d, function: %s\x1b[0m", INFORMATION);

#define CAT(x,y) x##y
#define CATSTR(x,y) #x#y

#define myStack(x) CAT(myStack_, x)
#define myStackStr(x) CATSTR(myStack_, x)

#define getName(name) #name

#define TYPE int 

struct myStack(TYPE) {
  unsigned long long canaryLeft = CANARY_LEFT;

  TYPE* ptrOnStack = nullptr;

  size_t sizeOfStack = -1;
  const size_t maxSizeOfStack = 100;

  unsigned long long hashStack = 0;
  unsigned long long hashArray = 0;

  unsigned long long canaryRight = CANARY_RIGHT;
};

void stackConstructor(struct myStack(TYPE)* Stack);
void stackDestructor(struct myStack(TYPE)* Stack);

void stackAssert(struct myStack(TYPE)* Stack, const char* fileOfMistake, int stringOfMistake, const char* functionOfMistake);
int stackIsGood(struct myStack(TYPE)* Stack);

void stackDump(struct      myStack(TYPE)* Stack, int keyOfMistake   ,
               const char* fileOfMistake       , int stringOfMistake,
               const char* functionOfMistake);

unsigned long long hashFunction(const char* str);

void stackPush(struct myStack(TYPE)* Stack, const TYPE value);
TYPE stackPop(struct myStack(TYPE)* Stack);
void stackResize(struct myStack(TYPE)* Stack, size_t newSize);
void printStack(struct myStack(TYPE)* Stack);
void printStackToLogfile(struct myStack(TYPE)* Stack);

unsigned long long hashForArray(struct myStack(TYPE)* Stack);
unsigned long long hashForStack(struct myStack(TYPE)* Stack);

char* strKeyOfMistake(const int keyOfMistake);

void stackConstructor(struct myStack(TYPE)* Stack) {
    Stack->ptrOnStack = (TYPE*) calloc(Stack->maxSizeOfStack, sizeof(Stack->maxSizeOfStack) + 2 * sizeof(int)); // +2 for canary

    *(((int*)Stack->ptrOnStack) + 0) = CANARY_LEFT;

    if (strcmp(myStackStr(TYPE), "myStack_int") == 0) {
        Stack->ptrOnStack += 1;
    }

    if (strcmp(myStackStr(TYPE), "myStack_char") == 0) {
        Stack->ptrOnStack += sizeof(int);
    }

    *(((int*)Stack->ptrOnStack) + Stack->maxSizeOfStack) = CANARY_RIGHT;

    Stack->sizeOfStack = 0;

    Stack->hashArray = hashForArray(Stack);
    Stack->hashStack = hashForStack(Stack);

    stackAssert (Stack, INFORMATION);
}

void stackDestructor(struct myStack(TYPE)* Stack) {
    stackAssert (Stack, INFORMATION);

     if (strcmp(myStackStr(TYPE), "myStack_int") == 0) {
        free(--(Stack->ptrOnStack));
    }

    if (strcmp(myStackStr(TYPE), "myStack_char") == 0) {
        free(Stack->ptrOnStack - sizeof(int));
    }

    //free(--(Stack->ptrOnStack)); //for canary
}

void stackPush(struct myStack(TYPE)* Stack, const TYPE value) {
    stackAssert (Stack, INFORMATION);

    if (Stack->sizeOfStack + 1 > Stack->maxSizeOfStack) {
        Stack->ptrOnStack = (TYPE*) realloc(Stack->ptrOnStack, Stack->sizeOfStack * 2);
    }

    *(Stack->ptrOnStack + Stack->sizeOfStack++) = value;

    Stack->hashArray = hashForArray(Stack);
    Stack->hashStack = hashForStack(Stack);

    stackAssert (Stack, INFORMATION);
}

TYPE stackPop(struct myStack(TYPE)* Stack) {
    stackAssert (Stack, INFORMATION);

    TYPE value = *(Stack->ptrOnStack);

    *(Stack->ptrOnStack + Stack->sizeOfStack-- - 1) = 0;

    Stack->hashArray = hashForArray(Stack);
    Stack->hashStack = hashForStack(Stack);

    stackAssert (Stack, INFORMATION);

    return value;
}

int stackIsGood(struct myStack(TYPE)* Stack) {
    if (!Stack) {
        return nullStackPtr;
    }

    if (!Stack->ptrOnStack){
        return nullPtrOnStack;
    }

    if (Stack->sizeOfStack < 0) {
        return negativeSize;
    }

    if (Stack->canaryLeft != CANARY_LEFT) {
        return leftCanary;
    }

    if (Stack->canaryRight != CANARY_RIGHT) {
        return rightCanary;
    }

    if (*((int*)Stack->ptrOnStack - 1) != CANARY_LEFT) {
        printf("%d\n", *((int*)Stack->ptrOnStack - 1));
        return arrayLeftCanary;
    }

    if (*((int*)Stack->ptrOnStack + Stack->maxSizeOfStack) != CANARY_RIGHT) {
        return arrayRightCanary;
    }

    if (Stack->hashArray != hashForArray(Stack)) {
        return hashArrayMistake;
    }

    if (Stack->hashStack != hashForStack(Stack)) {
        return hashStackMistake;
    }

    return noMistake;
}

void stackResize(struct myStack(TYPE)* Stack, size_t newSize) {
    stackAssert (Stack, INFORMATION);

    Stack->ptrOnStack = (TYPE*) realloc(Stack->ptrOnStack, newSize);

    Stack->hashArray = hashForArray(Stack);
    Stack->hashStack = hashForStack(Stack);

    stackAssert (Stack, INFORMATION);
}

void printStack(struct myStack(TYPE)* Stack) {
    stackAssert (Stack, INFORMATION);

    size_t index = 0;

    while (index < Stack->sizeOfStack) {
        if (strcmp(myStackStr(TYPE), "myStack_int") == 0) {
            printf("%d ", *((Stack->ptrOnStack) + index));
        }

        if (strcmp(myStackStr(TYPE), "myStack_char") == 0) {
            printf("%c ", *((Stack->ptrOnStack) + index));
        }

        ++index;
    }

    printf("\n");
}

void printStackToLogfile(struct myStack(TYPE)* Stack) {
    size_t index = 0;

    fprintf(logfile, "Left canary = %d\n",  Stack->canaryLeft);
    fprintf(logfile, "Right canary = %d\n", Stack->canaryRight);

    while (index < Stack->sizeOfStack) {
        if (strcmp(myStackStr(TYPE), "myStack_int") == 0) {
            fprintf(logfile, "Stack[%d] = %d\n", index, *((Stack->ptrOnStack) + index));
        }

        if (strcmp(myStackStr(TYPE), "myStack_char") == 0) {
            fprintf(logfile, "Stack[%d] = %c\n", index, *((Stack->ptrOnStack) + index));
        }

        ++index;
    }

    for (index; index < Stack->sizeOfStack + 3; ++index) {
        if (strcmp(myStackStr(TYPE), "myStack_int") == 0) {
            fprintf(logfile, "Stack[%d] = %d\n", index, *((Stack->ptrOnStack) + index));
        }

        if (strcmp(myStackStr(TYPE), "myStack_char") == 0) {
            fprintf(logfile, "Stack[%d] = %c\n", index, *((Stack->ptrOnStack) + index));
        }
    }

    fprintf(logfile, "\n");
}

void stackCopy(struct myStack(TYPE)* stackTo, struct myStack(TYPE)* stackFrom) {

}

void stackAssert(struct myStack(TYPE)* Stack, const char* fileOfMistake, int stringOfMistake, const char* functionOfMistake) {
    int keyOfMistake = 0;

    if ((keyOfMistake = stackIsGood(Stack)) == nullStackPtr     ||
        (keyOfMistake = stackIsGood(Stack)) == nullPtrOnStack   ||
        (keyOfMistake = stackIsGood(Stack)) == negativeSize     ||
        (keyOfMistake = stackIsGood(Stack)) == leftCanary       ||
        (keyOfMistake = stackIsGood(Stack)) == rightCanary      ||
        (keyOfMistake = stackIsGood(Stack)) == hashStackMistake ||
        (keyOfMistake = stackIsGood(Stack)) == hashArrayMistake ||
        (keyOfMistake = stackIsGood(Stack)) == arrayLeftCanary  ||
        (keyOfMistake = stackIsGood(Stack)) == arrayRightCanary) {
        stackDump(Stack, keyOfMistake, fileOfMistake, stringOfMistake, functionOfMistake);
    }
}

void stackDump(struct myStack(TYPE)* Stack, int keyOfMistake,
               const char* fileOfMistake, int stringOfMistake, const char* functionOfMistake) {
     fprintf(logfile, "\x1b[32m  Filename: %s, line of program: %d, function: %s  \x1b[0m\n\n",
             fileOfMistake, stringOfMistake, functionOfMistake);


    fprintf(logfile, "Pointer on stack(array) = %p\n", Stack->ptrOnStack);
    fprintf(logfile, "Stack size = %zd\n", Stack->sizeOfStack);
    fprintf(logfile, "Pointer on Stack: %p\n", Stack);
    fprintf(logfile, "Key of mistake: %s\n", strOfMistakes[100 + keyOfMistake]);
    fprintf(logfile, "Stack type: %s\n", myStackStr(TYPE));

    fprintf(logfile, "\n");
    fprintf(logfile, "left array canary: %d, right array canary: %d\n",
                     *((int*)Stack->ptrOnStack - 1), *((int*)Stack->ptrOnStack + Stack->maxSizeOfStack));

    fprintf(logfile, "left stack canary: %d, right array canary: %d\n",
                     Stack->canaryLeft, Stack->canaryRight);

    switch (keyOfMistake) {
        case nullStackPtr:
            fprintf(logfile, "key of mistake: null pointer on struct MyStack\n");
            break;
        case nullPtrOnStack:
            fprintf(logfile, "key of mistake: null pointer on stack\n");
            break;
        case negativeSize:
            fprintf(logfile, "key of mistake: negative size of stack\n");
            break;
    }

    fprintf(logfile, "\n");

    printStackToLogfile(Stack);
}

unsigned long long hashForArray(struct myStack(TYPE)* Stack) {
    unsigned long long Hash = 0;

    size_t index = 0;
    while (index++ != Stack->maxSizeOfStack) {
        Hash += *(Stack->ptrOnStack + index);
    }

    Hash %= 101;

    return Hash;
}

unsigned long long hashForStack(struct myStack(TYPE)* Stack) {
    unsigned long long Hash = 0;

    Hash = ((unsigned long long)(*Stack->ptrOnStack) + Stack->sizeOfStack
            + Stack->canaryLeft + Stack->canaryRight) % 101;

    return Hash;
}