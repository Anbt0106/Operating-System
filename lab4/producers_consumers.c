//producer consumer using concurrent threads

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<pthread.h>
#include<semaphore.h>

#define N 5
#define R 4
#define TRUE 1
#define up(sem) sem_post(sem)
#define down(sem) sem_wait(sem)

void *producer(void *arg);                           // Hàm của tiền trình sản xuất

void *consumer(void *arg);                           // Hàm của tiền trình tiêu thụ

void produce_item(int *pItem);                       //  tạo một mục

void enter_item(int item);                           // đưa mục vào hàng đợi

int get_item();                                      // lấy một mục từ hàng đợi

void consume_item(int item);                         // tiêu thụ mục

sem_t full, empty, mutex;                            // khai báo các semaphore

int queue[N];                                        // hàng đợi

int inIdx, outIdx, count;                            // chỉ số đầu vào, chỉ số đầu ra, số lượng mục trong hàng đợi

int main() {
    pthread_t pid, cid;                              // khai báo các tiến trình

    sem_init(&mutex, 0, 1);       // khởi tạo semaphore mutex

    sem_init(&empty, 0, N);       // khởi tạo semaphore empty

    sem_init(&full, 0, 0);        // khởi tạo semaphore full

    pthread_create(&pid, NULL, producer, NULL);         // tạo tiến trình sản xuất

    pthread_create(&cid, NULL, consumer, NULL);         // tạo tiến trình tiêu thụ

    pthread_join(pid, NULL);                                       // chờ tiến trình sản xuất kết thúc

    pthread_join(cid, NULL);                                       // chờ tiến trình tiêu thụ kết thúc
}

void *producer(void *arg) {
    int item;
    printf("P: Hello\n");
    while (TRUE) {
        produce_item(&item);                      // tạo một mục

        down(&empty);                                   // giảm giá trị của semaphore empty

        down(&mutex);                                   // giảm giá trị của semaphore mutex

        enter_item(item);                               // đưa mục vào hàng đợi

        up(&mutex);                                     // tăng giá trị của semaphore mutex

        up(&full);                                      // tăng giá trị của semaphore full

        sleep(rand() % R);
    }
    return NULL;
}

void *consumer(void *arg) {
    int item;
    printf("C: Hello\n");
    while (TRUE) {
        down(&full);                                    // giảm giá trị của semaphore full

        down(&mutex);                                   // giảm giá trị của semaphore mutex

        item = get_item();                              // lấy một mục từ hàng đợi

        up(&mutex);                                     // tăng giá trị của semaphore mutex

        up(&empty);                                     // tăng giá trị của semaphore empty

        consume_item(item);                             //   thụ mục

        sleep(rand() % R);
    }
    return NULL;
}

void produce_item(int *pItem) {
    (*pItem) = rand() % 100; // generate a random integer upto 99
    printf("P: Produce item %d\n", *pItem);
}

void enter_item(int item) {
    queue[inIdx] = item;
    inIdx = (inIdx + 1) % N;
    count++;
    printf("P: Enter item %d\n", item);
    if (count == N) printf("P: The queue is full\n");
}

int get_item() {
    int item;
    item = queue[outIdx];
    outIdx = (outIdx + 1) % N;
    count--;
    printf("C: Get item %d\n", item);
    if (count == 0) printf("C: The queue is empty\n");

    return item;
}

void consume_item(int item) {
    printf("C: Item %d is yum yum!\n", item);
}