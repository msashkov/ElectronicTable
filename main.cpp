#include <iostream>
#include <string>
#include <stdexcept>
#include "electronicTable.h"

int main() {

  ElectronicTable table;

  std::string command;
	bool exit = true;

  std::cout << "Use the command HELP for more info!" << std::endl;
	while (exit) {

		std::cout << "> ";
		std::getline(std::cin, command);

    try {

      exit = table.executeCommand(command);

    }catch(std::out_of_range& ex) {
      std::cout << "out_of_range: " << ex.what() << std::endl;
    }catch(std::invalid_argument& ex) {
      std::cout << "invalid_argument: " << ex.what() << std::endl;
    }
    
    std::cin.clear();

	}
  

  return 0;
}