#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "complex.h"
#include "stdlib.h"
#define pi 3.1416
#define Ns 512
#define N 14
#include "aa16_512.h"
#define n_point 512
#define Fs 16000.0
//#define n_point 256
//#define Fs 8000.0
//#define n_point 1024
//#define Fs 48000.0


// complex type structure definition
typedef struct {
	float real;
	float imag;
}comp;

//function definition
void autocorr(); //result is stored in s_auto
void lpc(); //result is stored in A
void freqz();//result is stored in spectral

// array declaration
//float signal[n_point];
float s_auto[2*n_point-1];
float A[N+1];
float spectral[Ns];

main()
{
	
    int i,j,num,F[3];

    /*FILE* fread = fopen("ee16.dat", "r");
	for (int i = 0; i <n_point; i++)
	{
        fscanf(fread, " %f", &signal[i]);
        
	}
	fclose(fread); */
	
	autocorr();
	lpc();
	freqz();
	
    num=0;
    for (j=2;j<Ns-1;j++)
    {
        if (spectral[j]>spectral[j-1] && spectral[j]>spectral[j+1])
        {
            F[num]=(int)(j*Fs/(2*Ns));
            num++;
            if (num==3)
                break;
		}
        else if (spectral[j]>spectral[j-1] && spectral[j]>spectral[j+1] && spectral[j]==spectral[j-2])
        {
            F[num]=(int)(j*Fs/(2*Ns));
            num++;
            if (num==3)
                break;
    	}
    }
  

	
	FILE* fwrite = fopen("lpspectrum.dat", "w");
	for (i=0;i<Ns;i++)
	{
		fprintf(fwrite,"%f\n",spectral[i]);
	}
	
	/*for(i=0;i<(2*n_point-1);i++)
	{
	
		printf("%f\n",s_auto[i]);
	}*/
	for(i=0;i<3;i++)
	{
	
		printf("%d\n",*(F+i));
	}
	
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

void lpc()
{
	float a[N],a1[N],num,din,k;
	int i,j;
	
	k=(float)(s_auto[n_point]/s_auto[n_point-1]);
	a[0]=(float)(-1.0*k);
	k=(float)((s_auto[n_point+1]+a[0]*s_auto[n_point])/(s_auto[n_point-1]+a[0]*s_auto[n_point]));
	
	for (j=1;j<N;j++)
	{
	    for (i=0;i<=j;i++)
	    {
	        if (i==j)
	            a1[i]=(float)(-1.0*k);
	        else
	            a1[i]=(float)(a[i]-k*a[j-i-1]);
	
		}
		for (i=0;i<=j;i++)
		{
			a[i]=a1[i];
		}

	    num=0;
		din=0;
	    for (i=0;i<=j;i++)
	    {
	        num=(float)(num+a[i]*s_auto[n_point+j-i]);
	        din=(float)(din+a[i]*s_auto[n_point+i]);
		}
	    k=(float)((s_auto[n_point+j+1]+num)/(s_auto[n_point-1]+din));
	}
	
	A[0]=1;
	for(i=1;i<=N;i++)
	{
	A[i]=a[i-1];   
	}
}

void freqz()
{
	int i,j;
	comp inter;
	
	for (i=0;i<Ns;i++)
	{
		inter.real=0;
		inter.imag=0;
		for (j=0;j<=N;j++)
		{
			inter.real=(float)(inter.real+A[j]*((float)cos((float)(pi*i*j/Ns))));
			inter.imag=(float)(inter.imag-A[j]*((float)sin((float)(pi*i*j/Ns))));	
		}
		spectral[i]=(float)(1/sqrt((float)(pow(inter.real,2)+pow(inter.imag,2))));
		
	}
	
}


