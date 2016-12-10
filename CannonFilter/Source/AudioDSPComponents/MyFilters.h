/*
  ==============================================================================

    MyFilters.h
    Created: 16 Nov 2016 5:36:41pm
    Author:  Lindley P Cannon Jr

  ==============================================================================
*/

#ifndef MYFILTERS_H_INCLUDED
#define MYFILTERS_H_INCLUDED

#include "JuceHeader.h"
#include "AudioFilter.h"


/*
 A very basic Virtual Analogue One Pole Filter based on calculations from The Art of VA Filter Design
 by Vadim Zavalishin see:
 
 NOTE : Need to implement oversampling as frequency response is incorrect at high frequencies approaching nyquist
 there is lost energy/gain which will be apparent on the filters response display (common issue with VA filters). You
 will notice the frequency response suddenly drops off as the cutoff approaches nyquist.
 This will be addressed in the next version.
 */

class MyFilters : public AudioFilter
{
    
public:
    MyFilters();
    
    virtual ~MyFilters();
    
    // Must be called before playback starts to set sample rate etc.
    void initializeFilter(double initSampRate, double initMinFrequency, double initMaxFrequency) override;
    
    void setCutoff(double cutoff) override;
    void setQFactor(double newQ)override;
    //Overriding base class filter response functions for VA One Pole response
    
    double designLowPass(double input, int channel);
    double designHighPass(double input, int channel);
    double designBiquad(double input);
    double designBiquadHighPass();
    double designBiquadBandPass(double input);
    
    void calcCoeff(void);
    double getMagnitudeResponse(double freq) const  override;
    double processFilter(double input, int channel) override;
    //reset state holders
    void Reset();
    
private:
    // z1 delay element/state holder (performing analgue integration equivalent)
    // set as a stereo option for now
    // stereo input between filter and plugin processor code but this is nice and simple.
    double z1 [2];
    double z2 [2];
    
    double s1, s2, s3 = 0.0;
    
    //coefficient variable
    double a0,a1,a2,b0,b1,b2;
    // Big G value for bilinear cutoff/pre-warping
    double G = 0.0;
    double Q = .707;
    // remember last parameters so we know when to recalculate them
    double lastCutoff = 0.0;
    double biquadCutoff;
    
    JUCE_LEAK_DETECTOR (MyFilters);
};



#endif  // MYFILTERS_H_INCLUDED
