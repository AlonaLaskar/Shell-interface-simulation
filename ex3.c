
//ex3:The program simulates a mini-shell that simulates the operating system.
//In this exercise we focused on working with pipes and linking actions done together using a pipe.
//When the user enters done the program will end and a summary of the number of actions performed and the number of uses of the pipes will be issued.
//Submitted by: Alona Laskar
//ID: 316559624
//---------------------------------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#define LENGTH 510

void printPrompt();
void getTheString(char*);
int countTheWord(char*);
char** SplitsTheActions(char*);
void newProcess(char*,int);
void deleteTheWords(char**,int);
int cheakThePipe(char* );
void creatOnePipe(char* str);
void CreatTwoPipe(char* str);


int main() {
    char str[LENGTH];
    int CountTheActions=0;
    int CountThePipes=0;
    int length=0;

    while (1) {
        printPrompt();
        getTheString(str);
        length+=(int)strlen(str)-1;
        CountTheActions++;
        int words= countTheWord(str);
        if(words==0)// if the input is no words
            continue;
        if ((strcmp(str,"\n"))== 0)// if the input is enter
            continue;
        int numPipe= cheakThePipe(str);//number of pipes
        CountThePipes+= numPipe;
        char ** a= SplitsTheActions(str);
        if ((strcmp(a[0],"cd"))== 0) {
            printf("command not supported(yet)\n");
            deleteTheWords(a,words);
            continue;
        }

        if ((strcmp(a[0],"done"))== 0 && a[1]==NULL){
            printf("num of commends : %d\n", CountTheActions-1);
            printf("Number of pipes : %d\n",CountThePipes);
            printf("see you Next time !\n");
            deleteTheWords(a,words);
            break;
        }
        deleteTheWords(a,words);
        if(numPipe>2){// if the user iput mor then 2 pipes
            printf("Unsupported use of more than two pipes\n");
            continue;
        }
        if(numPipe==0)
            newProcess(str,words);
        if(numPipe==1)
            creatOnePipe(str);
        if(numPipe==2)
            CreatTwoPipe(str);
    }
}
void printPrompt() {//Gets the username and command location
    struct passwd *p;
    char *currentDir;
    uid_t uid = 0;
    if ((p = getpwuid(uid)) == NULL)
        perror("getpwuid() NULL");
    else {
        currentDir = (char *) malloc(sizeof(char) * 512);
        if (currentDir == NULL) {
            printf("Error:could not allocate memory");
            exit(1);
        }
        getcwd(currentDir, 512);
        printf("%s@%s>", p->pw_name, currentDir);
        free(currentDir);
    }
}
void getTheString(char *str) { //This function getting the input from the user
    fgets(str, LENGTH, stdin);
}
int countTheWord(char* str) {//This function count how many words they have in the input
    int counter = 0, i;
    int len = (int) strlen(str);
    for (i = 0; i < len; i++) {
        if (i == 0 && str[i] != ' ')
            counter++;
        if (str[i] == ' ' && str[i + 1] != ' ' && str[i + 1] != '\n')
            counter++;
    }
    return counter;
}

