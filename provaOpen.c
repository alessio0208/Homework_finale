#include <pthread.h>
#include <stdio.h>

/* this function is run by the second thread */
void *inc_x(void *x_void_ptr, char *name)
{

/* increment x to 100 */
int *x_ptr = (int *)x_void_ptr;
while(++(*x_ptr) < 100);

printf("%c increment finished\n",name);

/* the function must return something - NULL will do */
return NULL;

}


int main()
{

int x = 0, y = 0, z=0;

/* show the initial values of x and y */
printf("x: %d, y: %d\n", x, y);

/* this variable is our reference to the second thread */
pthread_t thread2;

pthread_t thread3;

/* create a second thread which executes inc_x(&x) */
if(pthread_create(&thread2, NULL, inc_x, &x, 'x')) {

fprintf(stderr, "Error creating thread 2\n");
return 1;

}

if(pthread_create(&thread3, NULL, inc_x, &z, 'z')) {

fprintf(stderr, "Error creating thread 3\n");
return 1;

}

/* increment y to 100 in the first thread */
while(++y < 100);

printf("y increment finished\n");

/* wait for the second thread to finish */
if(pthread_join(thread2, NULL)) {

fprintf(stderr, "Error joining thread\n");
return 2;

}

if(pthread_join(thread3, NULL)) {

fprintf(stderr, "Error joining thread\n");
return 2;

}


/* show the results - x is now 100 thanks to the second thread */
printf("x: %d, y: %d z: %d\n", x, y,z);

return 0;

}
