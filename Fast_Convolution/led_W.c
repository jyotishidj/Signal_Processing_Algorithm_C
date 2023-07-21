#include "dsk6416.h"
#include "dsk6416_aic23.h"
#include "stdio.h"
#include "math.h"
#include "complex.h"
#include "stdlib.h"
#define Fs 48000
#define F1 37699
#define F2 50265
#define F3 62832
#pragma DATA_SECTION(voice,"CE0")
#pragma DATA_SECTION(result,"CE0")

Int16 sinetable[48] = {
    0x0000, 0x10b4, 0x2120, 0x30fb, 0x3fff, 0x4dea, 0x5a81, 0x658b,
    0x6ed8, 0x763f, 0x7ba1, 0x7ee5, 0x7ffd, 0x7ee5, 0x7ba1, 0x76ef,
    0x6ed8, 0x658b, 0x5a81, 0x4dea, 0x3fff, 0x30fb, 0x2120, 0x10b4,
    0x0000, 0xef4c, 0xdee0, 0xcf06, 0xc002, 0xb216, 0xa57f, 0x9a75,
    0x9128, 0x89c1, 0x845f, 0x811b, 0x8002, 0x811b, 0x845f, 0x89c1,
    0x9128, 0x9a76, 0xa57f, 0xb216, 0xc002, 0xcf06, 0xdee0, 0xef4c
};

/* Codec configuration settings */
DSK6416_AIC23_Config config = { \
    0x0017,  /* 0 DSK6416_AIC23_LEFTINVOL  Left line input channel volume */ \
    0x0017,  /* 1 DSK6416_AIC23_RIGHTINVOL Right line input channel volume */\
    0x00d8,  /* 2 DSK6416_AIC23_LEFTHPVOL  Left channel headphone volume */  \
    0x00d8,  /* 3 DSK6416_AIC23_RIGHTHPVOL Right channel headphone volume */ \
    0x0011,  /* 4 DSK6416_AIC23_ANAPATH    Analog audio path control */      \
    0x0000,  /* 5 DSK6416_AIC23_DIGPATH    Digital audio path control */     \
    0x0000,  /* 6 DSK6416_AIC23_POWERDOWN  Power down control */             \
    0x0043,  /* 7 DSK6416_AIC23_DIGIF      Digital audio interface format */ \
    0x0081,  /* 8 DSK6416_AIC23_SAMPLERATE Sample rate control */            \
    0x0001   /* 9 DSK6416_AIC23_DIGACT     Digital interface activation */   \
};

//function prototype definition

int bin2dec(int);
int dec2bin(int);
int flip(int,int);
float _Complex *fft(float *lr,int n);
float *ifft(float _Complex *lr,int n);
float *overlap(float *lr,float *h,int o,int l,int m);
float voice[48000];

void main()
{
	DSK6416_AIC23_CodecHandle hCodec;
    Uint16 lsr=0x8DF5u;
    DSK6416_init();
    Int i,j;
    Uint32 k;
    float h[81]={
    		  -1.664031134012e-17,-0.007670657211685,-0.002470468443381, 0.007337147777001,
    		   0.004988802181521,-0.006190042470114,-0.007309988514732, 0.004237375508315,
    		   0.009177230360659,-0.001562004646462, -0.01034217451056, -0.00167746890201,
    		    0.01058572818052, 0.005251380143059,-0.009737746794551,-0.008868878647738,
    		   0.007693901344571,  0.01219196410603,-0.004428206504948, -0.01485191834593,
    		  1.742951447827e-17,  0.01646548761049, 0.005445413861664, -0.01664685688962,
    		     -0.011682742057,  0.01500901683675,  0.01841904882169, -0.01114260710674,
    		   -0.02531168607147, 0.004545850681786,  0.03199054873995, 0.005564482815536,
    		   -0.03808301603088, -0.02078790942976,   0.0432396906178,  0.04537036742634,
    		   -0.04715895249012, -0.09534113145765,  0.04960841242538,   0.3171555070494,
    		     0.4539742000738,   0.3171555070494,  0.04960841242538, -0.09534113145765,
    		   -0.04715895249012,  0.04537036742634,   0.0432396906178, -0.02078790942976,
    		   -0.03808301603088, 0.005564482815536,  0.03199054873995, 0.004545850681786,
    		   -0.02531168607147, -0.01114260710674,  0.01841904882169,  0.01500901683675,
    		     -0.011682742057, -0.01664685688962, 0.005445413861664,  0.01646548761049,
    		  1.742951447827e-17, -0.01485191834593,-0.004428206504948,  0.01219196410603,
    		   0.007693901344571,-0.008868878647738,-0.009737746794551, 0.005251380143059,
    		    0.01058572818052, -0.00167746890201, -0.01034217451056,-0.001562004646462,
    		   0.009177230360659, 0.004237375508315,-0.007309988514732,-0.006190042470114,
    		   0.004988802181521, 0.007337147777001,-0.002470468443381,-0.007670657211685,
    		  -1.664031134012e-17
    		};

	float *result;
	result=(float *)malloc(50000*sizeof(float));

	 for(i=0;i<48000;i++)
	    {

	    	while (!DSK6416_AIC23_read(hCodec, &k));

	    	voice[i]=(float)(Int16)k+(double)sin((double)F1*i/Fs)+(double)sin((double)F2*i/Fs)+(double)sin((double)F3*i/Fs);

	    }


	result=overlap(voice,h,48000,48,81);

    for(i=0;i<49500;i++)
    {
    	while (!DSK6416_AIC23_write(hCodec, (Int16)(Uint32)result[i]));

    }




    DSK6416_AIC23_closeCodec(hCodec);
}