char** SplitsTheActions(char* str) {//This function dynamically assigns a two-dimensional array and copies the words to it
    int i=0,counter = 0,j,startWord;
    int len =(int)strlen(str);
    int numWord = countTheWord(str)+1;
    char **Actions = (char **) malloc(numWord * sizeof(char *));//Assign the array as large as the size of the words plus one
    if (Actions == NULL) {  //Checks if there is place in the memory for allocation
        printf("Error:could not allocate memory");
        exit(1);
    }
    for ( j = 0; j <= len; j++) {//Checks how many letters there are in each word
        if (counter == 0) {
            startWord = j;
        }
        if (str[j] != ' ' && str[j] != '\0' && str[j] != '\n') {
            counter++;
        }
        else if (str[j] == ' ' || str[j] == '\n' || str[j]== '\0') {
            if (counter > 0) {
                Actions[i] = (char *) malloc((counter + 1) * sizeof(char));//Dynamically assigns the small array

                if ( Actions[i] == NULL) {  //Checks if there is place in the memory for allocation
                    printf("Error:could not allocate memory");
                    deleteTheWords(Actions, counter + 1);
                    exit(1);
                }
                strncpy(Actions[i],str+startWord, counter);//copy the words
                Actions[i][counter]='\0';
                i++;
                counter=0;
            }
        }
    }
    Actions[numWord-1]=NULL;
    return Actions;
}
void newProcess(char* str,int count) {//Creates a new process
    char** a= SplitsTheActions(str);
    pid_t id;
    id = fork();
    if (id < 0) {// error
        printf("Fork failed");
        deleteTheWords(a, count);
        exit(1);
    }
    if (id == 0) {
        if (execvp(a[0], a) < 0) {//creat son
            printf("command not found\n");
            deleteTheWords(a,count);
            exit(EXIT_FAILURE);
        }
    }
    else {
        wait(NULL);
        deleteTheWords(a,count);
    }
}
void deleteTheWords(char** a,int t){//free all the allocation of memory
    int m;
    for (m = 0; m<t;m++) {
        free(a[m]);
    }
    free(a);
}
int cheakThePipe(char* str){//cheak how many pipe the user enter
    int i,pipeNum=0;
    int len= (int)strlen(str);
    for (i = 0; i < len; i++) {
        if(str[i]=='|')
            pipeNum++;
    }
    return pipeNum;
}
void creatOnePipe(char* str){//This function crosses the array before and after the pipe.And sends the commands to execute
    char* token;
    token = strtok(str, "|");
    int num1= countTheWord(token);
    char** left=SplitsTheActions(token);

        if ((strcmp(left[0],"done"))==0){
            printf("command not found\n");
            return;
    }


    token= strtok(NULL,"|");
    int num2= countTheWord(token);
    char** right= SplitsTheActions(token);

    if ((strcmp(right[0],"done"))==0){
        printf("command not found\n");
        return;
    }

    pid_t firstSon,secondSon;
    int pipe_onePipd[2];
    if(pipe(pipe_onePipd)==-1){
        perror("pipe faild");
        exit(EXIT_FAILURE);
    }
    firstSon=fork();
    if(firstSon<0){
        perror("fork faild");
        deleteTheWords(left,num1);
        deleteTheWords(right,num2);
        exit(EXIT_FAILURE);
    }
    if (firstSon==0){
        dup2(pipe_onePipd[1],STDOUT_FILENO);
        close(pipe_onePipd[0]);
        close(pipe_onePipd[1]);
            if (execvp(left[0],left) < 0) {
                printf("command not found\n");
            deleteTheWords(left, num1);
            deleteTheWords(right,num2);
            exit(EXIT_FAILURE);
        }
    }
    secondSon=fork();
    if(secondSon<0){
        perror("fork faild");
        deleteTheWords(left, num1);
        deleteTheWords(right,num2);
        exit(EXIT_FAILURE);
    }
    if (secondSon==0){
        dup2(pipe_onePipd[0],STDIN_FILENO);
        close(pipe_onePipd[0]);
        close(pipe_onePipd[1]);
        if (execvp(right[0], right) < 0) {
            printf("command not found\n");
            deleteTheWords(left, num1);
            deleteTheWords(right,num2);
            exit(EXIT_FAILURE);
        }
    }
    else{
        close(pipe_onePipd[0]);
        close(pipe_onePipd[1]);
        wait(NULL);
        wait(NULL);
        deleteTheWords(left,num1);
        deleteTheWords(right,num2);
    }
}
void CreatTwoPipe(char* str){//creat two pipe and 3 process.This function crosses the array before and after the pipe.And sends the commands to execute
    char* token;
    token = strtok(str,"|");
    int num1=countTheWord(token);
    char** left=SplitsTheActions(token);
    token= strtok(NULL,"|");
    int num2= countTheWord(token);
    char** middle= SplitsTheActions(token);
    token= strtok(NULL,"|");
    int num3=countTheWord(token);
    char** right= SplitsTheActions(token);
    pid_t firstSon,secondSon,thirdSon;

    if (((strcmp(right[0],"done"))==0) || ((strcmp(left[0],"done"))==0) || ((strcmp(middle[0],"done"))==0)){
        printf("command not found\n");
        return;
    }
    int pipe_firstPipe[2];
    int pipe_seconPipe[2];
    if(pipe(pipe_firstPipe)==-1 || pipe(pipe_seconPipe)==-1){
        perror("Pipe Failed");
        deleteTheWords(left,num1);
        deleteTheWords(right,num2);
        exit(EXIT_FAILURE);
    }

    firstSon = fork();
    if (firstSon < 0) {// error
        printf("Fork Failed");
        deleteTheWords(left,num1);
        deleteTheWords(middle,num2);
        deleteTheWords(right,num3);
        exit(1);
    }
    if (firstSon == 0) {
        dup2(pipe_firstPipe[1],STDOUT_FILENO);
        close(pipe_firstPipe[0]);
        close(pipe_firstPipe[1]);
        if(execvp(left[0],left)<0) {
            perror("command not found");
            deleteTheWords(left,num1);
            deleteTheWords(middle,num2);
            deleteTheWords(right,num3);
            exit(EXIT_FAILURE);
        }
    }
    secondSon=fork();
    if (secondSon < 0) {// error
        printf("Fork Failed");
        deleteTheWords(left,num1);
        deleteTheWords(middle,num2);
        deleteTheWords(right,num3);
        exit(1);
    }
    if (secondSon == 0) {
        close(pipe_firstPipe[1]);
        dup2(pipe_firstPipe[0],STDIN_FILENO);//output
        close(pipe_firstPipe[0]);
        close(pipe_seconPipe[0]);
        dup2(pipe_seconPipe[1],STDOUT_FILENO);//input
        close(pipe_seconPipe[1]);
        if (execvp(middle[0],middle)<0) {
            perror("command not found");
            deleteTheWords(left,num1);
            deleteTheWords(middle,num2);
            deleteTheWords(right,num3);
            exit(EXIT_FAILURE);
        }
    }
    thirdSon = fork();
    if (thirdSon < 0) {// error
        printf("Fork Failed");
        deleteTheWords(left,num1);
        deleteTheWords(middle,num2);
        deleteTheWords(right,num3);
        exit(1);
    }
    if (thirdSon == 0) {
        close(pipe_firstPipe[0]);
        close(pipe_firstPipe[1]);
        close(pipe_seconPipe[1]);
        dup2(pipe_seconPipe[0],STDIN_FILENO);
        close(pipe_seconPipe[0]);
        if   (execvp(right[0],right)<0){
            perror("command not found");
            deleteTheWords(left,num1);
            deleteTheWords(middle,num2);
            deleteTheWords(right,num3);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        close(pipe_firstPipe[0]);
        close(pipe_firstPipe[1]);
        close(pipe_seconPipe[0]);
        close(pipe_seconPipe[1]);
        wait(NULL);
        wait(NULL);
        wait(NULL);
        deleteTheWords(left,num1);
        deleteTheWords(middle,num2);
        deleteTheWords(right,num3);
    }
}