#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>

int buffer[27], cnt;
sem_t sem, syncSem1, syncSem2;

void productor() {
    while (1) {
        for (int i = 0; i < 27; i++) {
            sem_wait(&sem);
            if (buffer[i] == -1) {
                buffer[i] = rand() % 100;
                printf("produce %d\n", buffer[i]);
                cnt ++;
                if (cnt > 0) {
                    sem_post(&sem);
                }
            }
        }
        
    }
}
void cosumer() {
    while (1) {
        for (int j = 0; j < 27; j++) {
            if (buffer[j] != -1) {
                printf("cosume %d\n", buffer[j]);
                buffer[j] = -1;
                cnt --;
                if (cnt <= 0) {
                    sem_post(&sem);
                }
            } else {
	        sem_wait(&sem);
            }
        }
    }
}

int main() {
    srand(0);
    for (int i = 0; i < 27; i++) {
        buffer[i] = -1;
    }
    pthread_t id_1,id_2;
    sem_init(&sem, 0, 1);
    pthread_create(&id_1, NULL, (void *)productor, NULL);
    pthread_create(&id_2, NULL, (void *)cosumer, NULL);
    pthread_join(id_1, NULL);
    pthread_join(id_2, NULL);
    return 0;
}
