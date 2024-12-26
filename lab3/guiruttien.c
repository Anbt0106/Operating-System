// Gửi và rút tiền với race condition
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define DELAY    sleep(rand() % 4) // sleep for a random time (0-3 seconds)

int account; // bank account

void *chaGuiTien() {
    int x, i;
    for (i = 0; i < 5; i++) {
        x = account;
        DELAY;
        x += 3;
        DELAY;
        account = x;
        DELAY;
        printf("Cha da gui. Account=%d\n", account);
    }
    return NULL;
}

void *conRutTien() {
    int y, i;
    for (i = 0; i < 5; i++) {
        y = account;
        DELAY; // sleep for a random time (0-3 seconds)
        y -= 2;
        DELAY;
        if (y >= 0) {
            account = y;
            printf("Con da rut. Account=%d\n", account);
        } else {
            printf("Khong du tien\n");
            i--;
        }
    }
    return NULL;
}

void main() {
    pthread_t tid1, tid2;

    printf("Account=%d\n", account);
    pthread_create(&tid1, NULL, chaGuiTien, NULL); // tao luong chaGuiTien
    pthread_create(&tid2, NULL, conRutTien, NULL); // tao luong conRutTien

    pthread_join(tid1, NULL);   // cha cho luong chaGuiTien ket thuc khong nhan gia tri tra ve
    pthread_join(tid2, NULL);   // cha cho luong conRutTien ket thuc khong nhan gia tri tra ve
}
