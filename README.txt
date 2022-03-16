Support of the mini  shell in the pipes.
==Description==
Functions:
The program simulates the commands of the operating system.
Through this program we can run a program and not with the help of the terminal.
This is a follow-up exercise for ex2 The difference is that now the mini-shell knows how to communicate between the processes that take place together.
Now our program knows how to use pipes.
 ==Program Files==
void printPrompt()-This function receives from the system the username and where the file is located on the computer.
void getTheString(char* str)- This function receives from the user the command line or word (taken from EX1).
 int countTheWord(char* str)- This function gets the writer the number of words - commands (taken from EX1).
char** SplitsTheActions(char* str)- This function divides the words and sub-commands and allocates space for them in memory.
void newProcess(char* str,int)- Creates a new process and sends the command to the operating system.
void deleteTheWords(char** a, int t)- This function frees the memory of the dynamic assignment.
== Program Files that joined during this exercise==
int cheakThePipe(char* )- This function runs through the entire string and counts the number of  pipes that exist.
And return the number of pipes.
void creatOnePipe(char* str)- This function crosses the string up to and after the pipe. and makes a linking process between the processes running together..
void CreatTwoPipe(char* str)- This function crosses the string up to and after the pipe. and makes a linking process between the processes running together.
==How to compile?==
compile: gcc ex3.c -o ex3
run: ./ex3
input:
The user enters the command line to select, and this time the user can put in his string with 0 pipes or '|' Or two.
output:
The operation details entered by the user will be displayed.
The program connects the various commands that are executed in a mini-shell even those made through a tube and prints the desired output on the screen.
If you click on the word "done", the program will display statistics of the number of actions entered and also the number of uses of the pipes.