//function definition

int dec2bin(int num)
{
	int binary=0,base=1,remainder;
	while (num > 0)
    {
        remainder = num % 2;
        binary = binary + remainder * base;
        num = num / 2;
        base = base * 10;
    }
    return(binary);
}
int bin2dec(int num)
{
	int decimal=0,i=0,remainder;
	while (num > 0)
	{
		remainder=num%10;
		decimal=decimal+remainder*pow(2,i);
		num=num/10;
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
		remainder=num%10;
		out=out+remainder*pow(10,t-1);
		num=num/10;
		t--;

	}
	return(bin2dec(out));

}

float _Complex *fft(float *lr,int n)
{
  int i,j,k,a,s; /* initialisirng int variables which ae used in  loop*/
  int n1,N,c,h;  /* initialisirng int variables */


  float _Complex *x,x1,x2;
  x=(float _Complex*)malloc(n*sizeof(float _Complex));

  n1=log10(n)/log10(2);


  for (i=0;i<n;i++)
  {
	c=flip(i,n1);
    x[i]=(float _Complex)lr[c];
  }
  for (a=1;a<=n1;a++)
  {
	  N=pow(2,a);
	  for(k=1;k<=(n/N);k++)
	  {
		  for(s=1;s<=N/2;s++)
		  {
	            j=N*k-(N/2+s);
	            x1=x[j];
	            h=N*k-s;
	            x2=x[h]*cexp(-I*2*pi*((N/2)-s)/N);
	            x[j]=x1+x2;
	            x[h]=x1-x2;
		  }
	  }

  }
  return(x);

}


float *ifft(float _Complex *lr,int n)
{
  int i,j,k,a,s; /* initialising int variables which ae used in  loop*/
  int n1,N,c,h;  /* initialising int variables */

  float _Complex *x,x1,x2;
  x=(float _Complex*)malloc(n*sizeof(float _Complex));
  float *X;
  X=(float *)malloc(n*sizeof(float));

  n1=log10(n)/log10(2);


  for (i=0;i<n;i++)
  {
	c=flip(i,n1);
    x[i]=lr[c];
  }
  for (a=1;a<=n1;a++)
  {
	  N=pow(2,a);
	  for(k=1;k<=(n/N);k++)
	  {
		  for(s=1;s<=N/2;s++)
		  {
	            j=N*k-(N/2+s);
	            x1=x[j];

	            h=N*k-s;
	            x2=x[h]*cexp(I*2*pi*((N/2)-s)/N);

	            x[j]=0.5*(x1+x2);
	            x[h]=0.5*(x1-x2);
		  }
	  }

  }
  for(i=0;i<n;i++)
  {
  	X[i]=creal(x[i]);
  	//printf("\n x is <%f>",X[i]);
  }
  return(X);

}

float *overlap(float *lr,float *h,int o,int l,int m)
{
	int n;
	n=l+m-1;
	float _Complex *Y,*H1,prod[n];
	Y=(float _Complex *)malloc(n*sizeof(float _Complex));
	H1=(float _Complex *)malloc(n*sizeof(float _Complex));
	float y[n],H[n],*inter,*x;
	inter=(float *)malloc(n*sizeof(float));
	x=(float *)calloc(o+m-1,sizeof(float));
	int k=(o/l),i,j;

	for(j=0;j<m;j++)
	{
		H[j]=h[j];
	}
	for(j=m;j<n;j++)
	{
		H[j]=0;
	}
	H1=fft(H,n);

	for(i=0;i<k;i++)
	{
		for(j=0;j<l;j++)
		{
			y[j]=lr[i*l+j];
		}
		for(j=l;j<n;j++)
		{
			y[j]=0;
		}

		Y=fft(y,n);

		for(j=0;j<n;j++)
		{
			prod[j]=Y[j]*H1[j];
		}
		inter=ifft(prod,n);
		if(i==0)
		{
			for(j=0;j<n;j++)
			{
				x[j]=inter[j];
			}
		}
		else
		{
			for(j=0;j<n;j++)
			{
				x[i*l+j]=inter[j]+x[i*l+j];
			}

		}

	}
	return(x);

}
