/*
  ==============================================================================

    BiquadFilter.h
    Created: 30 Oct 2016 12:06:48am
    Author:  Lindley P Cannon Jr

  ==============================================================================
*/

#ifndef BIQUADFILTER_H_INCLUDED
#define BIQUADFILTER_H_INCLUDED

#include "JuceHeader.h"
#include "AudioFilter.h"


class BiquadFilter : public AudioFilter
{

public:
    BiquadFilter();
    
    virtual ~BiquadFilter();
    
    // Must be called before playback starts to set sample rate etc.
    void initializeFilter(float initSampRate, float initMinFrequency, float initMaxFrequency) override;
    
    void calcBiquad(void);
    
    //Overriding base class filter response functions for VA One Pole response
    float getMagnitudeResponse(float freq) const  override;
    float processFilter(float input, int channel) override;
    
protected:
    float z1 [2];
    double a0, a1, a2, b1, b2;
    double Fc, Q, peakGain;
    double z2;
    
    // Big G value for bilinear cutoff/pre-warping
    float G = 0.0;
};




#endif  // BIQUADFILTER_H_INCLUDED
