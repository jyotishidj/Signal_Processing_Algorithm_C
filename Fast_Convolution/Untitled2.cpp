#include<stdio.h>
#include<complex.h>
#include<math.h>
#include<stdlib.h>

int *addition(int*y,int z);
main()
{
	int i;
	int *y;
	y=(int*)malloc(100*sizeof(int));
	int x[5];

	for(i=0;i<5;i++)
	{
		x[i]=i;
		printf("the number is %d\n",x[i]);
    }
    printf("size of %d\n",sizeof(x));
    y=addition(x,2);
    for(i=0;i<5;i++)
	{
		printf("the number after function %d\n",y[i]);
    }
}

int *addition(int*y,int z)
{
	printf("size of %d\n",sizeof(y));
	int i;
	for(i=0;i<5;i++)
	{
		y[i]=z+i+y[i];
		printf("the number in function %d\n",y[i]);
    }
	return(y);
}
