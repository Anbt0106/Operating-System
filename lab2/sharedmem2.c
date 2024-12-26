//producer consumer using concurrent processes
// gcc <program.c> -lrt
// remember to put -lrt at the end

#include <stdio.h>             
#include <string.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

// shared memory
struct mySharedMem {
	char buf[256];
} *pMem;
 
int main() {
	pid_t pid;
	int shmfd;
	shm_unlink ("mySharedMemObject");
	shmfd = shm_open ("mySharedMemObject", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	assert (shmfd != -1);
	assert (ftruncate (shmfd, sizeof (struct mySharedMem)) != -1);
	pMem = mmap (NULL, sizeof (struct mySharedMem),PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
	assert (pMem != MAP_FAILED);
	printf("Type a message to the child: ");
   	fgets(pMem->buf , 255, stdin);
	pid = fork();
	if (pid == 0) {
        for (int i = 0; i < strlen(pMem->buf); i++) {
            pMem->buf[i] = toupper(pMem->buf[i]);
        }
		munmap (pMem, sizeof (struct mySharedMem)); 
		return 0;
	}
	wait(NULL);
	
	printf("Message from the child: %s\n", pMem->buf);
	munmap (pMem, sizeof (struct mySharedMem)); 
	shm_unlink ("mySharedMemObject");	
	return 0;
}
