

#ifndef AUDIOFILTER_H_INCLUDED
#define AUDIOFILTER_H_INCLUDED

#include "JuceHeader.h"


//Abstract audio filter base class
class AudioFilter
{
public:
    
    AudioFilter();
    
    virtual ~AudioFilter();
    
    //Filters deriving from this class must implement their process method
    virtual double processFilter (double input, int channel) = 0;
    
    /* 
        Filters deriving from this class must implement their own initialization code - i.e for flushing storage registers/state holders etc.
        Must be called before playback starts to set sample rate etc. 
     */
    virtual void initializeFilter (double initSampleRate, double initMinFrequency, double initMaxFrequency) = 0;
    
    inline void setSampleRate (double newSampRate) {sampleRate = newSampRate;}
    inline double getSampleRate() const {return sampleRate;}
    
    inline void setMinFrequency (double newMinFrequency) {minFrequency = newMinFrequency;}
    inline double getMinFrequency() const {return minFrequency;}
    
    inline void setMaxFrequency (double newMaxFrequency) {maxFrequency = newMaxFrequency;}
    inline double getMaxFrequency() const {return maxFrequency;}
    
   
    /* 
        Parameter set and get methods made virtual to facilitate use in different types of filters - i.e virtual analogue filters where
        cutoff frequencies may need to be pre-warped etc.
     */
    virtual void setCutoff (double newCutoff);
    virtual double getCutoff() const;
    virtual void setQFactor (double newQFactor);
    virtual double getQFactor() const;
    virtual void setGain (double newGain);
    virtual double getGain() const;
    
    /* 
        Filter response functions that can be overriden in baseclass to return the filters magnitude and phase response.
        These can be used to provide frequency response for displays and GUI components etc. Default implementations just return 0.0.
        These functions do not HAVE to be implemented - the filter object will run fine without them.
    */
    virtual double getMagnitudeResponse(double freq) const;
    virtual double getPhaseResponse(double freq) const;
    
    void setFilterType (int newFilterType);
    int getFilterType() const;
    
    //Filter types / response shapes - lowpass, highpass etc.
    enum filterTypeList
    {
        LowPass = 0,
        HighPass,
        bqLowPass,
        bqHighPass,
        bqBandPass
    };
    
protected:
    
    double sampleRate = 0.0;
    double minFrequency = 0.0;
    double maxFrequency = 0.0;
    
    double cutoffFrequency = 0.0;
    double filterGain = 1.0;
    double qFactor = 0.1;
    
    int filterType = 0;
};

#endif  // AUDIOFILTER_H_INCLUDED
