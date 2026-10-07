#pragma once
#include <JuceHeader.h>
#include <cmath>
class H2Engine{
public:
 void prepare(double sr){sampleRate=sr;dc=0;lp=0;ironState=0;}
 void reset(){dc=lp=ironState=0;}
 float process(float x,float drive,float h2,float asym,float bias,float warmth,float iron,bool pure,bool solo){
   float in=x;float d=juce::jmap(drive,0.0f,1.0f,1.0f,14.0f);
   // PURE H2: centered square-law term. DC servo removes x^2 DC while preserving 2f.
   float sq=in*in;dc+=0.0007f*(sq-dc);float even=sq-dc;
   float pureH2=in+even*(h2*2.8f);
   // Tube-inspired asymmetric transfer. Bias and unequal positive/negative curvature favor H2.
   float z=in*d+(bias-.5f)*1.1f;float pos=1.0f+asym*3.5f,neg=1.0f+(.15f+1.0f-asym)*1.1f;
   float tube=z>=0?std::tanh(z*pos)/pos:std::tanh(z*neg)/neg;
   float generated=tube+even*h2*1.4f;
   float y=pure?pureH2:generated;
   // Warmth: stateful HF smoothing; Iron: flux-memory saturation.
   float a=juce::jmap(warmth,0.0f,1.0f,.82f,.22f);lp+=a*(y-lp);y=juce::jmap(warmth,y,lp);
   ironState+=0.0012f*(y-ironState);float id=1.0f+iron*5.0f;y=std::tanh((y+ironState*iron*.25f)*id)/std::tanh(id);
   if(solo)y-=in;return y;
 }
private:double sampleRate=44100;float dc=0,lp=0,ironState=0;
};
