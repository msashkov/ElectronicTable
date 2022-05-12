#pragma once

#include<string>

//class which stores information for one cell in the table
class Cell {
private:
    std::string value;
    int row;
    int column;

public:
    Cell(const std::string& value, int row, int column) : value(value), row(row), column(column) {}

    Cell(const Cell& other) {
        this->value = other.value;
        this->row = other.row;
        this->column = other.column;
    }

    Cell& operator =(const Cell& other) {
        
        if(this != &other) {
            this->value = other.value;
            this->row = other.row;
            this->column = other.column;
        }

        return *this;
    }

    std::string getValue() const {
        return value;
    }

    int getRow() const {
        return row;
    }

    int getColumn() const {
        return column;
    }

    void setValue(const std::string& newValue) {
        this->value = newValue;
    }

    bool operator >(const Cell& other) const {
        return (this->row > other.row) || (this->row == other.row && this->column > other.column);
    }

    bool operator <(const Cell& other) const {
        return (this->row < other.row) || (this->row == other.row && this->column < other.column);
    }

};