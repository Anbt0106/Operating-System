//producer consumer using concurrent processes
// gcc -pthread <program.c> -lrt
// remember to put -lrt at the end

#include <stdio.h>             
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define DELAY(S)	sleep(rand() % S)

void chaGuiTien(int amount);
void conRutTien(int amount);

// shared memory
struct mySharedMem {
	int account;
} *pMem;

int main() {
	pid_t pid,cid;
	int shmfd;
	int status;
	shm_unlink ("mySharedMemObject");
	shmfd = shm_open ("mySharedMemObject", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	assert (shmfd != -1);
	assert (ftruncate (shmfd, sizeof (struct mySharedMem)) != -1);
	pMem = mmap (NULL, sizeof (struct mySharedMem),PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
	assert (pMem != MAP_FAILED);
	pMem->account = 0;
	pid = fork();
	if (pid == 0) { // child process
		chaGuiTien(3);
		munmap (pMem, sizeof (struct mySharedMem)); // unmap the object
		return 0;
	}
	else { // parent process
		cid = fork();   // create a child process to run conRutTien()
		if (cid == 0) { // child process
			conRutTien(3); DELAY(3);
			munmap (pMem, sizeof (struct mySharedMem)); // unmap the object
			return 0;
		}
	}
	
	waitpid(pid, &status, 0);
	waitpid(cid, &status, 0);
	printf("Well done. Let's go home\n");
	munmap (pMem, sizeof (struct mySharedMem)); // unmap the object
	shm_unlink ("mySharedMemObject");	// remove the object
	return 0;
}

void chaGuiTien(int amount) {  
	int x, i;
	for (i=0; i<5; i++) {
		x = pMem->account;
		DELAY(2);
		x += amount;
		DELAY(2);
		pMem->account = x;
		DELAY(2);
		printf("Tao da 	gui. Account=%d\n", pMem->account);
	}
}

void conRutTien(int amount) { 
	int y, i;
	for (i=0; i<5; i++) {
		y = pMem->account;
		DELAY(2);
		y -= amount;
		DELAY(2);
		if (y>=0) { 
			pMem->account = y;
			printf("Con da rut. Account=%d\n", pMem->account);
		}
		else { printf("Khong du tien\n"); i--;}
		DELAY(2);
		
	}
}
