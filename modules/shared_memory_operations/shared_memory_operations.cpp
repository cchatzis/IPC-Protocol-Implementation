#include "shared_memory_operations.h"

using namespace std;

int shared_memory_initializer(int size){

    /* Initializes a shared memory segment with given size in bytes and returns its ID, or -1 on error */

    int shmid;

    key_t sh_key;

    sh_key = ftok("test.txt",shared_mem_id);

    shared_mem_id++; /* Increment this, so the next shared memory segment we create has a tottaly new id */

    shmid = shmget(sh_key,size, 0666 | IPC_CREAT);

    if( shmid < 0 ){

        cout << "[ERROR]Error in creating shared memory segment. (shmget)" << endl;

        return -1;

    }

    return shmid;

}

char * get_pointer_to_shared_memory(int id){
    
    /* Returns pointer to pointer of given ID, or NULL on error */

    return (char *) shmat(id,NULL,0); 
  
}

int shared_memory_deinitializer(int shmid){ 

    /* Deletes all shared memory segments, return -1 on ERROR, else 0 */

    if( shmctl(shmid,IPC_RMID,0) == -1 ){

        cout << "[ERROR]Cannot delete " << shmid << " memory segment. (shmctl)" << endl;

        return -1;

    }    

    return 0;

}