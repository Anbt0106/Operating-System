#include <pthread.h>
#include <stdio.h>

char *pHello="Hello world";
void *myThread(void* pStr) {
	// Đây là mã của luồng
	printf("%s\n", (char*) pStr);
	printf("Thread finished\n");
}

void main() {
	pthread_t tid;
	// tạo luồng *myThread, truyền tham số pHello
	pthread_create(&tid, NULL, myThread, (void*) pHello);
	// cha chờ *myThread kết thúc
	pthread_join(tid, NULL);
}