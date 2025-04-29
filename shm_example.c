#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/resource.h>
#include <string.h>
#include <errno.h>

int main() {

    struct rlimit rl;
    if (getrlimit(RLIMIT_MEMLOCK, &rl) == -1) {
        perror("getrlimit failed");
        return 1;
    }
    printf("RLIMIT_MEMLOCK: soft = %ld KB, hard = %ld KB\n",
           rl.rlim_cur / 1024, rl.rlim_max / 1024);

    size_t size = 64 * 1024;
    int shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        return 1;
    }

    char *shm_ptr = (char *)shmat(shmid, NULL, 0);
    if (shm_ptr == (char *)-1) {
        perror("shmat failed");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    const char *msg = "Hello, Shared Memory!";
    strcpy(shm_ptr, msg);
    printf("Data in shared memory: %s\n", shm_ptr);

    if (shmdt(shm_ptr) == -1) {
        perror("shmdt failed");
    }
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl(IPC_RMID) failed");
    }

    return 0;
}

