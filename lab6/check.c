#include<stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>


//void main() {
//    if (fork()) { // parent process
//        printf("A\n");
//    } else { // child process
//        fork();
//        fork();
//        printf("B\n");
//    }
//    printf("C\n");
//}
void main() {
    for (int i = 0; i < 2; ++i) {
        if (fork() != 0) {
            printf("A\n");
        } else {
            fork();
            printf("B\n");
        }
        printf("C\n");
    }
}