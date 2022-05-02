#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <fstream>
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
    Node* copy(Node* root) {

        return root ?
            new Node(root->key, copy(root->left), copy(root->right), root->height) :
            root;
    }

    //returns the height of sub tree
    int height(Node *node) {
        if (node == nullptr)
        return 0;

        return node->height;
    }

    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    int min(int a, int b) {
        return (a < b) ? a : b;
    }

    //helper function for self-balancing
    Node *rightRotate(Node *rotateNode) {

        Node *leftChildOfNode = rotateNode->left;
        Node *rigthChildOfLeft = leftChildOfNode->right;

        leftChildOfNode->right = rotateNode;
        rotateNode->left = rigthChildOfLeft;

        rotateNode->height = max(height(rotateNode->left), height(rotateNode->right)) + 1;
        leftChildOfNode->height = max(height(leftChildOfNode->left), height(leftChildOfNode->right)) + 1;

        return leftChildOfNode;

    }

    //helper function for self-balancing
    Node *leftRotate(Node *rotateNode) {

        Node *rightChildOfNode = rotateNode->right;
        Node *leftChildOfRight = rightChildOfNode->left;

        rightChildOfNode->left = rotateNode;
        rotateNode->right = leftChildOfRight;

        rotateNode->height = max(height(rotateNode->left), height(rotateNode->right)) + 1;
        rightChildOfNode->height = max(height(rightChildOfNode->left), height(rightChildOfNode->right)) + 1;

        return rightChildOfNode;

    }

    //returns the balanced factor
    int getBalanceFactor(Node *node) {

        if (node == nullptr)
            return 0;

        return height(node->left) - height(node->right);

    }

    //helper function which inserts one node in the tree
    Node* insertNode(Node* node, const Cell& cell) {
        
        if (node == nullptr) {
            if(cell.getRow() > maxRow) {
                maxRow = cell.getRow();
            }

            if(cell.getColumn() > maxColumn) {
                maxColumn = cell.getColumn();
            }

            Node* newNode = new Node(cell, nullptr, nullptr, 1);
            countOfNonEmptyCell++;      
            return newNode;
        }       
        if (cell < node->key)
            node->left = insertNode(node->left, cell);
        else if (cell > node->key)
            node->right = insertNode(node->right, cell);
        else {
            node->key.setValue(cell.getValue());
            return node;
        }

        node->height = 1 + max(height(node->left), height(node->right));
        int balanceFactor = getBalanceFactor(node);

        if (balanceFactor > 1) {
            if (cell < node->left->key) {
                return rightRotate(node);
            } 
            else if (cell > node->left->key) {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }
        }

        if (balanceFactor < -1) {
            if (cell > node->right->key) {
                return leftRotate(node);
            } 
            else if (cell < node->right->key) {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }
        }
   
        return node;

    }

    //insert a new cell
    void insert(const Cell& cell) {    
        root = insertNode(root, cell);
    }
    
    //delete the nodes in the tree
    void deleteNodes(Node* root) {
        if(root) {
            deleteNodes(root->left);
            deleteNodes(root->right);
            delete root;
        }
    }

    //clear all the information
    void clear() {
        deleteNodes(root);
        root = nullptr;
        maxRow = 0;
        maxColumn = 0;
        isSaved = true;
        countOfNonEmptyCell = 0;
    }

    //helper function which print the formula in the cell
    bool helperForPrintEXPR(Node* root, int row, int column, bool isInPrintAllFunction) {

        if(row > maxRow || (row == maxRow && column > maxColumn) || row < 0 || column < 0) {
            throw std::out_of_range("This address does not exist");
        }

        if(root == nullptr) {
            return false;
        }

        if(root->key.getRow() == row && root->key.getColumn() == column) {
            if(isInPrintAllFunction && root->key.getValue().length() > 12) {
                std::string value = root->key.getValue().substr(0, 9) + "...";
                std::cout << value;
                return true;
            }
            std::cout << root->key.getValue();
            return true;
        }

        else if((root->key.getRow() > row) || (root->key.getRow() == row && root->key.getColumn() > column)) {
            helperForPrintEXPR(root->left, row, column, isInPrintAllFunction);
        }

        else if((root->key.getRow() < row) || (root->key.getRow() == row && root->key.getColumn() < column)) {
            helperForPrintEXPR(root->right, row, column, isInPrintAllFunction);
        }

    }

    //helper function which print the value in the cell
    bool helperForPrintVal(Node* root, int row, int column, bool isInPrintAllFunction) {

        if(row > maxRow || (row == maxRow && column > maxColumn) || row < 0 || column < 0) {
            throw std::out_of_range("This address does not exist");
        }

        if(root == nullptr) {
            return false;
        }

        if(root->key.getRow() == row && root->key.getColumn() == column) {

            if(root->key.getValue()[0] == '\"') {
                if(isInPrintAllFunction && root->key.getValue().length() > 12) {
                    std::string value = root->key.getValue().substr(0, 9) + "...";
                    std::cout << value;
                    return true;
                }
                std::cout << root->key.getValue();
            }

            else if(root->key.getValue()[0] == '=') {
                std::string result = calculator(root->key.getValue().substr(2), row, column);
                if(isInPrintAllFunction && result.length() > 12) {
                    std::string value = result.substr(0, 9) + "...";
                    std::cout << value;
                    return true;
                }
                std::cout << result;
            }

            else if(validate.functionForAbsoluteAddressing(root->key.getValue()) || 
                    validate.functionForRelativeAddressing(root->key.getValue())) {
                
                int currentCounter = 1;
                std::string value = root->key.getValue();
                int currentRow = row, currentColumn = column;
                while((validate.functionForAbsoluteAddressing(value) || validate.functionForRelativeAddressing(value)) && 
                       currentCounter < countOfNonEmptyCell - 1) {

                    if(validate.functionForAbsoluteAddressing(value)) {

                        int r = std::stoi(value.substr(1, value.find('C') - 1));
                        int c = std::stoi(value.substr(value.find('C') + 1));

                        value = getValue(r, c);

                    }

                    else if(validate.functionForRelativeAddressing(value)) {

                        int r = std::stoi(value.substr(2, value.find('C') - 2));
                        int c = std::stoi(value.substr(value.find('C') + 2, value.size() - 2));

                        value = getValue(r + currentRow, c + currentColumn);
                        currentRow += r;
                        currentColumn += c;
                    }
                    currentCounter++;

                }
    
                if(currentCounter == countOfNonEmptyCell - 1) {
                    throw std::invalid_argument("In the table exist infinity loop from addresses!");
                }

                if(isInPrintAllFunction && value.length() > 12) {
                    std::string newValue = value.substr(0, 9) + "...";
                    std::cout << newValue;
                    return true;
                }

                std::cout << value;

            }
            return true;
        }

        else if((root->key.getRow() > row) || (root->key.getRow() == row && root->key.getColumn() > column)) {
            helperForPrintVal(root->left, row, column, isInPrintAllFunction);
        }

        else if((root->key.getRow() < row) || (root->key.getRow() == row && root->key.getColumn() < column)) {
            helperForPrintVal(root->right, row, column, isInPrintAllFunction);
        }

    }

    //helper function which prints all information in the table like formulas
    void helperForPrintAllEXPR(Node* root) {
        
        int digitForColumns = 0;
        int digitForRows = 0;

        if(maxColumn > 10) {

            int currentMax = 3;
            int index = 0;
            int previousIndex;
            while(currentMax <= maxColumn) {

                digitForRows = 0;
                std::cout << "  |";
                previousIndex = digitForColumns;
                index = previousIndex;
                while (digitForColumns <= currentMax) { 
                    std::cout << std::setw(12) << digitForColumns << "|";
                    digitForColumns++;
                }
                std::cout << std::endl;

                while(digitForRows <= maxRow) {
                    std::cout << std::setw(2) << digitForRows << "|";
                    while(index <= currentMax) {
                        std::cout << std::setw(12);
                        if(!helperForPrintEXPR(root, digitForRows, index, true)) {
                            std::cout << " ";
                        }
                        std::cout << "|";
                        index++;
                    }
                    index = previousIndex;
                    digitForRows++;
                    std::cout << std::endl;

                }
                std::cout << std::endl;
                if(maxColumn - currentMax < 5 && maxColumn - currentMax > 0) {
                    currentMax += maxColumn - currentMax;
                }

                else {
                    currentMax += 4;
                }
                
            }

        }

        else {
        
            std::cout << "  |";
            for(int i = 0; i <= maxColumn; i++) { 
                std::cout << std::setw(12) << digitForColumns << "|";
                digitForColumns++;
            }
            std::cout << std::endl;

            for(int i = 0; i <= maxRow; i++) {
                std::cout << std::setw(2) << digitForRows << "|";
                for(int j = 0; j <= maxColumn; j++) {
                    std::cout << std::setw(12);
                    if(!helperForPrintEXPR(root, i, j, true)) {
                        std::cout << " ";
                    }
                    std::cout << "|";

                }
                digitForRows++;
                std::cout << std::endl;

            }

        }

    }

    //helper function which prints all information in the table like values
    void helperForPrintAllVal(Node* root) {
    
        int digitForColumns = 0;
        int digitForRows = 0;

        if(maxColumn > 10) {

            int currentMax = 3;
            int index = 0;
            int previousIndex;
            while(currentMax <= maxColumn) {

                digitForRows = 0;
                std::cout << "  |";
                previousIndex = digitForColumns;
                index = previousIndex;
                while (digitForColumns <= currentMax) { 
                    std::cout << std::setw(12) << digitForColumns << "|";
                    digitForColumns++;
                }
                std::cout << std::endl;

                while(digitForRows <= maxRow) {
                    std::cout << std::setw(2) << digitForRows << "|";
                    while(index <= currentMax) {
                        std::cout << std::setw(12);
                        if(!helperForPrintVal(root, digitForRows, index, true)) {
                            std::cout << " ";
                        }
                        std::cout << "|";
                        index++;
                    }
                    index = previousIndex;
                    digitForRows++;
                    std::cout << std::endl;

                }
                std::cout << std::endl;
                if(maxColumn - currentMax < 5 && maxColumn - currentMax > 0) {
                    currentMax += maxColumn - currentMax;
                }

                else {
                    currentMax += 4;
                }
                
            }

        }

        else {

            std::cout << "  |";
            for(int i = 0; i <= maxColumn; i++) { 
                std::cout << std::setw(12) << digitForColumns << "|";
                digitForColumns++;
            }
            std::cout << std::endl;

            for(int i = 0; i <= maxRow; i++) {
                std::cout << std::setw(2) << digitForRows << "|";
                for(int j = 0; j <= maxColumn; j++) {
                    std::cout << std::setw(12);
                    if(!helperForPrintVal(root, i, j, true)) {
                        std::cout << " ";
                    }
                    std::cout << "|";

                }
                digitForRows++;
                std::cout << std::endl;

            }

        }
        
    }

    //function for the command PRINT EXPR <address>
    void printEXPR(int row, int column) {
        helperForPrintEXPR(root, row, column, false);
    }

    //function for the command PRINT VAL <address>
    void printVal(int row, int column) {
        helperForPrintVal(root, row, column, false);
    }

    //function for the command PRINT EXPR ALL
    void printAllEXPR() {
        helperForPrintAllEXPR(root);
    }

    //function for the command PRINT VAL ALL
    void printAllVal() {
        helperForPrintAllVal(root);
    }

    //helper function which returns the count of non-empty cells in that diapason
    void helperForCount(Node* root, int row1, int column1, int row2, int column2, int& count) {

        if(row1 > maxRow || (row1 == maxRow && column1 > maxColumn) || row1 < 0 || column1 < 0) {
            throw std::out_of_range("This address does not exist: (" + std::to_string(row1) + ", " + std::to_string(column1) + ")");
        }

        if(row2 > maxRow || (row2 == maxRow && column2 > maxColumn) || row2 < 0 || column2 < 0) {
            throw std::out_of_range("This address does not exist: (" + std::to_string(row2) + ", " + std::to_string(column2) + ")");
        }

        if (root == nullptr)
            return;
       
        helperForCount(root->left, row1, column1, row2, column2, count);   
        if(root->key.getRow() >= row1 && root->key.getRow() <= row2 && root->key.getColumn() >= min(column1, column2) && 
           root->key.getColumn() <= max(column1, column2)) {

            count++;
        }
        helperForCount(root->right, row1, column1, row2, column2, count);

    }

    //function for the command COUNT
    void count(int row1, int column1, int row2, int column2) {

        int count = 0;
        helperForCount(root, row1, column1, row2, column2, count);
        std::cout << count << std::endl;

    }

    //function which increments int number
    void incrementNumber(Node*& root) {
        int number = stoi(root->key.getValue().substr(1, root->key.getValue().size() - 2));
        number++;
        root->key.setValue("\"" + std::to_string(number) + "\"");
    }

    //function which decrements int number
    void decrementNumber(Node*& root) {
        int number = stoi(root->key.getValue().substr(1, root->key.getValue().size() - 2));
        number--;
        root->key.setValue("\"" + std::to_string(number) + "\"");
    }

    //helper function which increments number or formula
    void helperForIncrement(Node* root, int row, int column) {

        if(row > maxRow || (row == maxRow && column > maxColumn) || row < 0 || column < 0) {
            throw std::out_of_range("This address does not exist");
        }

        if(root == nullptr) {
            return;
        }

        if(root->key.getRow() == row && root->key.getColumn() == column) {

            if(root->key.getValue()[0] == '=') {
                std::string result = calculator(root->key.getValue().substr(2), row, column);

                if(validate.ifFormulaIsInt(result.substr(2))) {
                    root->key.setValue(root->key.getValue() + " + 1");
                }

            }

            else if(validate.checkIfIsInt(root->key.getValue())) {
                incrementNumber(root);
            }
            return;
        }

        else if((root->key.getRow() > row) || (root->key.getRow() == row && root->key.getColumn() > column)) {
            helperForIncrement(root->left, row, column);
        }

        else if((root->key.getRow() < row) || (root->key.getRow() == row && root->key.getColumn() < column)) {
            helperForIncrement(root->right, row, column);
        }

    }

    //helper function which decrements number or formula
    void helperForDecrement(Node* root, int row, int column) {

        if(row > maxRow || (row == maxRow && column > maxColumn) || row < 0 || column < 0) {
            throw std::out_of_range("This address does not exist");
        }

        if(root == nullptr) {
            return;
        }

        if(root->key.getRow() == row && root->key.getColumn() == column) {

            if(root->key.getValue()[0] == '=') {
                std::string result = calculator(root->key.getValue().substr(2), row, column);

                if(validate.ifFormulaIsInt(result.substr(2))) {
                    root->key.setValue(root->key.getValue() + " - 1");
                }

            }

            else if(validate.checkIfIsInt(root->key.getValue())) {
                decrementNumber(root);
            }
            return;
        }

        else if((root->key.getRow() > row) || (root->key.getRow() == row && root->key.getColumn() > column)) {
            helperForDecrement(root->left, row, column);
        }

        else if((root->key.getRow() < row) || (root->key.getRow() == row && root->key.getColumn() < column)) {
            helperForDecrement(root->right, row, column);
        }

    }

    //function for the command ++
    void increment(int row, int column) {
        helperForIncrement(root, row, column);
    }

    //function for the command --
    void decrement(int row, int column) {
        helperForDecrement(root, row, column);
    }

    //helper function which returns a queue with the correct order of the cells
    void helperForSave(Node* root, std::queue<Cell>& correctOrder) {

        if (root == nullptr)
            return;
       
        helperForSave(root->left, correctOrder);   
        correctOrder.push(root->key);
        helperForSave(root->right, correctOrder);

    }

    //function for the command SAVE
    void saveInfo(const std::string &fileName) {

        std::string type = fileName.substr(fileName.size() - 4, 4);

        if(type.compare(".csv") != 0) { 
            throw std::invalid_argument("Wrong type of file. The type must be .csv!");
        }

        std::ofstream out(fileName);

        std::queue<Cell> correctOrder;
        helperForSave(root, correctOrder);
        int row = 0;
        int column = 0;

        while(row <= maxRow) {

            if(row == correctOrder.front().getRow() && column == correctOrder.front().getColumn()) {
                out << correctOrder.front().getValue();
                correctOrder.pop();
            }

            column++;

            while(column <= maxColumn) {

                if(row == correctOrder.front().getRow() && column == correctOrder.front().getColumn()) {
                    out << ";" << correctOrder.front().getValue();
                    correctOrder.pop();
                }

                else {
                    out << ";";
                }

                column++;

            }

            out << std::endl;
            row++;
            column = 0;

        }

        isSaved = true;
        out.close();
    }

    //helper function which returns the value from a cell
    void helperForGetValue(Node* root, int row, int column, std::string &value) const {
        if (root == nullptr)
            return;
       
        helperForGetValue(root->left, row, column, value);   
        if(root->key.getRow() == row && root->key.getColumn() == column) {
            value = root->key.getValue();
            return;
        }
        helperForGetValue(root->right,row, column, value);
    }

    //getter for value
    std::string getValue(int row, int column) const {
        std::string value = "";
        helperForGetValue(root, row, column, value);
       
        return value.empty() ? "ERROR" : value;
    }

    //helper function which sum all values which are numbers in that diapason
    void helperForSum(Node* root, int row1, int column1, int row2, int column2, double& sum) {

        if(row1 > maxRow || (row1 == maxRow && column1 > maxColumn) || row1 < 0 || column1 < 0) {
            throw std::out_of_range("This address does not exist: (" + std::to_string(row1) + ", " + std::to_string(column1) + ")");
        }

        if(row2 > maxRow || (row2 == maxRow && column2 > maxColumn) || row2 < 0 || column2 < 0) {
            throw std::out_of_range("This address does not exist: (" + std::to_string(row2) + ", " + std::to_string(column2) + ")");
        }

        if (root == nullptr)
            return;
       
        helperForSum(root->left, row1, column1, row2, column2, sum);   
        if(root->key.getRow() >= row1 && root->key.getRow() <= row2 && root->key.getColumn() >= min(column1, column2) && 
           root->key.getColumn() <= max(column1, column2)) {
            
            if(root->key.getValue()[0] == '=') {
                std::string result = calculator(root->key.getValue().substr(2), root->key.getRow(), root->key.getColumn());
                double value = std::stod(result);
                sum += value;
            }

            else if(validate.functionForNormalText(root->key.getValue()) && validate.isDigit(root->key.getValue().substr(1, root->key.getValue().size() - 2))) {

                double value = std::stod(root->key.getValue().substr(1, root->key.getValue().size() - 2));
                sum += value;

            }

        }
        helperForSum(root->right, row1, column1, row2, column2, sum);

    }

    //function for the command SUM
    void sum(int row1, int column1, int row2, int column2) {
        double sum = 0;
        helperForSum(root, row1, column1, row2, column2, sum);
        std::cout << sum << std::endl;
    }

    //function for the command LOAD
    void loadFromFile(const std::string &fileName) {

        std::string type = fileName.substr(fileName.size() - 4, 4);

        if(type.compare(".csv") != 0) { 
            throw std::invalid_argument("Wrong type of file. The type must be .csv!");
        }
        
        std::ifstream in(fileName);

        if(!in.is_open()) {
            throw std::invalid_argument("The file does not exist or cannot be loaded!");
        }

        int row = 0;
        int column = 0;

        std::string line = "";
        std::string cell = "";
        std::string delimeter = ";";
        std::string firstLine = "";
        int semiColons = 0;

       
        while(std::getline(in, line)) {

            if(row == 0) {
                semiColons = validate.checkIfthelineIsCorrect(line);
            }

            int curentSemicolons = validate.checkIfthelineIsCorrect(line);

            if(curentSemicolons != semiColons) {
                throw std::invalid_argument("The file is not correct");
            }

            int start = 0;
            int end = line.find(delimeter);

            while (end != -1) {

                cell = line.substr(start, end - start);
                if(!cell.empty()) {
                    
                    if(!(validate.functionForNormalText(cell) || validate.functionForFormula(cell) || 
                         validate.functionForAbsoluteAddressing(cell) || validate.functionForRelativeAddressing(cell))) {
                        throw std::invalid_argument("Unknown data type!");
                    }

                    Cell key(cell, row, column);
                    root = insertNode(root, key);
                }
    
                else{

                    if((row > maxRow) || (row == maxRow && column > maxColumn)) {
                        maxRow = row;
                        maxColumn = column;
                    }

                }
                column++;
                start = end + delimeter.size();
                end = line.find(delimeter, start);

            }
            cell = line.substr(start, end - start);
            if(!cell.empty()) {

                if(!(validate.functionForNormalText(cell) || validate.functionForFormula(cell) || 
                     validate.functionForAbsoluteAddressing(cell) || validate.functionForRelativeAddressing(cell))) {
                    throw std::invalid_argument("Unknown data type!");
                }

                Cell key(cell, row, column);
                root = insertNode(root, key);
            }

            else{

                if((row > maxRow) || (row == maxRow && column > maxColumn)) {
                    maxRow = row;
                    maxColumn = column;
                }

            }

            row++;
            column = 0;

        }

        in.close();
        
    }

    //helper function which calculates formulas
    std::string calculate(std::vector<std::string> &partsOftheFormula, int row, int column) {
        
        bool isInTrueStatement = false;
        std::stack <double> values; 
        std::stack <std::string> operators;
        
        for(int i = 0; i < partsOftheFormula.size(); i++) {
            
            if(partsOftheFormula[i] == "{" || partsOftheFormula[i] == "}") {
                continue;
            }

            else if(validate.functionForAbsoluteAddressing(partsOftheFormula[i])) {

                int r = std::stoi(partsOftheFormula[i].substr(1, partsOftheFormula[i].find('C') - 1));
                int c = std::stoi(partsOftheFormula[i].substr(partsOftheFormula[i].find('C') + 1));
                std::string value = getValue(r, c);
                
                if(r == row && c == column) {
                    std::string error = "\"ERROR\"";
                    return error;
                }

                if(value.compare("ERROR") == 0) {
                    std::string error = "\"ERROR\"";
                    return error;
                }

                if(validate.functionForNormalText(value) && validate.isDigit(value.substr(1, value.size() - 2))) {
                    values.push(std::stod(value.substr(1, value.size() - 2)));
                }

                else if(validate.functionForAbsoluteAddressing(value) || validate.functionForRelativeAddressing(value)) {

                    int currentRow = r, currentColumn = c;
                    int currentCounter = 1;
                    while((validate.functionForAbsoluteAddressing(value) || validate.functionForRelativeAddressing(value)) && 
                           currentCounter < countOfNonEmptyCell - 1) {

                        
                        if(validate.functionForAbsoluteAddressing(value)) {

                            int newRow = std::stoi(value.substr(1, value.find('C') - 1));
                            int newColumn = std::stoi(value.substr(value.find('C') + 1));

                            value = getValue(newRow, newColumn);

                        }

                        else if(validate.functionForRelativeAddressing(value)) {

                            int newRow = std::stoi(value.substr(2, value.find('C') - 2));
                            int newColumn = std::stoi(value.substr(value.find('C') + 2, value.size() - 2));
                            
                            value = getValue(currentRow + newRow, currentColumn + newColumn);
                            currentRow += newRow;
                            currentColumn += newColumn;
                        }
                        currentCounter++;

                    }

                    if(currentCounter == countOfNonEmptyCell - 1) {
                        throw std::invalid_argument("In the table exist infinity loop from addresses!");
                    }

                    else {

                        if(value.compare("ERROR") == 0) {
                            std::string error = "\"ERROR\"";
                            return error;
                        }

                        if(validate.functionForNormalText(value) && validate.isDigit(value.substr(1, value.size() - 2))) {
                            values.push(std::stod(value.substr(1, value.size() - 2)));
                        }

                        else if(value[0] == '=' && validate.functionForFormula(value)) {
                            std::string result = calculator(value.substr(2), r, c);
                            values.push(std::stod(result));
                        }

                        else {
                            values.push(0);
                        }

                    }

                }

                else if(value[0] == '=' && validate.functionForFormula(value)) {
                    std::string result = calculator(value.substr(2), r, c);
                    values.push(std::stod(result));
                }

                else {
                    values.push(0);
                }

            } 

            else if(validate.functionForRelativeAddressing(partsOftheFormula[i])) {

                int r = std::stoi(partsOftheFormula[i].substr(2, partsOftheFormula[i].find('C') - 2));
                int c = std::stoi(partsOftheFormula[i].substr(partsOftheFormula[i].find('C') + 2, partsOftheFormula[i].size() - 2));

                std::string value = getValue(r + row, c + column);

                if(r == 0 && c == 0) {
                    std::string error = "\"ERROR\"";
                    return error;
                }

                if(value.compare("ERROR") == 0) {
                    std::string error = "\"ERROR\"";
                    return error;
                }

                if(validate.functionForNormalText(value) && validate.isDigit(value.substr(1, value.size() - 2))) {
                    values.push(std::stod(value.substr(1, value.size() - 2)));
                }

                else if(validate.functionForAbsoluteAddressing(value) || validate.functionForRelativeAddressing(value)) {

                    int currentCounter = 1;
                    int currentRow = r + row, currentColumn = c + column;
                    while((validate.functionForAbsoluteAddressing(value) || validate.functionForRelativeAddressing(value)) && 
                           currentCounter < countOfNonEmptyCell - 1) {

                        if(validate.functionForAbsoluteAddressing(value)) {

                            int r = std::stoi(value.substr(1, value.find('C') - 1));
                            int c = std::stoi(value.substr(value.find('C') + 1));

                            value = getValue(r, c);

                        }

                        else if(validate.functionForRelativeAddressing(value)) {

                            int r = std::stoi(value.substr(2, value.find('C') - 2));
                            int c = std::stoi(value.substr(value.find('C') + 2, value.size() - 2));

                            value = getValue(r + currentRow, c + currentColumn);
                            currentRow += r;
                            currentColumn += c;
                        }
                        currentCounter++;

                    }

                    if(currentCounter == countOfNonEmptyCell - 1) {
                        throw std::invalid_argument("In the table exist infinity loop from addresses!");
                    }

                    else {
        
                        if(value.compare("ERROR") == 0) {
                            std::string error = "\"ERROR\"";
                            return error;
                        }

                        if(validate.functionForNormalText(value) && validate.isDigit(value.substr(1, value.size() - 2))) {
                            values.push(std::stod(value.substr(1, value.size() - 2)));
                        }

                        else if(value[0] == '=' && validate.functionForFormula(value)) {
                            std::string result = calculator(value.substr(2), r, c);
                            values.push(std::stod(result));
                        }

                        else {
                            values.push(0);
                        }

                    }

                }

                else if(value[0] == '=' && validate.functionForFormula(value)) {
                    std::string result = calculator(value.substr(2), r, c);
                    values.push(std::stod(result));
                }

                else {
                    values.push(0);
                }

            }  
                    
            else if(validate.isDigit(partsOftheFormula[i])) {
                values.push(std::stod(partsOftheFormula[i]));

            }

            else if(ifIsOperator(partsOftheFormula[i])) {

                if(is_open_parenthesis(partsOftheFormula[i]) || partsOftheFormula[i] == "not") {
                    operators.push(partsOftheFormula[i]);
                }

                else if(partsOftheFormula[i] == "if") {
                    isInTrueStatement = false;
                    operators.push(partsOftheFormula[i]);
                }

                else if(partsOftheFormula[i] == ":" && isInTrueStatement) {
                    i++;
                    while(partsOftheFormula[i] != "}") {
                        i++;
                    }
                }

                else if(partsOftheFormula[i] == ":") {

                    while(!operators.empty() && operators.top() != "if") {

                        if(evaluate(values, operators) == "\"ERROR\"") {
                            return std::string("\"ERROR\"");
                        }

                    }
            
                    if(!operators.empty()) {
                        operators.pop();
                    }

                    if(values.top() == 0) {
                        i++;
                        while(partsOftheFormula[i] != ":") {
                            i++;
                        }
                        
                        values.pop();
                    }

                    else {
                        isInTrueStatement = true;
                        values.pop();
                    }

                }

                else if(is_close_parenthesis(partsOftheFormula[i])) {

                    while(!operators.empty() && operators.top() != "(") {

                        if(evaluate(values, operators) == "\"ERROR\"") {
                            return std::string("\"ERROR\"");
                        }

                    }
            
                    if(!operators.empty()) {
                        operators.pop();
                    }

                }

                else {

                    while(!operators.empty() && precedence(operators.top()) >= precedence(partsOftheFormula[i])){

                        if(evaluate(values, operators) == "\"ERROR\"") {
                            return std::string("\"ERROR\"");
                        }

                    }
    
                    operators.push(partsOftheFormula[i]);

                }

            }

            else {
                throw std::invalid_argument("Unknown data type!");
            }

        }
        
        while(!operators.empty()) {

            if(evaluate(values, operators) == "\"ERROR\"") {
                    return std::string("\"ERROR\"");
            }

        }
                
        return std::to_string(values.top());

    }

    //function which define the precendence between the operators
    int precedence(const std::string &op) {

        if(op == "or")
            return 1;
        if(op == "and")
            return 2;
        if(op == "==" || op == "!=")
            return 3;
        if(op == "<" || op == ">")
            return 4;
        if(op == "+" || op == "-")
            return 5;
        if(op == "*" || op == "/") 
            return 6;
        if(op == "not")
            return 7;

        return 0;

    }
    
    //function which applies operator on two numbers (one number with the operator 'not')
    double applyOperator(const std::string &op, double a, double b) {

        switch(op[0]) {

            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': return a / b;
            case 'a': return a && b;
            case 'n': return !a;
            case 'o': return a || b;
            case '<': return a < b;
            case '>': return a > b;
            case '=': return a == b;
            case '!': return a != b;
        }
            
    }

    //function which returns whether the last operation is right or not
    std::string evaluate(std::stack<double> &value, std::stack<std::string> &op) {

        std::string operators = op.top();
        op.pop();

        double val2;
        double val1;

        if(operators == "not") {

            val1 = value.top();
            value.pop();
            
            double result = applyOperator(operators, val1, 0.0);
            
            value.push(result);

            return std::string("Good");
        }

        val2 = value.top();
        value.pop();
        
        val1 = value.top();
        value.pop();

        if(val2 == 0 && operators == "/") {
            return std::string("\"ERROR\"");
        }

        double result = applyOperator(operators, val1, val2);

        value.push(result);

        return std::string("Good");

    }

    //helper function which checks if the value is operator
    bool ifIsOperator(const std::string &partOfFormula) {

       return partOfFormula == "not" || partOfFormula == "*" || partOfFormula == "/" ||
              partOfFormula == "+" || partOfFormula == "-" || partOfFormula == "and" ||
              partOfFormula == "or" || partOfFormula == "<" || partOfFormula == ">" ||
              partOfFormula == "!=" || partOfFormula == "==" || partOfFormula == "(" || 
              partOfFormula == ")"  || partOfFormula == "if" || partOfFormula == ":";

    }

    bool is_open_parenthesis(const std::string &value) {
        return value == "(";
    }

    bool is_close_parenthesis(const std::string &value) {
        return value == ")";
    }

    //function which returns the last result
    std::string calculator (const std::string &formula, int row, int column) {

        std::vector<std::string> partsOftheFormula;
        std::stringstream s(formula);
        std::string parts;

        while( std::getline(s, parts, ' ')) {  
            partsOftheFormula.push_back(parts);
        }

        std::string result = calculate(partsOftheFormula, row, column);
    
        return result;
    }

    //function for the command HELP(more info about the commands)
    void help() {
        std::cout << "Commands: " << std::endl;
        std::cout << "SET <address> <value> - creates new cell or changes old value - " 
                  << "the address must be in absolute form, also all parts have to be divided with only one"
                  << " whitespace, the same thing is when you want to input a formula"
                  << " (For example: correct formula = 3 + 4, incorrect formula =3+4). Also if you want to write an"
                  << " if statement it have a special construction : if { statement : true-statement : false statment } !" << std::endl;
        std::cout << "PRINT VAL <address> - prints the value of the cell with that address!"
                  << " (the address must be in absolute form)" << std::endl;
        std::cout << "PRINT EXPR <address> - prints the formula of the cell with that address!"
                  << " (the address must be in absolute form)" << std::endl;
        std::cout << "PRINT VAL ALL - prints the whole table with the values in the cells!" << std::endl;
        std::cout << "PRINT EXPR ALL - prints the whole table with the formulas in the cells!" << std::endl;
        std::cout << "SAVE <file_name> - saves the information from table in file with that name!" << std::endl;
        std::cout << "LOAD <file_name> - loads an information from .csv file and puts it in table!" << std::endl;
        std::cout << "++ <address> - increment the value of the cell if the value can be converted to whole number!"
                  << " (the address must be in absolute form)" << std::endl;
        std::cout << "-- <address> - decrement the value of the cell if the value can be converted to whole number!"
                  << " (the address must be in absolute form)" << std::endl;
        std::cout << "SUM <address1> <address2> - sums all the values in the table between these two addresses!"
                  << " (the addresses must be in absolute form)" << std::endl;
        std::cout << "COUNT <address1> <address2> - counts all non empty cells between these two addresses!"
                  << " (the addresses must be in absolute form)" << std::endl;
        std::cout << "EXIT - terminate the program!" << std::endl;
    }

