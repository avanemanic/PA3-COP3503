/*
 * Ava Nemanic
 * COP3503
 * Spring 2018
 * Project 3
 */

#include "pa3.h"

using namespace std;

int main() {
    //Help with file input: https://stackoverflow.com/questions/6396330/why-should-i-use-c-str-in-functions
    cout << "Please enter the name of the input file:" << endl;
    string fileName;
    cin >> fileName;
    ifstream file(fileName);

    if (!file) {
        cout << "File " << fileName << " not found. Please try again." << endl;
        return 1;
    }

        //Help with reading line by line: https://stackoverflow.com/questions/7868936/read-file-line-by-line
    else {
        while (file >> line) { //Keeps adding lines to the input stack until it runs out
            input.push(line);
        }
    }

    while (!input.isEmpty()) {
        string temporary = input.pop(); //Sets temporary to the top of the stack
        syntaxError = true;//Will change to false if any of the specified keywords/characters are found

        //methods for doing the parsing
        input.operatorCheck(temporary);
        input.delimiterCheck(temporary);
        input.constantsCheck(temporary);
        input.identifierCheck(temporary);
        input.nestedLoopCheck(temporary);

        if (syntaxError) {
            //Just did this in case I don't have time to do the identifiers
            if (temporary != "i" && temporary != "j" && temporary != "k") {
                syntaxErrors.push(temporary);
            }
        }
    }
    printStacks();

    return 0;
}


//Method to check for operators in the file
//Simply looks for the characters and then adds them to the stack
void Stack::operatorCheck(string temporary) {
    for (unsigned int i = 0; i < temporary.length(); i++) {

        //Did this for the same reason as the delimiter
        //If there is an operator it should come directly after another character
        string currChar(1, temporary.at(i));

        //If the substring matches any of the following operators
        if (temporary == "++" || temporary.at(i) == '+' || temporary == "--" || temporary.at(i) == '-' ||
            temporary.at(i) == '*' || temporary.at(i) == '/' || temporary.at(i) == '%' || temporary.at(i) == '=') {

            syntaxError = false;

            //Checks to see if there is a delimiter/parenthesis in the string and pushes just the operator
            if (temporary == "++;" || temporary == "++)" || temporary == "(++" || temporary == "--;" ||
                temporary == "--)" || temporary == "(--") {
                if (!operators.checkDuplicate("++")) {//Adds just the increment operator
                    removeCharsFromString(temporary, (char *) "();");
                    operators.push(temporary);
                }
            }

                //If there's not a delimiter/parenthesis attached, just pushes the regular string
            else if (!operators.checkDuplicate(currChar)) {
                operators.push(currChar);
            }

        }
    }
}


//Method to find delimiters and add them to the stack
void Stack::delimiterCheck(string temporary) {
    for (unsigned int i = 0; i < temporary.length(); i++) {
        //Used this because if there is a delimiter it should always come directly after another char without spaces
        string currChar(1, temporary.at(i));

        //If the character at the index is a delimiter, pushes the substring
        if (temporary.at(i) == ';' || temporary.at(i) == ',') {
            syntaxError = false;

            if (!delimiters.checkDuplicate(currChar)) {//Pushes to the stack if there are no duplicates
                delimiters.push(currChar);
            }
        }
    }
}


//Method pushes keywords to the keyword stack
void Stack::nestedLoopCheck(string temporary) {
    //Checks to see if string matches any of the keywords exactly
    if (temporary == "BEGIN" || temporary == "FOR" || temporary == "END") {
        syntaxError = false;
        if (!keywords.checkDuplicate(temporary)) {
            keywords.push(temporary);
        }
        if (temporary == "FOR") {
            forCount++; //Adds to the for-loop count to check nested loops later
        }
    }
}

//Method for finding constants
void Stack::constantsCheck(string temporary) {

    //Cycles through the inputted string until it finds a digit
    for (unsigned int i = 0; i < temporary.length(); i++) {
        if (isdigit(temporary.at(i))) {
            //Used substrings to remove commas and other parenthesis from the digit string
            if (!constants.checkDuplicate(temporary.substr(0, temporary.length() - 1))) {
                constants.push(temporary.substr(0, temporary.length() - 1));
            }
        }
    }
}

