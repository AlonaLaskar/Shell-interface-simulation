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
void newProcess(char**,int);
void deleteTheWords(char**,int);


int main() {
    char str[LENGTH];
    int CountTheActions=0;
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

        char ** a= SplitsTheActions(str);

        if ((strcmp(a[0],"cd"))== 0) {
            printf("command not supported(yet)\n");
            deleteTheWords(a,words);
            continue;
        }
        if ((strcmp(a[0],"done"))== 0 && a[1]==NULL){
            printf("num of commends : %d\n", CountTheActions);
            printf("total length of all the commends : %d\n",length);
            double average=(double)length/CountTheActions;
            printf("Average length of all the commends : %lf\n",average);
            printf("see you Next time !\n");
            deleteTheWords(a,words);
            break;
        }
        newProcess(a,words);
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
    int author = 0, i;
    int len =(int)strlen(str);
    for (i = 0; i < len; i++) {
        if (i == 0 && str[i] != ' ')
            author++;
        if (str[i] == ' ' && str[i + 1] != ' ' && str[i + 1] != '\n')
            author++;
    }
    return author;
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
    for ( j = 0; j < len; j++) {//Checks how many letters there are in each word
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
                i++;
                counter=0;
            }
        }
    }
    Actions[numWord-1]=NULL;
    return Actions;
}
void newProcess(char** Actions,int count) {//Creates a new process
    pid_t id;
    id = fork();
    if (id < 0) {
        printf("Fork failed");
        deleteTheWords(Actions, count);
        exit(1);
    }
    if (id == 0) {
        if (execvp(Actions[0], Actions) < 0) {
            for(int i=0;i<count+1;i++){
                printf("Actions in %d is %s\n",i,Actions[i]);
            }
            printf("command not found\n");
            deleteTheWords(Actions, count);
            exit(1);
        }
    }
    else {
        wait(NULL);
       // deleteTheWords(Actions,count);
    }
}
void deleteTheWords(char** a,int t){//free the dynamic assignmentls
    int m;
    for (m = 0; m<t;m++) {
        free(a[m]);
    }
    free(a);
}
