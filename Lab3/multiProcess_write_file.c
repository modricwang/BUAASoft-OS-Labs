#include <stdio.h>
#include <zconf.h>
#include <wait.h>
#include <stdlib.h>
#include <sys/file.h>

int main() {

    pid_t pid;
    if ((pid = fork()) < 0) {
        printf("Fork failed!\n");
        exit(0);
    }


    if (pid > 0) { //main Process
        FILE *file = fopen("test.txt", "a");
        if (file == NULL) {
            printf("main Process: File open failed!\n");
            exit(0);
        }

        int loop_cnt = 20;
        while (loop_cnt--) {
            if (flock(fileno(file), LOCK_EX | LOCK_NB) == 0) {
                printf("main Process: locked file\n");
                if (fprintf(file, "main Process write successful!\n") > 0)
                    printf("main Process write successful!\n");
                else
                    printf("main Process write failed!\n");

                if (flock(fileno(file), LOCK_UN) == 0) {
                    printf("main Process: unlocked file\n");
                } else {
                    printf("main Process: unlock failed!\n");
                }
            } else {
                printf("main Process: lock failed!\n");
            }


        }
    } else {    //sub Process
        FILE *file = fopen("test.txt", "a");
        if (file == NULL) {
            printf("sub Process: File open failed!\n");
            exit(0);
        }
        int loop_cnt = 20;
        while (loop_cnt--) {
            if (flock(fileno(file), LOCK_EX | LOCK_NB) == 0) {
                printf("sub Process: locked file\n");
                if (fprintf(file, "sub Process write successful!\n") > 0)
                    printf("sub Process write successful!\n");
                else
                    printf("sub Process write failed!\n");

                if (flock(fileno(file), LOCK_UN) == 0) {
                    printf("sub Process: unlocked file\n");
                } else {
                    printf("sub Process: unlock failed!\n");
                }
            } else {
                printf("sub Process: lock failed!\n");
            }


        }

    }
    waitpid(pid, NULL, 0);

    return 0;
}
