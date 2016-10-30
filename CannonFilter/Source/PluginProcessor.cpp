/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CannonFilterAudioProcessor::CannonFilterAudioProcessor() : filter1(new BiquadFilter())
                                                          
{
    /* The lambda is capturing a value copy of the this pointer to the audio processor. The processor will be destroyed after the parameter object so
     this is safe.*/
    auto cutoffParamCallback = [this] (float newCutoff){this->filter1->setCutoff(newCutoff);};
    filterCutoffParam = new CustomAudioParameter("FilterCutoff", "FilterCutoff", false, cutoffParamCallback);
    //This param represents a filters cutoff frequency so appending Hz string to label for display purposes.
    filterCutoffParam->setLabel("Hz");
    
    //auto qFactorParamCallback = [this] (int newQFactor){this->filter1->setQFactor(newQFactor);};
    //filterQParam = new CustomAudioParameter("Q", "Q", true, qFactorParamCallback);
    
    /*
        The filters min and max frequency will be used as normalized range values and values in this range will be passed to the set value
        callback function.
     */
    filterCutoffParam->setNormalisableRange(defaultMinFilterFrequency, defaultMaxFilterFrequency);
   
    auto gainParamCallback = [this] (float gain) {this->filter1->setGain(gain);};
    filterGainParam = new CustomAudioParameter("FilterGain", "FilterGain", true, gainParamCallback);
    
    addParameter(filterCutoffParam);
    addParameter(filterGainParam);
    //addParameter(filterQParam);
}

CannonFilterAudioProcessor::~CannonFilterAudioProcessor()
{
}

//==============================================================================
const String CannonFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CannonFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CannonFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CannonFilterAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double CannonFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CannonFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CannonFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CannonFilterAudioProcessor::setCurrentProgram (int index)
{
}

const String CannonFilterAudioProcessor::getProgramName (int index)
{
    return String();
}

void CannonFilterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void CannonFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    //Initialize our audio filter for playback.
    filter1->initializeFilter(sampleRate, defaultMinFilterFrequency, defaultMaxFilterFrequency);
}

void CannonFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void CannonFilterAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    float numSamples = buffer.getNumSamples();
    float currentSampleRate = getSampleRate();
    
    //Handles filter being added onto an already playing audio track where some hosts will not call prepare to play method.
    if (filter1->getSampleRate() != currentSampleRate)
    {
        filter1->initializeFilter(currentSampleRate, defaultMinFilterFrequency, defaultMaxFilterFrequency);
    }
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // I've added this to avoid people getting screaming feedback
    // when they first compile the plugin, but obviously you don't need to
    // this code if your algorithm already fills all the output channels.
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // MAIN AUDIO PROCESSING BLOCK. PROCESS FILTER TWICE FOR STEREO CHANNELS
    for (int channel = 0; channel < getTotalNumInputChannels(); ++channel)
    {
        const float* input = buffer.getReadPointer(channel);
        float* output = buffer.getWritePointer (channel);
        
        for (int i = 0; i < numSamples; i++)
        {
            output[i] = filter1->processFilter(input[i], channel);
        }
    }
}

//==============================================================================
AudioFilter& CannonFilterAudioProcessor::getAudioFilter()
{
    return *filter1;
}

//==============================================================================
bool CannonFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CannonFilterAudioProcessor::createEditor()
{
    return new CannonFilterAudioProcessorEditor (*this);
}

//==============================================================================
void CannonFilterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    // Create an outer XML element..
    XmlElement xml ("MYPLUGINSETTINGS");
    
    // Store the values of all our parameters, using their param ID as the XML attribute
    for (int i = 0; i < getNumParameters(); ++i)
        if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
            xml.setAttribute (p->paramID, p->getValue());
    
    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml, destData);
}

void CannonFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState != nullptr)
    {
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // Now reload our parameters..
            for (int i = 0; i < getNumParameters(); ++i)
                if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
                    p->setValueNotifyingHost ((float) xmlState->getDoubleAttribute (p->paramID, p->getValue()));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CannonFilterAudioProcessor();
}
