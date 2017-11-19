#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <wait.h>

sem_t sem;

int shared;
int pause_time = 100;

void *thread1_function() {
    sem_wait(&sem);
    ++shared;
    for (int i = 0; i < 3; i++) {
        printf("thread1 value = %d\n", shared);
        wait(&pause_time);
    }
    sem_post(&sem);
}

void *thread2_function() {
    sem_wait(&sem);
    --shared;
    for (int i = 0; i < 3; i++) {
        printf("thread2 value = %d\n", shared);
        wait(&pause_time);
    }
    sem_post(&sem);
}

int main() {
    shared = 0;
    sem_init(&sem, 0, 1);
    pthread_t p1, p2;
    pthread_create(&p1, NULL, thread1_function, NULL);
    pthread_create(&p2, NULL, thread2_function, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
}
