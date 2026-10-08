#pragma once
#include <JuceHeader.h>
#include <cmath>
class H2Engine{
public:
 void prepare(double sr){sampleRate=sr;dc=0;lp=0;ironState=0;cathode6=0;sag6=0;dc6=0;follower12=0;}
 void reset(){dc=lp=ironState=0;}
 float process(float x,float drive,float h2,float asym,float bias,float warmth,float iron,bool pure,bool solo,bool sn7On,float sn7Drive,float sn7Bias){
   float in=x;
   if(sn7On){
     // 6SN7-inspired grounded-cathode voltage stage.
     // Drive is mapped to grid swing; positive grid excursions load the source.
     // Keep the 6SN7 in its broad, low-odd-order operating region. The previous
     // grid swing pushed the stage far enough to regenerate H3 before the H2 block.
     float grid=in*(1.0f+sn7Drive*4.5f);
     float biasV=-5.15f+(sn7Bias-.5f)*2.8f-cathode6*1.35f;
     float vgk=grid+biasV;
     float gridCurrent=softplus((vgk+0.35f)*5.0f)*0.018f;
     grid-=gridCurrent*(0.55f+sn7Drive*1.15f);
     vgk=grid+biasV;
     // Smooth Koren-inspired triode-current surrogate. B+ droops with recent current.
     float bplus=300.0f-sag6*72.0f;
     float mu=20.0f;
     float effective=softplus((vgk+bplus/mu)*0.42f);
     float plateCurrent=effective*effective*0.0028f;
     sag6+=0.00010f*(juce::jlimit(0.0f,1.0f,plateCurrent*12.0f)-sag6);
     cathode6+=0.00028f*(juce::jlimit(0.0f,1.0f,plateCurrent*7.5f)-cathode6);
     float plate=-(plateCurrent*82.0f);
     // Coupling capacitor / DC blocker preserves asymmetric AC curvature.
     dc6+=0.00018f*(plate-dc6);
     in=(plate-dc6)*0.34f;
   }
   // Keep the interstage clean: the dedicated H2 block creates the desired even harmonic.
   float d=juce::jmap(drive,0.0f,1.0f,1.0f,8.0f);
   // PURE H2: centered square-law term. DC servo removes x^2 DC while preserving 2f.
   float sq=in*in;
   // Fast DC servo: x^2 contains DC + 2f. Remove DC and retain the even-order AC term.
   dc+=0.0007f*(sq-dc);
   float even=sq-dc;
   // II control is intentionally aggressive near the top. At 11 it becomes an H2 generator,
   // rather than merely asking another saturator to clip harder.
   float h2Curve=h2*h2*(3.0f+9.0f*h2);
   float pureH2=in+even*h2Curve;
   // Tube-inspired asymmetric transfer. The II control crossfades away from the
   // odd-rich saturating carrier and toward a deliberately even-order generator.
   float z=in*d+(bias-.5f)*1.1f;float pos=1.0f+asym*3.5f,neg=1.0f+(.15f+1.0f-asym)*1.1f;
   float tube=z>=0?std::tanh(z*pos)/pos:std::tanh(z*neg)/neg;
   float tubeBlend=juce::jlimit(0.0f,1.0f,(1.0f-h2)*(1.0f-h2)*0.55f);
   float carrier=in+(tube-in)*tubeBlend;
   float generated=carrier+even*h2Curve;
   float y=pure?pureH2:generated;
   // 12BH7-inspired follower. Keep this stage essentially linear: hard symmetric
   // saturation here previously regenerated H3 after the H2 stage had done its job.
   follower12+=0.0011f*(y-follower12);
   y=y+follower12*.035f;
   // Warmth: stateful HF smoothing; Iron: flux-memory saturation.
   float a=juce::jmap(warmth,0.0f,1.0f,.82f,.22f);lp+=a*(y-lp);y=juce::jmap(warmth,y,lp);
   ironState+=0.0012f*(y-ironState);
   // Preserve transformer flux memory while progressively removing the symmetric
   // tanh saturation that regenerates H3. At high II, IRON becomes mostly memory/color.
   float ironInput=y+ironState*iron*.25f;
   float ironNonlinear=iron*(1.0f-h2)*(1.0f-h2);
   float id=1.0f+ironNonlinear*5.0f;
   float ironSat=std::tanh(ironInput*id)/std::tanh(id);
   y=juce::jmap(ironNonlinear,ironInput,ironSat);
   // Energy compensation keeps the maximum-H2 end from winning by loudness alone.
   float comp=1.0f/std::sqrt(1.0f+h2Curve*h2Curve*0.10f);
   y*=comp;
   if(solo)y-=in*comp;
   return y;
 }
private:double sampleRate=44100;float dc=0,lp=0,ironState=0,cathode6=0,sag6=0,dc6=0,follower12=0;
 static float softplus(float x){if(x>12.0f)return x;if(x<-12.0f)return std::exp(x);return std::log1p(std::exp(x));}
};
