#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#define next(h) (((h << 7))&((1<<28)-1))
int main(int argc, char *argv[]) {
FILE *input = fopen(argv[1], "rb");
char *q2_model = calloc(1<<26,1);
int h=0;
if(argc==3){
int *model = calloc(1<<28,4);
char d;
while (fread(&d, 1, 1, input) == 1) {
if (d<0) continue;
h=next(h)|d;
model[h]++;
}
model[0]=1;
for (int base=0;base<(1<<28);base+=1<<7){
int mval = 0;
for (int d=0;d<128;d++)if(model[base+d]>mval)mval=model[base+d];
for (int d=0;d<128;d++){
char val=0;
if (mval) val = (3 * model[base + d]) / mval;
q2_model[(base+d)>>2]|=val<<(((base+d)&3)*2);
}}
FILE *model_file = fopen(argv[2], "wb");
fwrite(q2_model,1,1<<26,model_file);
}else{
fread(q2_model,1,1<<26,input);
srand(time(NULL));
int sr=0;
while(sr<7) {
h = next(h);
char weights[128];
int total_weight=0;
for (int d=0;d<128;d++){
weights[d]=(q2_model[(h+d)>>2]>>(((h+d)&3)*2))&3;
total_weight+=weights[d];
}
char next_byte=32;
if (total_weight) {
int r=rand()%total_weight;
int sum=0;
for (int d=0;d<128;d++){
sum+=weights[d];
if(sum>r){
next_byte=d;
break;
}}}
h|=next_byte;
putchar(next_byte);
sr++;
if(next_byte != 32)sr=0;
}}}
