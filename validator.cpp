#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include "validator.h"

bool Validator::functionForAbsoluteAddressing(const std::string &value) {

    if(value[0] != 'R') {
        return false;
    }

    std::string checkNumberForTheRow = value.substr(1, value.find('C') - 1);

    if(checkNumberForTheRow.size() > 1 && checkNumberForTheRow[0] == '0') {
        return false;
    }
    
    std::size_t countOfDigitsForRow = checkNumberForTheRow.size();
    for(int i = 0; i < countOfDigitsForRow; i++) {

        if(checkNumberForTheRow[i] < '0' || checkNumberForTheRow[i] > '9') {
            return false;
        }
    }

    std::string checkNumberForTheColumn = value.substr(value.find('C') + 1);

    if(checkNumberForTheColumn.size() > 1 && checkNumberForTheColumn[0] == '0') {
        return false;
    }

    std::size_t countOfDigitsForColumn = checkNumberForTheColumn.size();
    for(int i = 0; i < countOfDigitsForColumn; i++) {

        if(checkNumberForTheColumn[i] < '0' || checkNumberForTheColumn[i] > '9') {
            return false;
        }
    }
    
    return true;

}

bool Validator::functionForRelativeAddressing(const std::string &value) {

    if(!(value[0] == 'R' && value[1] == '[')) {
        return false;
    }

    std::string checkNumberForTheRow = value.substr(2, value.find(']') - 2);

    if(checkNumberForTheRow.size() == 1 && (checkNumberForTheRow[0] < '0' || checkNumberForTheRow[0] > '9')) {
        return false;
    }

    if(checkNumberForTheRow.size() > 1 && checkNumberForTheRow[0] == '-' && checkNumberForTheRow[1] == '0') {
        return false;
    }

    std::size_t countOfDigitsForRow = checkNumberForTheRow.size();
    for(int i = 0; i < countOfDigitsForRow; i++) {

        if(!(checkNumberForTheRow[0] == '-' || (checkNumberForTheRow[i] >= '0' && checkNumberForTheRow[i] <= '9'))) {
            return false;
        }
    }

    std::string checkNumberForTheColumn = value.substr(value.find('C') + 1);

    if(checkNumberForTheColumn[0] != '[') {
        return false;
    }

    std::string column = checkNumberForTheColumn.substr(1, checkNumberForTheColumn.find(']') - 1);

    if(column.size() == 1 && (column[0] < '0' || column[0] > '9')) {
        return false;
    }

    if(column.size() > 1 && column[0] == '-' && column[1] == '0') {
        return false;
    }
    
    std::size_t countOfDigitsForColumn = column.size();
    for(int i = 0; i < countOfDigitsForColumn; i++) {

        if(!(column[0] == '-' || (column[i] >= '0' && column[i] <= '9'))) {
            return false;
        }
    }

    return true;

}

bool Validator::functionForNormalText(const std::string &value) {
    if(value.front() != '\"' || value.back() != '\"') {
        return false;
    }
    return true;
}

bool Validator::checkForValidBeginning(const std::vector<std::string>& partsOfTheFormula) {

    return isDigit(partsOfTheFormula[0]) || functionForRelativeAddressing(partsOfTheFormula[0]) ||
           functionForAbsoluteAddressing(partsOfTheFormula[0]) || partsOfTheFormula[0] == "(" || 
           partsOfTheFormula[0] == "if" || partsOfTheFormula[0] == "not";

}

bool Validator::checkForIncorrectPart(const std::vector<std::string>& partsOftheFormula, int i) {
    return !functionForRelativeAddressing(partsOftheFormula[i]) || !functionForAbsoluteAddressing(partsOftheFormula[i]) || 
           !isMathOperator(partsOftheFormula[i]) || !isCompareOperator(partsOftheFormula[i]) || 
           !isBracketOrSpecialSymbol(partsOftheFormula[i]) || 
           !isDigit(partsOftheFormula[i]) || !booleanOperators(partsOftheFormula[i]);
}

bool Validator::checkForValidEnding(const std::vector<std::string>& partsOftheFormula, int i) {

    return i == partsOftheFormula.size() - 1 && (isDigit(partsOftheFormula[i]) || partsOftheFormula[i] == ")" ||
           partsOftheFormula[i] == "}" || functionForRelativeAddressing(partsOftheFormula[i]) || 
           functionForAbsoluteAddressing(partsOftheFormula[i]));

}

