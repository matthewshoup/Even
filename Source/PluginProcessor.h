#pragma once
#include <JuceHeader.h>
#include "H2Engine.h"
class EvenAudioProcessor:public juce::AudioProcessor{
public:EvenAudioProcessor();void prepareToPlay(double,int)override;void releaseResources()override{}bool isBusesLayoutSupported(const BusesLayout&)const override;void processBlock(juce::AudioBuffer<float>&,juce::MidiBuffer&)override;juce::AudioProcessorEditor*createEditor()override;bool hasEditor()const override{return true;}const juce::String getName()const override{return "EVEN";}bool acceptsMidi()const override{return false;}bool producesMidi()const override{return false;}bool isMidiEffect()const override{return false;}double getTailLengthSeconds()const override{return 0;}int getNumPrograms()override{return 1;}int getCurrentProgram()override{return 0;}void setCurrentProgram(int)override{}const juce::String getProgramName(int)override{return{};}void changeProgramName(int,const juce::String&)override{}void getStateInformation(juce::MemoryBlock&)override;void setStateInformation(const void*,int)override;juce::AudioProcessorValueTreeState apvts;static juce::AudioProcessorValueTreeState::ParameterLayout layout();
private:H2Engine eng[2];juce::dsp::Oversampling<float> oversampling{2,3,juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR,true,false};JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EvenAudioProcessor)
};
