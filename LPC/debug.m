clc;
clear all;
close all;
x=load('aa16.dat');
A=lpc(x,14);
A1=load('lpspectrum.dat');
spect=abs(freqz(1,A));
subplot(211)
plot(log(spect));
subplot(212)
plot(log(A1),'r');


fid=fopen('sine.h','w');
fprintf(fid,'float signal[48]={'); %print array name,"={"
fprintf(fid,'%d, ' ,signal(1:48)); %print 127 points
fprintf(fid,'};\n'); %print closing bracket
fclose(fid);