bool Validator::checkForValidSymbolAfterMathOp(const std::vector<std::string>& partsOftheFormula, int i) {

    return isMathOperator(partsOftheFormula[i]) && (!(isDigit(partsOftheFormula[i + 1]) || 
           functionForRelativeAddressing(partsOftheFormula[i + 1]) ||
           functionForAbsoluteAddressing(partsOftheFormula[i + 1]) || 
           partsOftheFormula[i+1] == "if" || partsOftheFormula[i+1] == "("));

}

bool Validator::checkForSymbolAfterCompareOP(const std::vector<std::string>& partsOftheFormula, int i) {

    return isCompareOperator(partsOftheFormula[i]) && (!(isDigit(partsOftheFormula[i + 1]) || 
           functionForRelativeAddressing(partsOftheFormula[i + 1]) ||
           functionForAbsoluteAddressing(partsOftheFormula[i + 1]) || 
           partsOftheFormula[i+1] == "if" || partsOftheFormula[i+1] == "("));

}

bool Validator::checkForSymbolAfterIf(const std::vector<std::string>& partsOftheFormula, int i) {
    return partsOftheFormula[i] == "if" && partsOftheFormula[i + 1] != "{";
}

bool Validator::checkForSymbolAfterOpenBrace(const std::vector<std::string>& partsOftheFormula, int i) {

    return (partsOftheFormula[i] == ":" || partsOftheFormula[i] == "(" || partsOftheFormula[i] == "{") && 
           (!(partsOftheFormula[i+1] == "(" || isDigit(partsOftheFormula[i+1]) || 
           functionForAbsoluteAddressing(partsOftheFormula[i+1]) ||
           functionForRelativeAddressing(partsOftheFormula[i+1]) || 
           partsOftheFormula[i + 1] == "if" || booleanOperators(partsOftheFormula[i + 1])));

}

bool Validator::checkForSymbolAfterBooleanOp(const std::vector<std::string>& partsOftheFormula, int i) {

    return booleanOperators(partsOftheFormula[i]) && (!(partsOftheFormula[i+1] == "(" || 
           isDigit(partsOftheFormula[i+1]) || functionForAbsoluteAddressing(partsOftheFormula[i+1]) ||
           functionForRelativeAddressing(partsOftheFormula[i+1]) || partsOftheFormula[i + 1] == "if"));

}

bool Validator::checkForSymbolAfterAddressOrDigit(const std::vector<std::string>& partsOftheFormula, int i) {

    return i != partsOftheFormula.size() - 1 && (functionForAbsoluteAddressing(partsOftheFormula[i]) || 
           functionForRelativeAddressing(partsOftheFormula[i]) ||
           isDigit(partsOftheFormula[i])) && !(isMathOperator(partsOftheFormula[i+1]) || 
           isCompareOperator(partsOftheFormula[i+1]) || booleanOperators(partsOftheFormula[i+1]) ||
           isBracketOrSpecialSymbol(partsOftheFormula[i+1]));

}

bool Validator::checkForSymbolAfterCloseBrace(const std::vector<std::string>& partsOftheFormula, int i) {

    return partsOftheFormula[i] == ")" && !(isMathOperator(partsOftheFormula[i+1]) || booleanOperators(partsOftheFormula[i+1]) ||
           partsOftheFormula[i + 1] == ")" || partsOftheFormula[i + 1] == ":" || isCompareOperator(partsOftheFormula[i+1]));

}

bool Validator::checkForSymbolAfterEndOfIF(const std::vector<std::string>& partsOftheFormula, int i) {

    return (partsOftheFormula[i] == "}" && i != partsOftheFormula.size() - 1) && !(partsOftheFormula[i + 1] == "}" ||
            partsOftheFormula[i + 1] == ":" || partsOftheFormula[i + 1] == ")" || isMathOperator(partsOftheFormula[i+1]) ||
            booleanOperators(partsOftheFormula[i+1]) || isCompareOperator(partsOftheFormula[i+1]));

}

bool Validator::checkForCorrectFormula(const std::vector<std::string>& partsOftheFormula, int i) {
    return checkForValidSymbolAfterMathOp(partsOftheFormula, i) ||
           checkForSymbolAfterCompareOP(partsOftheFormula, i) ||
           checkForSymbolAfterIf(partsOftheFormula, i) ||
           checkForSymbolAfterOpenBrace(partsOftheFormula, i) ||
           checkForSymbolAfterBooleanOp(partsOftheFormula, i) ||
           checkForSymbolAfterAddressOrDigit(partsOftheFormula, i) ||
           checkForSymbolAfterCloseBrace(partsOftheFormula, i) ||
           checkForSymbolAfterEndOfIF(partsOftheFormula, i);

}
 

