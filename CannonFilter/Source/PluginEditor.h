/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/
#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED


#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "GUIComponents/CustomLookAndFeel.h"
#include "GUIComponents/FilterResponseDisplay.h"
#include "GUIComponents/CustomSlider.h"


//==============================================================================
/**
*/
class CannonFilterAudioProcessorEditor  : public AudioProcessorEditor,
                                           public ComboBoxListener
{
public:

    CannonFilterAudioProcessorEditor (CannonFilterAudioProcessor&);
    ~CannonFilterAudioProcessorEditor();
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    AudioProcessorParameter* getParameterFromSlider(const Slider* slider) const;
    
    //==============================================================================
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CannonFilterAudioProcessor& processor;

    //GUI Components
    CustomLookAndFeel customLookAndFeel;
    FilterResponseDisplay filterResponseDisplay;
    std::unique_ptr<CustomSlider> frequencyCutoffSlider, filterGainSlider;
    ComboBox filterTypeDropDown;
    Label filterCutoffLabel, filterGainLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CannonFilterAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
