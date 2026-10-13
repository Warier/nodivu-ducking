#include "../src/dsp.h"
#include <cassert>
#include <cstdio>
int main(){plugin::Dsp d;float in[2]={0.5f,-0.25f},key[2]={0,0},out[2];
 for(int i=0;i<48000;++i)d.sample(in,key,out);assert(out[0]==in[0]&&out[1]==in[1]);
 key[0]=0.5f;for(int i=0;i<48000;++i)d.sample(in,key,out);assert(std::fabs(out[0]/in[0]-std::pow(10.f,-18.f/20))<1.e-4f);assert(out[1]==-out[0]*0.5f);
 float silent[2]={0,0};d.sample(silent,key,out);assert(out[0]==0&&out[1]==0);
 key[0]=0;for(int i=0;i<480000;++i)d.sample(in,key,out);assert(out[0]>0.499f);
 d.configure(0,0);key[1]=1;for(int i=0;i<48000;++i)d.sample(in,key,out);assert(out[0]>0.499f);d.reset();d.sample(in,key,out);assert(out[0]==in[0]);
 plugin::Dsp other;assert(other.gain==1);

 std::puts("PASS: independent DSP, stereo, state, controls, reset");}