bool Validator::functionForFormula(const std::string &value) {
    
    std::string formula = value.substr(2);
    std::vector<std::string> partsOftheFormula;
    std::stringstream s(formula);
    std::string parts;
    
    if(!differentBalancedBrackets(formula)) {
        return false;
    }

    if(!checkForDoubleDots(formula)) {
        return false;
    }

    while( getline(s, parts, ' ')) {  
        partsOftheFormula.push_back(parts);
    }

    if(!checkForValidBeginning(partsOftheFormula)) {
        return false;
    }

    std::size_t lengthOfFormula = partsOftheFormula.size();
    for(int i = 0; i < lengthOfFormula; i++) {

        if(!checkForIncorrectPart(partsOftheFormula, i)) {
            return false;
        }

        if(checkForValidEnding(partsOftheFormula, i)) {
            return true;
        }

        if(checkForCorrectFormula(partsOftheFormula, i)) {
            return false;
        }

    }

    return true;

}

bool Validator::isMathOperator(const std::string &value) {
    return value == "+" || value == "-" || value == "/" || value == "*";
            
}

bool Validator::isCompareOperator(const std::string &value) {
    return value == ">" || value == "<" || value == "==" || value == "!=";
}

bool Validator::booleanOperators(const std::string &value) {
    return value == "and" || value == "or" || value == "not";
}

bool Validator::isBracketOrSpecialSymbol(const std::string &value) {
    return value == "(" || value == ")" || value == "{" || value == "}" ||
            value == "if" || value == ":";
}

bool Validator::isDigit(const std::string &value) {

    if(value.size() == 1 && (value[0] < '0' || value[0] > '9')) {
        return false;
    }

    for(int i = 0; i < value.size(); i++) {

        if(!(((value[i] >= '0' && value[i] <= '9') || (value[i] == '-' && i == 0) || 
                (value[i] == '.' && i != 0 && i != value.size() - 1)) && checkForDots(value))) {

            return false;
        }

    }

    return true;

}

bool Validator::checkForDots(const std::string &value) {

    int dots = 0;

    for(int i = 0; i < value.size(); i++) {

        if(value[i] == '.') {
            dots++;
        }

    }

    return (dots == 0) || (dots == 1 && value.size() > 1);

}

bool Validator::checkForDoubleDots(const std::string &value) {

    int dots = 0;
    int ifs = 0;

    for(int i = 0; i < value.size(); i++) {

        if(value[i] == ':') {
            dots++;
        }

        if(value[i] == 'i') {
            ifs++;
        }

    }

    return 2*ifs == dots;

}

bool Validator::differentBalancedBrackets(const std::string &value) {
    int strLen = value.length();
    std::stack<char> s;
    for (int i = 0; i < strLen; ++i) {
        switch (value[i]) {
        case '(':
        case '{':
            s.push(value[i]);
            break;
        case ')':
            if (s.top() != '(')
                return false;
            s.pop();
            break;
        case '}':
            if (s.top() != '{')
                return false;
            s.pop();
            break;
        default:
            break;
        }
    }
    return s.empty();
}

bool Validator::checkIfIsInt(const std::string &value) {

    std::string newValue = "";

    if(value[0] == '\"') {
        newValue = value.substr(1, value.size() - 2);
    }

    else {
        return false;
    }

    std::size_t size = newValue.size();

    if(newValue[0] != '-' && (newValue[0] < '1' || newValue[0] > '9')) {
        return false;
    }

    if((newValue[0] == '-' && newValue[1] == 0) || newValue[0] == '0') {
        return false;
    }

    for(int i = 1; i < size; i++) {

        if(!(newValue[i] >= '0' && newValue[i] <= '9')) {
            return false;
        }
    }

    return true;
}

int Validator::checkIfthelineIsCorrect(const std::string &line) {
    int countOfSemiColons = 0;
    for(int i = 0; i < line.size(); i++) {
        if(line[i] == ';') {
            countOfSemiColons++;
        }
    }

    return countOfSemiColons;
}

bool Validator::ifFormulaIsInt(const std::string &value) {

    std::size_t length = value.length();

    for(std::size_t i = 0; i < length; i++) {

        if(value[i] != '0') {
            return false;
        }

    }

    return true;

}