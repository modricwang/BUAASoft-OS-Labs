#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


#define CONSUMERS_COUNT 1
#define PRODUCERS_COUNT 1
#define BUFFSIZE 10

int g_buffer[BUFFSIZE];

unsigned short in = 0;
unsigned short out = 0;
unsigned short produce_id = 0;
unsigned short consume_id = 0;

sem_t g_sem_full;
sem_t g_sem_empty;
pthread_mutex_t g_mutex;

pthread_t g_thread[CONSUMERS_COUNT + PRODUCERS_COUNT];

void *consume(void *arg) {
    int inx = *((int *) arg);
    free(arg);
    int i;
    while (1) {
        printf("%d wait buffer empty\n", inx);
        sem_wait(&g_sem_empty);
        pthread_mutex_lock(&g_mutex);
        //消费产品
        //打印仓库当前状态
        for (i = 0; i < BUFFSIZE; ++i) {
            printf("%02d ", i);
            if (-1 == g_buffer[i]) {
                printf("null");
            } else {
                printf("%d ", g_buffer[i]);
            }
            if (i == in) {
                printf("\t<--consume");
            }
            printf("\n");
        }
        //消费产品
        consume_id = g_buffer[out];
        printf("%d begin consume product %d\n", inx, consume_id);
        g_buffer[out] = -1;
        out = (out + 1) % BUFFSIZE;
        printf("%d end consume product %d\n", inx, consume_id);
        pthread_mutex_unlock(&g_mutex);
        sem_post(&g_sem_full);
        sleep(1);
    }
    return NULL;
}

void *produce(void *arg) {
    int inx = *((int *) arg);
    free(arg);
    int i;
    while (1) {
        printf("%d wait buffer full\n", inx);
        sem_wait(&g_sem_full);
        pthread_mutex_lock(&g_mutex);
        //生产产品
        //打印仓库当前状态
        for (i = 0; i < BUFFSIZE; ++i) {
            printf("%02d ", i);
            if (-1 == g_buffer[i]) {
                printf("null");
            } else {
                printf("%d ", g_buffer[i]);
            }
            if (i == in) {
                printf("\t<--produce");
            }
            printf("\n");
        }
        printf("%d begin produce product %d\n", inx, produce_id);
        g_buffer[in] = produce_id;
        in = (in + 1) % BUFFSIZE;
        printf("%d end produce product %d\n", inx, produce_id++);
        pthread_mutex_unlock(&g_mutex);
        sem_post(&g_sem_empty);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    //初始化一个匿名的POSIX信号量
    sem_init(&g_sem_full, 0, BUFFSIZE);
    sem_init(&g_sem_empty, 0, 0);
    //互斥锁
    pthread_mutex_init(&g_mutex, NULL);
    int ret, i, *p;
    //初始化仓库
    for (i = 0; i < BUFFSIZE; ++i) {
        g_buffer[i] = -1;
    }
    //创建线程
    for (i = 0; i < CONSUMERS_COUNT; ++i) {
        p = (int *) malloc(sizeof(int));
        *p = i;
        ret = pthread_create(&g_thread[i], NULL, consume, p);
        if (ret != 0) {
            fprintf(stderr, "pthread_create:%s\n", strerror(ret));
            exit(EXIT_FAILURE);
        }
    }
    for (i = 0; i < PRODUCERS_COUNT; ++i) {
        p = (int *) malloc(sizeof(int));
        *p = i;
        ret = pthread_create(&g_thread[CONSUMERS_COUNT + i],
                             NULL, produce, p);
        if (ret != 0) {
            fprintf(stderr, "pthread_create:%s\n", strerror(ret));
            exit(EXIT_FAILURE);
        }
    }
    //主线程最好是等待子线程结束
    for (i = 0; i < CONSUMERS_COUNT + PRODUCERS_COUNT; ++i) {
        ret = pthread_join(g_thread[i], NULL);
        if (ret != 0) {
            fprintf(stderr, "pthread_join:%s\n", strerror(ret));
            exit(EXIT_FAILURE);
        }
    }
    sem_destroy(&g_sem_full);
    sem_destroy(&g_sem_empty);
    pthread_mutex_destroy(&g_mutex);
    return 0;
}
