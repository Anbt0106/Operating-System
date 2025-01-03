//producer consumer using concurrent processes
// gcc -pthread <program.c> -lrt
// remember to put -lrt at the end

#include <stdio.h>             
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define N 2
#define R 4
#define TRUE 1
#define up(sem) sem_post(sem)
#define down(sem) sem_wait(sem)

void producer(void);
void consumer(void);
void produce_item(int* pItem);
void enter_item(int item);
int get_item();
void consume_item(int item);

// shared memory
struct mySharedMem {
	sem_t full,empty,mutex;
	int queue[N];
	int inIdx, outIdx, count;
	int start;
} *pMem;

int main() {
	pid_t pid,cid;
	int shmfd;
	int status;
		
	// Create unsized shared memory object;
	shmfd = shm_open ("mySharedMemObject", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	assert (shmfd != -1);
	
	// Resize the region to store 1 struct instance
	assert (ftruncate (shmfd, sizeof (struct mySharedMem)) != -1);
	
	// Map the object into memory so file operations aren't needed
	pMem = mmap (NULL, sizeof (struct mySharedMem),PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
	assert (pMem != MAP_FAILED);
	
	
	// Initialize shared data, done only by the 1st arriver
	if (pMem->start==0) {
		sem_init(&(pMem->mutex),1,1);
		sem_init(&(pMem->empty),1,N);
		sem_init(&(pMem->full),1,0);
		pMem->inIdx = 0;
		pMem->outIdx = 0;
		pMem->count = 0;
		pMem->start=1;
		printf("Press enter to continue\n");
		getchar();
	}
	
	
	producer();
	munmap (pMem, sizeof (struct mySharedMem)); // unmap the object
	shm_unlink ("mySharedMemObject");	// remove the object
	return 0;
}

void producer() {
	int i, item;
	printf("P: Hello\n");
	for (i=0; i<10; i++) {
		produce_item(&item);
		down(&(pMem->empty));
		
		down(&(pMem->mutex));
		enter_item(item);
		up(&(pMem->full));
		up(&(pMem->mutex));
		
		sleep(rand() % R);
    }
}

void produce_item(int* pItem) {
	(*pItem)=rand() % 100; // generate a random integer upto 99
	printf("P: Produce item %d\n", *pItem);
}

void enter_item(int item) {
	pMem->queue[pMem->inIdx]=item;
	pMem->inIdx = (pMem->inIdx+1) % N;
	pMem->count++;
	printf("P: Enter item %d\n", item);
	if (pMem->count==N) printf("P: The queue is full\n");
}


