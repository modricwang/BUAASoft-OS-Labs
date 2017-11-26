#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define HUMANS 100
#define P_NUM 30
sem_t sem_db;
sem_t sem_m;
int rc;

void *reader(void *arg) {
    sem_wait(&sem_m);
    rc++;
    if (rc == 1)
        sem_wait(&sem_db);
    sem_post(&sem_m);
    printf("The reader NO.%02d is reading.\n", *((int *) arg));
    printf("The reader NO.%02d finished reading.\n", *((int *) arg));
    sem_wait(&sem_m);
    rc--;
    if (rc == 0)
        sem_post(&sem_db);
    sem_post(&sem_m);
}

void *writer(void *arg) {
    sem_wait(&sem_db);
    printf("The writer NO.%02d is writting.\n", *((int *) arg));
    printf("The writer NO.%02d finished writting.\n", *((int *) arg));
    sem_post(&sem_db);
}

int main() {
    srand((unsigned int) time(0));
    sem_init(&sem_db, 0, 1);
    sem_init(&sem_m, 0, 1);
    pthread_t p[P_NUM];
    int i;
    int t[P_NUM];
    rc = 0;
    for (i = 0; i < P_NUM; i++) {
        t[i] = rand() % HUMANS;
        if ((rand() % HUMANS) < P_NUM)
            pthread_create(&p[i], NULL, reader, (void *) &t[i]);
        else
            pthread_create(&p[i], NULL, writer, (void *) &t[i]);
    }
}

