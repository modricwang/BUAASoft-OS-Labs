#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10
#define NUM 50
#define LEFT ((i+N-1)%N)
#define RIGHT ((i+1)%N)
#define T 0
#define H 1
#define E 2
int state[N];
sem_t mutex;
sem_t locks[N];

void test(int i) {
    if (state[i] == H && state[LEFT] != E && state[RIGHT] != E) {
        state[i] = E;
        sem_post(&locks[i]);
    }
}

void take_forks(int i) {
    sem_wait(&mutex);
    state[i] = H;
    test(i);
    printf("The NO.%d philosopher started dining.\n", i);
    sem_post(&mutex);
    sem_wait(&locks[i]);
}

void put_forks(int i) {
    sem_wait(&mutex);
    state[i] = T;
    printf("The NO.%d philosopher finished dining.\n", i);
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);
}

void *eat(void *arg) {
    int i = *((int *) arg);
    take_forks(i);
    put_forks(i);
    return NULL;
}

int main() {
    srand((unsigned int) time(0));
    pthread_t p[NUM];
    sem_init(&mutex, 0, 1);
    int i = 0;
    for (i = 0; i < N; i++) {
        sem_init(&locks[i], 0, 0);
        state[i] = T;
    }
    int num[NUM];
    for (i = 0; i < NUM; i++) {
        num[i] = rand() % N;
    }
    for (i = 0; i < NUM; i++) {
        pthread_create(&p[i], NULL, eat, (void *) &num[i]);
    }
    return 0;
}
