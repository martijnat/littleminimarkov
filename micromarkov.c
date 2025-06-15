#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#define next(h) (((h << 7))&((1<<28)-1))
int main(int argc, char *argv[]) {
  FILE *I=fopen(argv[1],"rb");
  char c,*Q=calloc(1<<26,1);
  int d,h=0;
  if(argc==3){
    int *M = calloc(1<<28,4);
    while (fread(&c, 1, 1, I) == 1) {
      if (c<0) continue;
      h=next(h)|c;
      M[h]++;
    }
    M[0]=1;
    for (int b=0;b<(1<<28);b+=1<<7){
      int mv = 0;
      for(d=0;d<128;d++)if(M[b+d]>mv)mv=M[b+d];
      for(d=0,c=0;d<128;d++){
        if (mv)c=3*M[b+d]/mv;
        Q[b+d>>2]|=c<<((b+d)&3)*2;
      }}
    fwrite(Q,1,1<<26,fopen(argv[2],"wb"));
  }else{
    fread(Q,1,1<<26,I);
    srand(time(NULL));
    int sr=0;
    while(sr<7) {
      h = next(h);
      char W[128];
      int T=0;
      for(d=0;d<128;d++)T+=W[d]=Q[(h+d)/4]>>(h+d&3)*2&3;
      c=32;
      if (T) {
        int r=rand()%T;
        int sum=0;
        for(d=0;d<128;d++){
          sum+=W[d];
          if(sum>r){
            c=d;
            break;
          }}}
      h|=c;
      putchar(c);
      sr++;
      if(c^32)sr=0;
    }}}
