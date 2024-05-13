#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

main()
{
    int fd;
    pid_t pid;
    char msg1[] = "Testing 1 2 3 ..\n";
    char msg2[] = "Hello, hello\n";

    if ((fd = creat("testfile", 0644)) == -1)
	return 0;

    if (write(fd, msg1, strlen(msg1)) == -1)
	return 0;

    if ((pid = fork()) == -1)
	return 0;

    if (write(fd, msg2, strlen(msg2)) == -1)
	return 0;

    close(fd);
    return 1;
}

