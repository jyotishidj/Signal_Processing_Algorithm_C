
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "complex.h"
#include "stdlib.h"
#define pi 3.1416
#define F 4000.0
#define Fs 48000.0
#define n_point 1024

int bin2dec(int);
int dec2bin(int);
int flip(int,int);
typedef struct {
	float real;
	float imag;
}comp;
void fft(float lr[],int n); //fft stores result in 'spect' variable

float signal[n_point];
float spectral[n_point];
comp spect[n_point];
main()
{

    int i;

    FILE* fread = fopen("eee.dat", "r");
	for (int i = 0; i <n_point; i++)
	{
        fscanf(fread, " %f", &signal[i]);
        
	}
	fclose(fread);
	
    fft(signal,n_point);

    for(i=0;i<n_point;i++)
    {
    	spectral[i]=(float)sqrt((pow(spect[i].real,2)+pow(spect[i].imag,2)));
    }
    
	
	FILE* fp = fopen("spect.dat", "w");
	for (int i = 0; i <n_point; i++)
	{
        fprintf(fp, " %f\n", spectral[i]);
        
	}
	fclose(fp);



}

//function definition starts
int dec2bin(int num)
{
	int binary=0,base=1,remainder;
	while (num > 0)
    {
        remainder = (int)((int)num % 2);
        binary = (int)((int)binary +(int)((int) remainder * (int)base));
        num =(int)((int) num / 2);
        base =(int)((int) base * 10);
    }
    return(binary);
}
int bin2dec(int num)
{
	int decimal=0,i=0,remainder;
	while (num > 0)
	{
		remainder=(int)((int)num%10);
		decimal=(int)((int)decimal+(int)((int)remainder*(int)pow(2,i)));
		num=(int)((int)num/10);
		i++;

	}
	return(decimal);

}
int flip(int number,int t)
{
	int num=dec2bin(number);

	int out=0,remainder;

	while(num>0)
	{
		remainder=(int)((int)num%10);
		out=(int)((int)out+(int)((int)remainder*(int)pow(10,t-1)));
		num=(int)((int)num/10);
		t--;

	}
	return(bin2dec(out));

}

void fft(float lr[],int n)
{
  int i,j,k,a,s; /* initialisirng int variables which ae used in  loop*/
  int n1,N,c,h;  /* initialisirng int variables */


  comp x1,x2;


  n1=(int)(log10(n)/log10(2));

  printf("n1 is %d \n", n1);


  for (i=0;i<n;i++)
  {
	c=flip(i,n1);
	//printf("flipped of %d is %d \n",i,c);
	spect[c].real=lr[i];
	spect[c].imag=(float)0.0;
    //printf("x[c].real is %f \n",x[c].real);
  }
  for (a=1;a<=n1;a++)
  {
	  N=(int)pow(2,a);

	  for(k=1;k<=(int)(n/N);k++)
	  {
		  for(s=1;s<=(int)(N/2);s++)
		  {
	            j=(int)((int)(N*k)-(((int)(N/2))+s));
	            x1=spect[j];
	            h=(int)((int)(N*k)-s);
	            x2.real=(float)(((spect[h].real)*(float)cos((float)(2.0*pi*(float)((N/2)-s)/(float)N)))+((spect[h].imag)*(float)sin((float)(2.0*pi*(float)((N/2)-s)/(float)N))));
	            x2.imag=(float)(((spect[h].imag)*(float)cos((float)(2.0*pi*(float)((N/2)-s)/(float)N)))-((spect[h].real)*(float)sin((float)(2.0*pi*(float)((N/2)-s)/(float)N))));
	            //printf("x2.real is %f and x2.imag is %f \n",x2.real,x2.imag);
	            spect[j].real=(float)(x1.real+x2.real);
	            spect[j].imag=(float)(x1.imag+x2.imag);
	            spect[h].real=(float)(x1.real-x2.real);
	            spect[h].imag=(float)(x1.imag-x2.imag);
		  }
	  }

  }

  /*for(i=0;i<n;i++)
    {
    	printf("x[c].real is %f and x[c].imag is %f \n",x[i].real,x[i].imag);
    }*/


}


