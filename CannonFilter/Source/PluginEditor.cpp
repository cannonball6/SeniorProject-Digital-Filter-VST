/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================

CannonFilterAudioProcessorEditor::CannonFilterAudioProcessorEditor (CannonFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), customLookAndFeel(),
      filterResponseDisplay(processor.getAudioFilter()), frequencyCutoffSlider(new CustomSlider (*processor.filterCutoffParam)),filterGainSlider(new CustomSlider (*processor.filterGainParam))
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 700);
    
    CannonFilterAudioProcessorEditor::setLookAndFeel(&customLookAndFeel);
    
    //Setup Components
    
    //Freq Cutoff Slider
    addAndMakeVisible(*frequencyCutoffSlider);
    frequencyCutoffSlider->setName("FilterCutoff");
    frequencyCutoffSlider->setSliderStyle(Slider::Rotary);
    frequencyCutoffSlider->setRange(0.0, 1.0);
   
    
    //Logarithmic frequency - 0.5 skew factor effectivley makes this a volt-octave knob.
    frequencyCutoffSlider->setSkewFactor(0.5);
    frequencyCutoffSlider->setColour(Slider::rotarySliderOutlineColourId, Colours::lightblue);
    frequencyCutoffSlider->setColour(Slider::rotarySliderFillColourId, Colours::greenyellow);
    
    //Gain Slider
    addAndMakeVisible(*filterGainSlider);
    filterGainSlider->setName("FilterGain");
    filterGainSlider->setSliderStyle(Slider::Rotary);
    filterGainSlider->setRange(0.0, 1.0, 0.05);
    filterGainSlider->setColour(Slider::rotarySliderOutlineColourId, Colours::lightblue);
    filterGainSlider->setColour(Slider::rotarySliderFillColourId, Colours::greenyellow);
    
    //Q Slider
    //addAndMakeVisible(*filterQSlider);
    //filterQSlider->setName("Q");
    //filterQSlider->setSliderStyle(Slider::Rotary);
    //filterQSlider->setRange(0.0, 3.0, 1.0);
    //filterQSlider->setColour(Slider::rotarySliderOutlineColourId, Colours::lightblue);
    //filterQSlider->setColour(Slider::rotarySliderFillColourId, Colours::greenyellow);
    
    //Cutoff Label
    addAndMakeVisible(filterCutoffLabel);
    filterCutoffLabel.setText("Frequency Cutoff", juce::NotificationType::dontSendNotification);
    filterCutoffLabel.setFont(Font ("Ayuthaya", 24.50f, Font::plain));
    filterCutoffLabel.setColour(Label::textColourId, Colours::lightblue);
    
    //Gain Label
    addAndMakeVisible(filterGainLabel);
    filterGainLabel.setText("Output Gain", juce::NotificationType::dontSendNotification);
    filterGainLabel.setFont(Font ("Ayuthaya", 22.50f, Font::plain));
    filterGainLabel.setColour(Label::textColourId, Colours::lightblue);
    
    //Q Label
    //addAndMakeVisible(filterQLabel);
    //filterQLabel.setText("Q", juce::NotificationType::dontSendNotification);
    //filterQLabel.setFont(Font ("Ayuthaya", 22.50f, Font::plain));
    //filterQLabel.setColour(Label::textColourId, Colours::lightblue);
    
    
    
    //Filter Selector Dropdown
    addAndMakeVisible(filterTypeDropDown);
    filterTypeDropDown.addItem("LowPass", 1);
    filterTypeDropDown.addItem("HighPass", 2);
    //filterTypeDropDown.addItem("BandPass", 3);
    //filterTypeDropDown.addItem("BandStop", 4);
    filterTypeDropDown.addItem("Select Filter", 3);
    filterTypeDropDown.setColour(ComboBox::outlineColourId, Colours::gold);
    filterTypeDropDown.setColour(ComboBox::backgroundColourId, Colours::dimgrey);
    filterTypeDropDown.setColour(ComboBox::textColourId, Colours::gold);
    filterTypeDropDown.setColour(ComboBox::buttonColourId, Colours::goldenrod);
    filterTypeDropDown.setSelectedId(3);
    filterTypeDropDown.addListener(this);
    
    addAndMakeVisible(filterResponseDisplay);
    filterResponseDisplay.setMagResponseColour(Colours::lightblue);
    filterResponseDisplay.setDisplayBackgroundColour(Colours::darkgrey);
    filterResponseDisplay.setBounds(50, 125, 500, 200);
    
    //Add the filter display as a listener to the cutoff and gain sliders for updates.
    frequencyCutoffSlider->addListener(&filterResponseDisplay);
    filterGainSlider->addListener(&filterResponseDisplay);
    //filterQSlider->addListener(&filterResponseDisplay);
}

CannonFilterAudioProcessorEditor::~CannonFilterAudioProcessorEditor()
{
}


//==============================================================================
void CannonFilterAudioProcessorEditor::comboBoxChanged(ComboBox* comboBoxThatChanged)
{
    //In here set filter type and also update filter response display's filter type
    if (comboBoxThatChanged == &filterTypeDropDown)
    {
        int filterTypeIndex = filterTypeDropDown.getSelectedItemIndex();
        processor.getAudioFilter().setFilterType(filterTypeIndex);
        
        //Repaint response display for filter type change
        filterResponseDisplay.repaint();
    }
}

//==============================================================================
void CannonFilterAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::darkslategrey);
}

void CannonFilterAudioProcessorEditor::resized()
{
    filterResponseDisplay.setBounds(50, 125, 500, 200);
    filterTypeDropDown.setBounds(225, 50, 150, 30);
    
    filterCutoffLabel.setBounds(75, 358, 130, 20);
    frequencyCutoffSlider->setBounds(50, 386, 135, 105);
    frequencyCutoffSlider->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 75, 20);
    
    filterGainLabel.setBounds(435, 358, 130, 20);
    filterGainSlider->setBounds(400, 386, 135, 105);
    filterGainSlider->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 75, 20);
    
    //filterQLabel.setBounds(254, 358, 130, 20);
    //filterQSlider->setBounds(225, 386, 135, 105);
    //frequencyQSlider->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 75, 20);
}
