
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <algorithm>

/*
 * Resources: Linked list https://www.youtube.com/watch?v=H5lkmKkfjD0&list=PLTxllHdfUq4fewZGVFPhzbmHTQC2UH7PJ
 */


class Stack {
private:
    int stackSize;
    struct node {
        std::string data;
        node* next;
    };
    typedef struct node* nodePtr;

    nodePtr stackStart;
    nodePtr curr;
    nodePtr temp;

public:
    Stack();
    void push(std::string data);
    std::string pop();
    void operatorCheck(std::string temporary);
    void delimiterCheck(std::string temporary);
    void nestedLoopCheck(std::string temporary);
    void constantsCheck(std::string temporary);
    void identifierCheck(std::string temporary);
    bool checkDuplicate(std::string info);
    bool isEmpty();
    void print();
};


Stack::Stack() {
    stackStart = NULL;
    curr = NULL;
    temp = NULL;
    stackSize = 0;
}

//Definitions

//Stacks that will be used to parse the input
Stack input;
Stack keywords;
Stack identifiers;
Stack constants;
Stack operators;
Stack delimiters;
Stack syntaxErrors;

std::string line;
std::string result;
bool syntaxError;
int forCount = 0;

void printStacks();
void removeCharsFromString(std::string &temporary, char const* charsToRemove );