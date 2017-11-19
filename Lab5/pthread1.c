#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <wait.h>

int shared;
int pause_time = 300;

void *thread1_function() {
    ++shared;
    for (int i = 0; i < 3; i++) {
        printf("thread1 value = %d\n", shared);
        wait(&pause_time);
    }
}

void *thread2_function() {
    --shared;
    for (int i = 0; i < 3; i++) {
        printf("thread2 value = %d\n", shared);
        wait(&pause_time);
    }
}

int main() {
    shared = 0;
    pthread_t p1, p2;
    pthread_create(&p1, NULL, thread1_function, NULL);
    pthread_create(&p2, NULL, thread2_function, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
}
