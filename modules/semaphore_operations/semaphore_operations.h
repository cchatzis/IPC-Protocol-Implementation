#include <iostream>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h> 
#include <sys/stat.h>
#include <errno.h>

using namespace std;

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

/*------------------------------------- Semaphore Operations ---------------------------------------*/

static int semaphore_id = 90;

int semaphore_create(void); /* Creates a semaphore and returns it's ID. Returns -1 on ERROR. */

int semaphore_init_0(int sem_id); /* Initializes specified sempahore, with value 0, returns -1 on ERROR, else 0. */

int semaphore_init_1(int sem_id); /* Initializes specified sempahore, with value 1, returns -1 on ERROR, else 0. */

int semaphore_p(int sem_id); /* Perfoms the P() operation on given semaphore, returns -1 on ERROR, else 0. */

int semaphore_v(int sem_id); /* Perfoms the V() operation on given semaphore, returns -1 on ERROR, else 0. */

int semaphore_delete(int sem_id); /* Deletes specified semaphore,returns -1 on ERROR, else 0. */

/*--------------------------------------------------------------------------------------------------*/