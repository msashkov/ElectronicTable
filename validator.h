#pragma once

#include <string>

// class with functions which check if a string is correct
class Validator {

public:

    //checks if the value is address in absolute form
    bool functionForAbsoluteAddressing(const std::string &value);

    //checks if the value is address in relative form
    bool functionForRelativeAddressing(const std::string &value);

    //checks if the value is normal text
    bool functionForNormalText(const std::string &value);

    //checks if the value is correct formula
    bool functionForFormula(const std::string &value);

    bool isMathOperator(const std::string &value);
    bool isCompareOperator(const std::string &value);
    bool booleanOperators(const std::string &value);
    bool isBracketOrSpecialSymbol(const std::string &value);

    //checks if the value is a correct number
    bool isDigit(const std::string &value);

    //checks if the value is correct double number
    bool checkForDots(const std::string &value);

    //check if the value has correct if`s structure
    bool checkForDoubleDots(const std::string &value);

    //checks if the brackets in the value are balanced
    bool differentBalancedBrackets(const std::string &value);

    //checks if the value is correct int number
    bool checkIfIsInt(const std::string &value);

    //returns the count of semicolons in every line of the file
    int checkIfthelineIsCorrect(const std::string &line);

    //cheks if the result of the formula is int number
    bool ifFormulaIsInt(const std::string &value);

};