#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
class EvenAudioProcessorEditor:public juce::AudioProcessorEditor{public:explicit EvenAudioProcessorEditor(EvenAudioProcessor&);void paint(juce::Graphics&)override;void resized()override;private:EvenAudioProcessor&p;juce::Slider h2,drive,asym,bias,warmth,iron,mix,output,sn7drive,sn7bias;juce::ToggleButton pure{"PURE H2"},solo{"H2 SOLO"},sn7{"6SN7 PREAMP"};using SA=juce::AudioProcessorValueTreeState::SliderAttachment;using BA=juce::AudioProcessorValueTreeState::ButtonAttachment;std::vector<std::unique_ptr<SA>>sa;std::unique_ptr<BA>pa,soa,sn7a;std::vector<juce::Slider*>knobs;JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EvenAudioProcessorEditor)};
