#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

main()
{
    int n;

    for (n = 0; n < 10; ++n)
    {
        printf("my pid = %d, n = %d\n", getpid(), n);
	
	sleep(1);
	
	fork();
    }
}

