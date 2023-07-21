clc;
clear all
close all;
[x,Fs]=audioread('eee.wav');
sound(x,Fs)
plot(x)
z=x(10000:11023);
