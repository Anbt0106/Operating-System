#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define up(sem) sem_post(sem)
#define down(sem) sem_wait(sem)
#define DELAY(R) sleep(rand() % R)
#define MAX_CLIENTS 25

// Function prototypes...
void *client(void *);

void *barber(void *);

sem_t clients;
sem_t barbers;
sem_t mutex, seatBelt;
int waiting, numChairs, numClients;
int allDone = 0;

int main(int argc, char *argv[]) {
    pthread_t btid;
    pthread_t ctid[MAX_CLIENTS];
    int i;
    int clientID[MAX_CLIENTS];

    // Check to make sure there are the right number of command line arguments.
    if (argc != 3) {
        printf("Usage: %s <Num clients> <Num Chairs>\n", argv[0]);
        exit(-1);
    }

    // Get the command line arguments and convert them into integers.
    numClients = atoi(argv[1]);
    numChairs = atoi(argv[2]);

    // Make sure the number of threads is less than the number of clients we can support.
    if (numClients > MAX_CLIENTS) {
        printf("The maximum number of clients is %d.\n", MAX_CLIENTS);
        exit(-1);
    }

    // Initialize the numbers array.
    for (i = 0; i < MAX_CLIENTS; i++) clientID[i] = i;

    // Initialize the semaphores with initial values...
    sem_init(&clients, 0, 0);
    sem_init(&barbers, 0, 0);
    sem_init(&mutex, 0, 1);
    sem_init(&seatBelt, 0, 0);

    // Create the barber.
    pthread_create(&btid, NULL, barber, NULL);

    // Create the clients.
    for (i = 0; i < numClients; i++)
        pthread_create(&ctid[i], NULL, client, (void *) &clientID[i]);

    // Join each of the threads to wait for them to finish.
    for (i = 0; i < numClients; i++)
        pthread_join(ctid[i], NULL);

    // When all of the clients are finished, kill the barber thread.
    allDone = 1;
    up(&clients);  // Wake the barber so he will exit.
    pthread_join(btid, NULL);
}

void *client(void *number) {
    int num = *(int *) number;

    DELAY(10);
    printf("client %d arrives\n", num);
    down(&mutex);
    if (waiting < numChairs) {
        waiting++;
        up(&clients); // wake up the barber
        printf("Client %d is waiting for turn\n", num);
        up(&mutex);  // release the mutex
        down(&barbers);  // try to get the barber's attention
        printf("Client %d is having a haircut\n", num); // get a haircut
        down(&seatBelt); // put on the seat belt until the barber is finished
        printf("Client %d is going home\n", num);
    } else {
        printf("Shop full. Client %d leaves\n", num);
        up(&mutex);
    }
    return NULL;
}

void *barber(void *junk) {
    while (!allDone) {
        // sleep until someone arrives and wakes you..
        printf("The barber is sleeping\n");
        down(&clients);  // get a client from the waiting room

        if (!allDone) {
            down(&mutex);   // acquire the mutex lock
            waiting--;  // decrease the number of clients in the waiting room
            up(&barbers);  // wake up the client
            up(&mutex);   // release the mutex lock
            printf("The barber is cutting hair\n");
            sleep(2); // it takes a while
            up(&seatBelt); // release the client when finished
            printf("The barber has finished cutting hair.\n");
        } else {
            printf("The barber is going home for the day.\n");
        }
    }
    return NULL;
}
