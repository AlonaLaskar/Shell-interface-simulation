Shell interface simulation
==Description==
Functions:
The program simulates the commands of the operating system.
Through this program we can run a program and not with the help of the terminal.
==Program Files==
* void printPrompt()-This function receives from the system the username and where the file is located on the computer.
* void getTheString(char* str)- This function receives from the user the command line or word (taken from EX1).
* int countTheWord(char* str)- This function gets the writer the number of words - commands (taken from EX1).
* char** SplitsTheActions(char* str)- This function divides the words and sub-commands and allocates space for them in memory.
* void newProcess(char** Actions,int)- Creates a new process and sends the command to the operating system.
* void deleteTheWords(char** a, int t)- This function frees the memory of the dynamic assignment.

==How to compile?==
compile: gcc ex2a.c -o ex2a
run: ./ex2a
==How to compile?==
compile: gcc ex2b.c -o ex2b
run: ./ex2b


input:
User enters command prompt to select.
Output:
The details of the action entered by the user will be displayed.
If the word "done" is pressed, the program will display statistics of operations.
Output ex2b:
The output is the same except that a scheduler is also printed
