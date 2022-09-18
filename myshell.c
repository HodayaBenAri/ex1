// Hodaya Raz 211540174
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[] )
{
    int forkPid;
    int pidArray[101];
    int i = 0;
    int len;

    char *PATH = getenv("PATH");
    char *addPath = (char *) malloc(strlen(PATH) + 1);
    strcpy(addPath, PATH);
    int index = 1;
    for (index; index<argc; ++index) {
        len = strlen(addPath) + strlen(argv[index]) + 2;
        addPath = (char *) realloc(addPath, len);
        sprintf(addPath, "%s:%s", addPath, argv[index]);
    }
    setenv("PATH",addPath,1);

    while (1)
    {
        char arr[101][101];
        char choice[101];
        printf("$ ");
        fflush(stdout);
        fgets(choice, sizeof(choice), stdin);

        if(strlen(choice) > 0) {
            choice[strlen(choice) - 1] ='\0';
        }
        strcpy(arr[i],choice);
        int indexWords = 0;
        char *p = strtok (choice, " ");
        char *arrayOfWords[101];
        while (p != NULL)
        {
            arrayOfWords[indexWords++] = p;
            p = strtok (NULL, " ");
        }
        arrayOfWords[indexWords] = NULL;
        if (!strcmp(choice,"history")) {
            pidArray[i] = getpid();
            int j = 0;
            for (j ; j<=i ; j++) {
                printf("%d %s\n",pidArray[j], arr[j]);
            }
        }
        else if (!strcmp(choice,"exit")) {
            exit(0);
        }
        else if (!strcmp(arrayOfWords[0],"cd")) {
            pidArray[i] = getpid();
            if (chdir(arrayOfWords[1]) != 0) {
                perror("chdir failed");
            }
        }
        else {
            pid_t pid;
            forkPid = fork();
            if(forkPid<0) {
                perror("fork failed");
            }
            if(forkPid > 0) {
                wait(NULL);
                pidArray[i] = forkPid;
            }

            if(forkPid == 0) {
                if(execvp(arrayOfWords[0], arrayOfWords) < 0) {
                    perror("execvp failed");
                    exit(1);
                }
            }

        }
        i++;
    }
}