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
    
    void Reset();
    void setFilterType (int newFilterType) ;
    void setCutoff(float cutoff) override;
    void setQFactor();
    
    //Overriding base class filter response
    float getMagnitudeResponse(float freq) const  override;
    float processFilter(float input, int channel) override;
    
    
    
protected:
    // remember last parameters so we know when to recalculate them
    double lastCutoff;
    double lastRes;
    
    void calcCoeff(void);
    
    float z1 [2];
    float z2 [2];
    double a0, a1, a2, b0, b1, b2;
    float Q = .7071;
    // Big G value for bilinear cutoff/pre-warping
    float G = 0.0;
};


#endif  // BIQUADFILTER_H_INCLUDED
