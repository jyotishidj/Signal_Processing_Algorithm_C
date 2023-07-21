
#include "dsk6416.h"
#include "dsk6416_aic23.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "complex.h"
#define pi 3.1416
#define p 1024
#pragma DATA_SECTION(voice,"CE0")
#pragma DATA_SECTION(magn,"CE0")
#pragma DATA_SECTION(x,"CE0")
/* Codec configuration settings */
DSK6416_AIC23_Config config = { \
    0x0017,  /* 0 DSK6416_AIC23_LEFTINVOL  Left line input channel volume */ \
    0x0017,  /* 1 DSK6416_AIC23_RIGHTINVOL Right line input channel volume */\
    0x00ff,  /* 2 DSK6416_AIC23_LEFTHPVOL  Left channel headphone volume */  \
    0x00ff,  /* 3 DSK6416_AIC23_RIGHTHPVOL Right channel headphone volume */ \
    0x0011,  /* 4 DSK6416_AIC23_ANAPATH    Analog audio path control */      \
    0x0000,  /* 5 DSK6416_AIC23_DIGPATH    Digital audio path control */     \
    0x0000,  /* 6 DSK6416_AIC23_POWERDOWN  Power down control */             \
    0x0043,  /* 7 DSK6416_AIC23_DIGIF      Digital audio interface format */ \
    0x0081,  /* 8 DSK6416_AIC23_SAMPLERATE Sample rate control */            \
    0x0001   /* 9 DSK6416_AIC23_DIGACT     Digital interface activation */   \
};

void fft(float lr[],int n);
int bin2dec(int);
int dec2bin(int);
int flip(int,int);

Int16 sinetable[48] = {
    0x0000, 0x10b4, 0x2120, 0x30fb, 0x3fff, 0x4dea, 0x5a81, 0x658b,
    0x6ed8, 0x763f, 0x7ba1, 0x7ee5, 0x7ffd, 0x7ee5, 0x7ba1, 0x76ef,
    0x6ed8, 0x658b, 0x5a81, 0x4dea, 0x3fff, 0x30fb, 0x2120, 0x10b4,
    0x0000, 0xef4c, 0xdee0, 0xcf06, 0xc002, 0xb216, 0xa57f, 0x9a75,
    0x9128, 0x89c1, 0x845f, 0x811b, 0x8002, 0x811b, 0x845f, 0x89c1,
    0x9128, 0x9a76, 0xa57f, 0xb216, 0xc002, 0xcf06, 0xdee0, 0xef4c
};

float voice[p];
float magn[p];
float _Complex x[p];

void main()
{
    DSK6416_AIC23_CodecHandle hCodec;
    DSK6416_init();
    hCodec = DSK6416_AIC23_openCodec(0, &config);
    int i,k=0,j=p;
    for(i=0;i<p;i++)
    {
    	voice[i]=(float)sinetable[k];
    	//printf("%f  ",voice[i]);
    	if (k==47)
    		k=0;
    	else
    		k++;

    }

    int un=(int)(log10(p)/log10(2));

      printf("n1 is:%d",un);

    fft(voice,j);
    for(i=0;i<p;i++)
            {
            	magn[i]=-1.0*sqrt(pow(creal(x[i]),2)+pow(cimag(x[i]),2));
            	//printf("%f  ",magn[i]);

            }

    while(1)
    {
        for(i=0;i<p;i++)
        {
        	while (!DSK6416_AIC23_write(hCodec, (Uint32)(Int16)magn[i]));
        	while (!DSK6416_AIC23_write(hCodec, (Uint32)(Int16)magn[i]));

        }

    }




    DSK6416_AIC23_closeCodec(hCodec);
}

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

void fft(float lr[],int n)
{
  int i,j,k,a,s; /* initialising int variables which are used in  loop*/
  int n1,N,c,h;  /* initialising int variables */

  float _Complex x1,x2;

  n1=(int)(log10(n)/log10(2));

  //printf("n1 is:%d",n1);


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
	            x2=(float _Complex)x[h]*cexp(-I*2.0*pi*(float)((N/2)-s)/N);
	            x[j]=x1+x2;
	            x[h]=x1-x2;
		  }
	  }

  }

}
