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
    void initializeFilter(float initSampRate, float initMinFrequency, float initMaxFrequency) override;
    
    void setCutoff(float cutoff) override;
    void setQFactor(float newQ)override;
    //Overriding base class filter response functions for VA One Pole response
    
    float designLowPass(float input, int channel);
    float designHighPass(float input, int channel);
    float designBiquad(float input);
    float designBiquadHighPass();
    float designBiquadBandPass(float input);
    
    void calcCoeff(void);
    float getMagnitudeResponse(float freq) const  override;
    float processFilter(float input, int channel) override;
    //reset state holders
    void Reset();
    
private:
    // z1 delay element/state holder (performing analgue integration equivalent)
    // set as a stereo option for now
    // stereo input between filter and plugin processor code but this is nice and simple.
    float z1 [2];
    float z2 [2];
    //coefficient variable
    float a0,a1,a2,b0,b1,b2;
    // Big G value for bilinear cutoff/pre-warping
    float G = 0.0;
    float Q = .1;
    // remember last parameters so we know when to recalculate them
    float lastCutoff;
    float biquadCutoff;
};



#endif  // MYFILTERS_H_INCLUDED
