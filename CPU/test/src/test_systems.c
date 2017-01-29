#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pids[2], pid;
    int i, status, n = 2;


    /* Start system processes */
    for (i = 0; i < n; ++i) {
        
        if ((pids[i] = fork()) < 0) {
            perror("fork");
            abort();
        }
        else if (pids[i] == 0) {
            if (i == 0){
                /* run pdm test */
                //printf("Do nothing...\n");
                printf("run pdm test\n");
                execl("/home/minieusouser/CPU/test/bin/pdm_acq", "pdm_acq", 0, NULL);
            }
            else if (i == 1){
                /* run camera test */
                //printf("run nothing here\n");
                printf("run camera test\n");
                execl("/home/minieusouser/CPU/cameras/test/multiplecam_test.sh", "multiplecam_test.sh", 0, NULL);
            }
            //else if (i == 2){
                /* run photodiode test */
                //printf("Do nothing\n");
                //printf("run photodiode test\n");
                //execlp("/home/minieusouser/CPU/analog/bin/test_photodiode", "test_photodiode", argv[1], NULL);
            //}

        /* not reached */        
        exit(0);
        } 
    }
    

    /* Wait for system processes to exit */
    while (n > 0) {
      pid = wait(&status);
      printf("Process with PID %ld exited with status 0x%x.\n", (long)pid, status);
      --n;  
    }

    return 0;
}