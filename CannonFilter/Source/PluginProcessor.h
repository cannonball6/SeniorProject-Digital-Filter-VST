/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioDSPComponents/MyFilters.h"
#include "Parameters/CustomAudioParameter.h"
#include <string>

//==============================================================================
/**
*/
class CannonFilterAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    CannonFilterAudioProcessor();
    ~CannonFilterAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
    
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //Returns the processors AudioFilter instance for GUI code etc.
    AudioFilter& getAudioFilter();
    
    CustomAudioParameter* filterCutoffParam;
    CustomAudioParameter* filterGainParam;
    CustomAudioParameter* filterQParam;

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;
    
private:
    //==============================================================================
  
    
    std::unique_ptr<AudioFilter> filter1;
    //std::unique_ptr<AudioFilter> filter2;
    
    //CustomAudioParameter* filterCutoffParam;
    //CustomAudioParameter* filterGainParam;
    
    //Default setup values for filter etc.
    const double defaultSampleRate = 44100.00;
    const double defaultMinFilterFrequency = 20.00;
    const double defaultMaxFilterFrequency = 20000.00;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CannonFilterAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