//Method to push identifiers to the stack
void Stack::identifierCheck(string temporary) {

    //Loops through the string until it finds lowercase characters, which would be the identifiers
    for (unsigned int i = 0; i < temporary.length(); i++) {
        if (islower(temporary.at(i))) {
            if (temporary.find(";,=)(") != 0) {//If the string has any excess characters it will return > 0
                removeCharsFromString(temporary, (char *) ";,=)(");//Removes these excess characters
                removeCharsFromString(temporary, (char *) "(");

                //I had an issue where the string contents would be repeated so in that situation this is used
                if (temporary.length() > 1) {//If the length is longer than one, so not a single letter identifier
                    int trueLength = temporary.length() / 2; //Divides in half since half of the string is repeated
                    string realTemp = temporary.substr(0, trueLength);//Then just snips the string at this point
                    if (!identifiers.checkDuplicate(realTemp)) {
                        identifiers.push(realTemp); //Adds to the stack if not a duplicate
                    }
                }

                    //Otherwise adds if not a duplicate
                else {
                    if (!identifiers.checkDuplicate(temporary)) {
                        identifiers.push(temporary); //Adds to the stack if not a duplicate
                    }
                }
            }
        }
    }

}

//Checks to see if the keyword is already in the stack
bool Stack::checkDuplicate(string keyword) {

    if (stackSize != 0) {
        curr = stackStart;

        //Loops through starting at the head until it either finds the keyword or doesn't
        while (curr->next != nullptr) {
            if (curr->data == keyword) {
                break; //Ends loop if keyword is found
            } else {
                curr = curr->next;
            }
        }
        if (curr->data == keyword) {
            return true; //Returns true if keyword is already in the stack
        } else {
            return false;
        }
    }

    return false; //Returns false otherwise

}

//Simply adds new data to the top of the stack
//Same code as what I used in PA2 to add a node
void Stack::push(string data) {
    nodePtr temp = new node;
    temp->next = nullptr;
    temp->data = data;

    if (stackStart != nullptr) {
        curr = stackStart;
        while (curr->next != nullptr) { //Cycles until it reaches an empty node
            curr = curr->next;
        }
        curr->next = temp; //Sets data to first empty node
    } else {
        stackStart = temp; //Sets new data at the top of the stack if there's nothing else there
    }

    stackSize++; //Increments size of the stack
}

//Logic help from https://stackoverflow.com/questions/18430048/stack-using-linked-list-in-c
//and also from https://stackoverflow.com/questions/7543765/pop-function-and-linked-list-implementation
//Removes element from the top of the stack and stores it in a string
string Stack::pop() {
    curr = stackStart;

    //Cycles through the nodes until it reaches a node that's empty
    //If the stack is empty, bypasses this and just returns an empty string at the end
    while (curr->next != nullptr) {
        temp = curr;
        curr = curr->next;
        if (curr->data.empty()) { //Once it reaches an empty node it breaks out of the loop
            break;
        }
    }

    //Pops the node and sets it to empty
    if (curr->data.empty()) {
        result = temp->data;
        temp->data = "";
        stackSize--;
        return result;
    }

        //If it ways always empty just pops the top of the list and returns an empty string
    else {
        result = curr->data;
        curr->data = "";
        stackSize--;
        return result;
    }

}

//Checks to see if the stack is empty or not
//Used to check edge case that inputted file has no data
bool Stack::isEmpty() {
    if (stackSize == 0) {
        return true;
    }
    return false;

}

//Prints out the stack
void Stack::print() {
    //Used for case when there are no syntax errors
    //Prints NA if the syntax errors stack is empty
    if (stackStart == nullptr) {
        cout << " NA" << endl;
    } else {
        if (stackSize != 0) {
            while (stackSize != 0) {
                cout << " " << pop();//Prints and empties out the stacks
            }
        }
    }
}

//Method just for print formatting and to make the main method look neater
void printStacks() {

    cout << "\nThe depth of nested loop(s) is: " << forCount - 1 << "\n" << endl;

    cout << "Keywords:";
    keywords.print();
    cout << endl;

    cout << "Identifier:";
    identifiers.print();
    cout << endl;

    cout << "Constant:";
    constants.print();
    cout << endl;

    cout << "Operators:";
    operators.print();
    cout << endl;

    cout << "Delimiter:";
    delimiters.print();
    cout << endl;

    cout << "\nSyntax Error(s):";
    syntaxErrors.print();
    cout << endl;
}


//Used algorithm from https://stackoverflow.com/questions/5891610/how-to-remove-certain-characters-from-a-string-in-c
//Removes specified characters from a string
//This required #include <algorithm>
void removeCharsFromString(string &temporary, char const *charsToRemove) {
    for (unsigned int i = 0; i < temporary.length(); ++i) {
        temporary.erase(remove(temporary.begin(), temporary.end(), charsToRemove[i]), temporary.end());
    }
}