/* hello_single.c
 *     a single threaded hello world program
 * */
#include <stdio.h>
#include <unistd.h>

#define NUM 5

void print_msg(char *msg);



int main()
{
    print_msg("hello");
    print_msg("world\n");

    return 0;
}



void print_msg(char *msg)
{
    int i;

    for (i = 0; i < NUM; ++i)
    {
	printf("%s", msg);
	fflush(stdout);
	sleep(1);
    }
}

