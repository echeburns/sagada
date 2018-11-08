#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void * start_server(void *param)
{	
	int *y = (int *)param;
	
	while(1)
	{
		printf("count %d\n", (*y)++);
		sleep(1);
	};
}


int main(int argc, char *argv[])
{
	int tid;
	pthread_attr_t attr;
	int ctr=0;
	
	pthread_attr_init(&attr);
	int rc = pthread_create(&tid, &attr, start_server, (void *)&ctr);
	printf("pthread_create = %d\n", rc);
	
	while(1)
	{
		printf("main %d\n", ctr);
		sleep(1);
	};
}
