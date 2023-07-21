#include<stdio.h>
#include<math.h>
#include<complex.h>
#include<stdlib.h>
#define pi 3.1416
#define F 1000.0
#define Fs 48000.0
#define n_point 512
int bin2dec(int);
int dec2bin(int);
int flip(int,int);
typedef struct {
	float real;
	float imag;
}comp;
comp *fft(float *lr,int n);
main()
{
    int i,j;
    float *signal,*spectral;
    comp *spect;
	signal=(float *)malloc(n_point*sizeof(float));
	spectral=(float *)malloc(n_point*sizeof(float));
	spect=(comp *)malloc(n_point*sizeof(comp));
	
    for(i=0;i<n_point;i++)
    {
    	*(signal+i)=(float)((float)100.0*sin((float)((float)2*(float)pi*(float)F*(float)i/(float)Fs)));
    }
    /*for(i=0;i<n_point;i++)
    {
    	*(signal+i)=(float)i;
    }*/
    
    
    spect=fft(signal,n_point);
    
    for(i=0;i<n_point;i++)
    {
    	*(spectral+i)=(float)sqrt((pow(spect[i].real,2)+pow(spect[i].imag,2)));
    }

  
FILE* fp = fopen("filtered.dat", "w");
for (int i = 0; i <n_point; i++)
{
        fprintf(fp, " %f\n", signal[i]);
        //printf( " %f\n", spectral[i]);
        
}

fclose(fp);
	
}
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

comp *fft(float *lr,int n)
{
  int i,j,k,a,s; /* initialisirng int variables which ae used in  loop*/
  int n1,N,c,h;  /* initialisirng int variables */
  

  comp *x,x1,x2;
  x=(comp*)malloc(n*sizeof(comp));

  n1=(int)(log10(n)/log10(2));
  
  printf("n1 is %d \n", n1);


  for (i=0;i<n;i++)
  {
	c=flip(i,n1);
	//printf("flipped of %d is %d \n",i,c);
    x[c].real=lr[i];
    x[c].imag=(float)0.0;
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
	            x1=x[j];
	            h=(int)((int)(N*k)-s);
	            x2.real=(float)(((x[h].real)*(float)cos((float)(2.0*pi*(float)((N/2)-s)/(float)N)))+((x[h].imag)*(float)sin((float)(2.0*pi*(float)((N/2)-s)/(float)N))));
	            x2.imag=(float)(((x[h].imag)*(float)cos((float)(2.0*pi*(float)((N/2)-s)/(float)N)))-((x[h].real)*(float)sin((float)(2.0*pi*(float)((N/2)-s)/(float)N))));
	            //printf("x2.real is %f and x2.imag is %f \n",x2.real,x2.imag);
	            x[j].real=(float)(x1.real+x2.real);
	            x[j].imag=(float)(x1.imag+x2.imag);
	            x[h].real=(float)(x1.real-x2.real);
	            x[h].imag=(float)(x1.imag-x2.imag);
		  }
	  }

  }
  
  /*for(i=0;i<n;i++)
    {
    	printf("x[c].real is %f and x[c].imag is %f \n",x[i].real,x[i].imag);
    }*/
  
  return(x);
 
}


