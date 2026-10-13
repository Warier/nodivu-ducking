#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
struct Parameter { const char* name; double min,max,initial; bool stepped; };
// Independent DSP: no CLAP, Nodivu, OS, heap, locks or I/O.
namespace plugin {
constexpr bool sidechain=true;
constexpr Parameter parameters[]={
 {"Redução (dB)",0,48,18,false},{"Sensibilidade (dBFS)",-72,0,-32,false},
 {"Ataque (ms)",1,200,15,false},{"Retorno (ms)",10,2000,350,false},{"Espera (ms)",0,1000,180,false}};
struct Dsp {
 float envelope=0,gain=1,duckGain=0.12589254f,threshold=0.02511886f,attack=0,release=0;
 uint32_t hold=8640,remaining=0;
 Dsp(){configure(0,18);configure(1,-32);configure(2,15);configure(3,350);}
 void reset(){envelope=0;gain=1;remaining=0;}
 void configure(uint32_t id,double value){
  if(id==0)duckGain=static_cast<float>(std::pow(10.,-value/20.));
  if(id==1)threshold=static_cast<float>(std::pow(10.,value/20.));
  if(id==2)attack=static_cast<float>(std::exp(-1./(48.*value)));
  if(id==3)release=static_cast<float>(std::exp(-1./(48.*value)));
  if(id==4)hold=static_cast<uint32_t>(48.*value);
 }
 void sample(const float in[2],const float key[2],float out[2]){
  const float peak=std::max(std::fabs(key[0]),std::fabs(key[1]));
  // Fast detector attack, 30 ms decay; reference is NEVER mixed into output.
  envelope=std::max(peak,envelope*0.999305797f);
  if(envelope>=threshold)remaining=hold;else if(remaining) --remaining;
  const float target=(envelope>=threshold||remaining)?duckGain:1.f;
  const float coefficient=target<gain?attack:release;
  gain=target+(gain-target)*coefficient;
  if(std::fabs(gain-target)<1.e-7f)gain=target;
  if(envelope<1.e-20f)envelope=0;
  out[0]=in[0]*gain;out[1]=in[1]*gain;
 }
};
}
