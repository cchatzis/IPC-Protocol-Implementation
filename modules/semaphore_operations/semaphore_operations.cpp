#include "semaphore_operations.h"

using namespace std;

int semaphore_create(void){
    
    /* Creates a semaphore and returns it's ID. Returns -1 on ERROR. */

    int sem;

    semaphore_id++; /* Increment this, so the next semaphore we create has a tottaly new id */

    key_t sem_key = ftok("test.txt",semaphore_id);

    sem = semget(sem_key,1,IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    int errsv = errno;

    if( sem < 0 ){

        cout << "[ERROR]Cannot create semaphore. " << endl;

        return -1;

    }

    return sem;

}

int semaphore_p(int sem_id){

    /* Perfosms the P() operation on given semaphore, returns -1 on ERROR, else 0. */

	struct sembuf sem_b;

	sem_b.sem_num = 0;

	sem_b.sem_op = -1; /* P() */

	sem_b.sem_flg = 0;

	if( semop(sem_id, &sem_b, 1) == -1 ){

		cout <<  "[ERROR]Semaphore P() on " << sem_id << " failed." << endl;

		return -1;

	}

	return 0;
};

int semaphore_v(int sem_id){

    /* Perfosms the V() operation on given semaphore, returns -1 on ERROR, else 0. */

	struct sembuf sem_b;

	sem_b.sem_num = 0;

	sem_b.sem_op = 1; /* V() */

	sem_b.sem_flg = 0;

	if( semop(sem_id, &sem_b, 1) == -1 ){

		cout <<  "[ERROR]Semaphore P() on " << sem_id << " failed." << endl;

		return -1;

	}

	return 0;

}

int semaphore_init_1(int sem_id){

    /* Initializes specified sempahore, with value 1, returns -1 on ERROR, else 0. */

	union semun arg;

	arg.val=1;

	if( semctl(sem_id,0,SETVAL,arg) < 0 ){

        cout << "[ERROR]Cannot initialize semaphore to 1 with id: " << sem_id << endl;

        return -1;

    }

    return 0;
}

int semaphore_init_0(int sem_id){

    /* Initializes specified sempahore, with value 0, returns -1 on ERROR, else 0. */

	union semun arg;

	arg.val=0;

	if( semctl(sem_id,0,SETVAL,arg) < 0 ){

        cout << "[ERROR]Cannot initialize semaphore to 0 with id: " << sem_id << endl;

        return -1;

    }

    return 0;

}

int semaphore_delete(int sem_id){

    /* Deletes specified semaphore,returns -1 on ERROR, else 0. */

	union semun sem_union;

	if ( semctl(sem_id, 0, IPC_RMID) < 0 ){
        
        cout << "[ERROR]Cannot delete semaphore with id: " << sem_id << endl;

        return -1;

    }

    return 0;

};