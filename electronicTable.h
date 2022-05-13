#pragma once

#include <string>
#include <queue>
#include <iomanip>
#include <stack>
#include "validator.h"
#include "cell.h"

//class for electronic table
class ElectronicTable {
private:

    Validator validate;
    int countOfNonEmptyCell;
    bool isSaved;
    int maxRow;
    int maxColumn;

    struct Node {
        Cell key;
        Node *left, *right;
        int height;

        Node(const Cell& other, Node* left = nullptr, Node* right = nullptr, int height = 1) : key(other), left(nullptr), 
        right(nullptr), height(1) {}

    }*root;

private:

    //copy nodes
    Node* copy(Node* root);

    //returns the height of sub tree
    int height(Node *node);
    int max(int a, int b);
    int min(int a, int b);

    //helper function for self-balancing
    Node *rightRotate(Node *rotateNode);

    //helper function for self-balancing
    Node *leftRotate(Node *rotateNode);

    //returns the balanced factor
    int getBalanceFactor(Node *node);

    //helper function which inserts one node in the tree
    Node* insertNode(Node* node, const Cell& cell);

    //insert a new cell
    void insert(const Cell& cell);
    
    //delete the nodes in the tree
    void deleteNodes(Node* root);

    //clear all the information
    void clear();

    //helper function which print the formula in the cell
    bool helperForPrintEXPR(Node* root, int row, int column, bool isInPrintAllFunction);

    //helper function which print the value in the cell
    bool helperForPrintValue(Node* root, int row, int column, bool isInPrintAllFunction);

    //helper function which prints all information in the table like formulas
    void helperForPrintAllEXPR(Node*& root);

    //helper function which prints all information in the table like values
    void helperForPrintAllValue(Node*& root);

    //function for the command PRINT EXPR <address>
    void printEXPR(int row, int column);

    //function for the command PRINT VAL <address>
    void printValue(int row, int column);

    //function for the command PRINT EXPR ALL
    void printAllEXPR();

    //function for the command PRINT VAL ALL
    void printAllValue();

    //helper function which returns the count of non-empty cells in that diapason
    void helperForCount(Node* root, int row1, int column1, int row2, int column2, int& count);

    //function for the command COUNT
    void count(int row1, int column1, int row2, int column2);

    //function which increments int number
    void incrementNumber(Node*& root);

    //function which decrements int number
    void decrementNumber(Node*& root);

    //helper function which increments number or formula
    void helperForIncrement(Node* root, int row, int column);

    //helper function which decrements number or formula
    void helperForDecrement(Node* root, int row, int column);

    //function for the command ++
    void increment(int row, int column);

    //function for the command --
    void decrement(int row, int column);

    //helper function which returns a queue with the correct order of the cells
    void helperForSave(Node* root, std::queue<Cell>& correctOrder);

    //function for the command SAVE
    void saveInfo(const std::string &fileName);

    //helper function which returns the value from a cell
    void helperForGetValue(Node* root, int row, int column, std::string &value) const;

    //getter for value
    std::string getValue(int row, int column) const;

    //helper function which sum all values which are numbers in that diapason
    void helperForSum(Node* root, int row1, int column1, int row2, int column2, double& sum);

    //function for the command SUM
    void sum(int row1, int column1, int row2, int column2);

    //helper function for Load command
    void helperFunctionForLoad(int& row, int& column, int& maxRow, int& maxColumn, const std::string &cell);

    //function for the command LOAD
    void loadFromFile(const std::string &fileName);

    std::string foundedRelativeAddress(const std::vector<std::string>& partsOftheFormula, int i, int row, int column, 
                                       std::stack<double>& values);

    //checks if the founded address is valid
    std::string foundedAbsoluteAddress(const std::vector<std::string>& partsOftheFormula, int i, int row, int column, 
                                       std::stack<double>& values);

    //do the accounts when founds an operator
    std::string foundedOperator(const std::vector<std::string>& partsOftheFormula, int i, std::stack<double>& values, 
                                std::stack<std::string>& operators, bool& isInTrueStatement);

    //helper function which calculates formulas
    std::string calculate(std::vector<std::string> &partsOftheFormula, int row, int column);

    //function which define the precendence between the operators
    int precedence(const std::string &op);
    
    //function which applies operator on two numbers (one number with the operator 'not')
    double applyOperator(const std::string &op, double a, double b);

    //function which returns whether the last operation is right or not
    std::string evaluate(std::stack<double> &value, std::stack<std::string> &op);

    //helper function which checks if the value is operator
    bool ifIsOperator(const std::string &partOfFormula);
    bool is_open_parenthesis(const std::string &value);
    bool is_close_parenthesis(const std::string &value);

    //function which returns the last result
    std::string calculator(const std::string &formula, int row, int column);

    bool foundedNormalText(Node*& root, bool isInPrintAllFunction);
    bool foundedFormula(Node*& root, bool isInPrintAllFunction, int row, int column);
    void calculateIndexesOfAbsoluteAddress(int& r, int& c, const std::string& value);
    void calculateIndexesOfRelativeAddress(int& r, int& c, const std::string& value);
    void checkForInfinityLoop(int& currentRow, int& currentColumn, int& currentCounter, std::string& value);
    void helperForPrintingLargerTableForEXPR(int& digitForRows, int& digitForColumns);
    void helperForPrintingSmallerTableForEXPR(int& digitForRows, int& digitForColumns);
    void helperForPrintingLargerTableForValue(int& digitForRows, int& digitForColumns);
    void helperForPrintingSmallerTableForValue(int& digitForRows, int& digitForColumns);
    std::string calculatingAdresses(int& currentCounter, const std::string& value, std::stack<double>& values, int& r, int& c);
    std::string differentCasesForResult(std::string& value, std::stack<double>& values, int& row, int& column, int& r, int& c);

    //function for the command HELP(more info about the commands)
    void help();
    void commandSET(const std::string& arguments);
    void commandPRINT(const std::string& arguments);
    void commandLOAD(const std::string& arguments);
    void commandIncrement(const std::string& arguments);
    void commandDecrement(const std::string& arguments);
    void commandSUM(const std::string& arguments);
    void commandCOUNT(const std::string& arguments);
    bool commandEXIT();

public:

    ElectronicTable(const ElectronicTable& r) : root(copy(r.root)), maxRow(r.maxRow), maxColumn(r.maxColumn), 
    validate(r.validate), countOfNonEmptyCell(r.countOfNonEmptyCell), isSaved(r.isSaved) {};

    ElectronicTable() : root(nullptr), maxRow(0), maxColumn(0), validate(Validator()), countOfNonEmptyCell(0), isSaved(true) {}

    ~ElectronicTable() {
        clear();
    }

    //function which executes the commands from the user
    bool executeCommand(const std::string &command);

};