public:

    ElectronicTable(const ElectronicTable& r) : root(copy(r.root)), maxRow(r.maxRow), maxColumn(r.maxColumn), 
    validate(r.validate), countOfNonEmptyCell(r.countOfNonEmptyCell), isSaved(r.isSaved) {};

    ElectronicTable() : root(nullptr), maxRow(0), maxColumn(0), validate(Validator()), countOfNonEmptyCell(0), isSaved(true) {}

    ~ElectronicTable() {
        clear();
    }

    //function which executes the commands from the user
    bool executeCommand(const std::string &command) {

        std::string firstPart = command.substr(0, command.find(" "));
        std::string arguments = command.substr(command.find(" ") + 1);

        if(firstPart.compare("HELP") == 0) {
            help();
        }

        else if(firstPart.compare("SET") == 0) {

            std::string address = arguments.substr(0, arguments.find(" "));
            if(!(validate.functionForAbsoluteAddressing(address))) {
                throw std::invalid_argument("The address is not in absolute form!");
            }

            std::string value = arguments.substr(arguments.find(" ") + 1);

            if(value == address) {
                throw std::invalid_argument("The value is incorrect or empty!");
            }
        
            if(!(validate.functionForNormalText(value) || validate.functionForAbsoluteAddressing(value) || 
                 validate.functionForRelativeAddressing(value) || (value[0] == '=' && validate.functionForFormula(value)))) {

                throw std::invalid_argument("The value is incorrect!");
            }
    
            int r = std::stoi(address.substr(1, address.find('C') - 1));
            int c = std::stoi(address.substr(address.find('C') + 1));

            Cell cell(value, r, c);
            insert(cell);
            isSaved = false;

        }

        else if(firstPart.compare("PRINT") == 0) {

            if(countOfNonEmptyCell == 0) {
                throw std::invalid_argument("The table is empty!");
            }

            std::string typeOfPrint = arguments.substr(0, arguments.find(" "));

            if(typeOfPrint.compare("VAL") == 0) {

                std::string whatToPrint = arguments.substr(arguments.find(" ") + 1);
                if(whatToPrint.compare("ALL") == 0) {
                    printAllVal();
                }

                else {

                    if(!validate.functionForAbsoluteAddressing(whatToPrint)) {
                        throw std::invalid_argument("The address is incorrect!");
                    }

                    int r = std::stoi(whatToPrint.substr(1, whatToPrint.find('C') - 1));
                    int c = std::stoi(whatToPrint.substr(whatToPrint.find('C') + 1));

                    printVal(r, c);
                    std::cout << std::endl;

                }

            }

            else if(typeOfPrint.compare("EXPR") == 0) {
                std::string whatToPrint = arguments.substr(arguments.find(" ") + 1);
                if(whatToPrint.compare("ALL") == 0) {
                    printAllEXPR();
                }

                else {

                    if(!validate.functionForAbsoluteAddressing(whatToPrint)) {
                        throw std::invalid_argument("The address is incorrect!");
                    }

                    int r = std::stoi(whatToPrint.substr(1, whatToPrint.find('C') - 1));
                    int c = std::stoi(whatToPrint.substr(whatToPrint.find('C') + 1));

                    printEXPR(r, c);
                    std::cout << std::endl;

                }
            }

            else {
                throw std::invalid_argument("The command for print is incorect! Use command HELP for more info!");
            }

        }

        else if(firstPart.compare("SAVE") == 0) {
            saveInfo(arguments);
        }

        else if(firstPart.compare("LOAD") == 0) {
            if(!isSaved) {

                std::string answer;
                std::cout << "The table is not saved! Do you want to save the informtion? (yes/no)" << std::endl;
                std::cin >> answer;

                if(answer.compare("yes") == 0) {
                    std::string fileName;
                    std::cout << "Please insert a file name :" << std::endl;
                    std::cin >> fileName;
                    saveInfo(fileName);
                    std::cout << "The information is saved!" << std::endl;
                }

            }
            clear();
            loadFromFile(arguments);
        }

        else if(firstPart.compare("++") == 0) {
            
            if(!validate.functionForAbsoluteAddressing(arguments)) {
                throw std::invalid_argument("The address is incorrect!");
            }

            int r = std::stoi(arguments.substr(1, arguments.find('C') - 1));
            int c = std::stoi(arguments.substr(arguments.find('C') + 1));
            
            increment(r, c);
        }

        else if(firstPart.compare("--") == 0) {
            
            if(!validate.functionForAbsoluteAddressing(arguments)) {
                throw std::invalid_argument("The address is incorrect!");
            }

            int r = std::stoi(arguments.substr(1, arguments.find('C') - 1));
            int c = std::stoi(arguments.substr(arguments.find('C') + 1));
            
            decrement(r, c);
        }

        else if(firstPart.compare("SUM") == 0) {

            std::string address1 = arguments.substr(0, arguments.find(" "));
            std::string address2 = arguments.substr(arguments.find(" ") + 1);

            if(!(validate.functionForAbsoluteAddressing(address1) && validate.functionForAbsoluteAddressing(address2)) || 
                address1 == address2) {
                throw std::invalid_argument("The addresses is incorrect!");
            }

            int r1 = std::stoi(address1.substr(1, address1.find('C') - 1));
            int c1 = std::stoi(address1.substr(address1.find('C') + 1));

            int r2 = std::stoi(address2.substr(1, address2.find('C') - 1));
            int c2 = std::stoi(address2.substr(address2.find('C') + 1));

            sum(r1, c1, r2, c2);

        }

        else if(firstPart.compare("COUNT") == 0) {

            std::string address1 = arguments.substr(0, arguments.find(" "));
            std::string address2 = arguments.substr(arguments.find(" ") + 1);

            if(!(validate.functionForAbsoluteAddressing(address1) && validate.functionForAbsoluteAddressing(address2)) || 
                address1 == address2) {
                throw std::invalid_argument("The addresses is incorrect!");
            }

            int r1 = std::stoi(address1.substr(1, address1.find('C') - 1));
            int c1 = std::stoi(address1.substr(address1.find('C') + 1));

            int r2 = std::stoi(address2.substr(1, address2.find('C') - 1));
            int c2 = std::stoi(address2.substr(address2.find('C') + 1));

            count(r1, c1, r2, c2);

        }

        else if(firstPart.compare("EXIT") == 0) {
            if(!isSaved) {

                std::string answer;
                std::cout << "The table is not saved! Do you want to save the informtion? (yes/no)" << std::endl;
                std::cin >> answer;

                if(answer.compare("yes") == 0) {
                    std::string fileName;
                    std::cout << "Please insert a file name :" << std::endl;
                    std::cin >> fileName;
                    saveInfo(fileName);
                    std::cout << "The information is saved!" << std::endl;
                }

            }
            std::cout << "Goodbye!" << std::endl;
            return false;
        }

        else {
            throw std::invalid_argument("The command is incorrect! Use the command HELP for more information!");
        }

        return true;
    }

};
