# Project
Project for electronic tables, working with excel files. The structure of the table is self-balancing tree (AVL). It has the commands: 
-SET <address> <value> - creates new cell or changes old value -  the address must be in absolute form, also all parts have to be divided with only one whitespace, the same thing is when you want to input a formula (For example: correct formula = 3 + 4, incorrect formula =3+4). Also if you want to write an if statement it have a special construction : if { statement : true-statement : false statment }
-PRINT VAL <address> - prints the value of the cell with that address! (the address must be in absolute form)
-PRINT EXPR <address> - prints the formula of the cell with that address! (the address must be in absolute form)
-PRINT VAL ALL - prints the whole table with the values in the cells!
-PRINT EXPR ALL - prints the whole table with the formulas in the cells!
-SAVE <file_name> - saves the information from table in file with that name!
-LOAD <file_name> - loads an information from .csv file and puts it in table!
- ++ <address> - increment the value of the cell if the value can be converted to whole number! (the address must be in absolute form)
- -- <address> - decrement the value of the cell if the value can be converted to whole number! (the address must be in absolute form)
-SUM <address1> <address2> - sums all the values in the table between these two addresses! (the addresses must be in absolute form)
-COUNT <address1> <address2> - counts all non empty cells between these two addresses! (the addresses must be in absolute form)
-EXIT - terminate the program!
