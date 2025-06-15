#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#define N(h)h<<7&(1<<28)-1
int main(int R,char**V){
FILE*I=fopen(V[1],"rb");
char*Q=calloc(1<<26,1),c;
int T,S=0,d,h=0;
if(R==3){
int*M=calloc(1<<28,4);
for(;fread(&c,1,1,I)==1;M[h=c<0?h:N(h)|c]++);
for(M[0]=0;S<(1<<28);S+=1<<7,R=0){
for(d=0;d<128;d++)if(M[S+d]>R)R=M[S+d];
for(d=0,c=0;d<128;d++){
if(R)c=3*M[S+d]/R;
Q[S+d>>2]|=c<<((S+d)&3)*2;
}}
fwrite(Q,1,1<<26,fopen(V[2],"wb"));
}else{
fread(Q,1,1<<26,I);srand(time(0));
for(R=0;R<7;R++) {
h=N(h);char W[128];T=0;
for(d=0,c=32;d<128;d++)T+=W[d]=Q[(h+d)/4]>>(h+d&3)*2&3;
if(T){d=rand()%T;S=0;for(c=0;S<=d;c++,S+=W[c]);}
h|=putchar(c);if(c^32)R=0;
}}}
