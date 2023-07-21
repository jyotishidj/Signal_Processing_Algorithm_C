#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "complex.h"
#include "stdlib.h"
#define pi 3.1416
#define Fs 48000.0
#define N 14
#define n_point 1024

// complex type structure definition
typedef struct {
	float real;
	float imag;
}comp;

// array declaration
float signal[n_point];
float s_auto[2*n_point-1];
float A[n+1];
float spectral[n_point];
comp spect[n_point];

//function definition
void autocorr(); //result is stored in s_auto
//void lpc(int); //result is stored in A
//void freqz()//result is stored in spectral
main()
{
	
    int i;

    FILE* fread = fopen("eee.dat", "r");
	for (int i = 0; i <n_point; i++)
	{
        fscanf(fread, " %f", &signal[i]);
        
	}
	fclose(fread);
	
	/*for(i=0;i<n_point;i++)
	{
		signal[i]=(float)(i+0.5);
	}*/
	
	autocorr();
	
	FILE* fwrite = fopen("autocorr.dat", "w");
	for (i=0;i<(2*n_point-1);i++)
	{
		fprintf(fwrite,"%f\n",s_auto[i]);
	}
	
	/*for(i=0;i<(2*n_point-1);i++)
	{
	
		printf("%f\n",s_auto[i]);
	}
	for(i=0;i<(n_point);i++)
	{
	
		printf("%f\n",signal[i]);
	}*/
}

void autocorr()
{
	int j,k;
	
	for (j=0;j<n_point;j++)
	{
		s_auto[j+n_point-1]=0.0;
		for(k=0;j+k<n_point;k++)
		{
			s_auto[j+n_point-1]=s_auto[j+n_point-1]+(float)(signal[k]*signal[j+k]);
		}
		
	}
	
	for (j=0;j<n_point-1;j++)
	{
		s_auto[j]=s_auto[2*n_point-2-j];
	}
}
