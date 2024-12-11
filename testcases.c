#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <process.h>
#include <math.h>

enum PIPES { READ, WRITE }; /* Constants 0 and 1 for READ and WRITE */
#define NUMPROBLEM 8

int main(int argc, char* argv[])
{
    int fdpipe[2];
    char hstr[20];
    int pid, problem, c;
    int termstat;
    
    setvbuf(stdout, NULL, _IONBF, 0);

    /* Open a set of pipes */
    if (_pipe(fdpipe, 256, O_BINARY) == -1)
        exit(1);

    /* Convert pipe read descriptor to string and pass as argument
     * to spawned program. Program spawns itself (argv[0]).
     */
    _itoa_s(fdpipe[READ], hstr, sizeof(hstr), 10);
    if ((pid = _spawnl(P_NOWAIT, "P1.exe", "P1.exe",
        hstr, NULL)) == -1)
        printf("Spawn failed");

    /* Put problem in write pipe. Since spawned program is
     * running simultaneously, first solutions may be done
     * before last problem is given.
     */
    //printf("Hello");
    _write(fdpipe[WRITE], "Hello\r", 6);
    _write(fdpipe[WRITE], "\r", 1);

     /*for (problem = 1000; problem <= NUMPROBLEM * 1000; problem += 1000)
     {

         printf("Son, what is the square root of %d?\n", problem);
         _write(fdpipe[WRITE], (char*)&problem, sizeof(int));

     }*/
    
    _cwait(&termstat, pid, WAIT_CHILD);
    if (termstat & 0x0)
        printf("Child failed\n");

    char buffer[512];
    int bytesRead = _read(fdpipe[READ], buffer, sizeof(buffer));

     /* Wait until spawned program is done processing. */
    while (1) {




    }

    _close(fdpipe[READ]);
    _close(fdpipe[WRITE]);
}