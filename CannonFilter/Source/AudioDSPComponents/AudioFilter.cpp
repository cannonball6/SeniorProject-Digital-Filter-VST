/*
  ==============================================================================

    AudioFilter.cpp
    Created: 25 Jul 2015 10:05:58am
    Author:  Joshua Marler

  ==============================================================================
*/


#include "AudioFilter.h"

AudioFilter::AudioFilter()
{
    //filterType = LowPass;
}

AudioFilter::~AudioFilter()
{

}

void AudioFilter::setCutoff(double newCutoffFrequency)
{
    cutoffFrequency = newCutoffFrequency;
}


double AudioFilter::getCutoff() const
{
    return cutoffFrequency;
}

void AudioFilter::setGain(double newGain)
{
    filterGain = newGain;
}

double AudioFilter::getGain() const
{
    return filterGain;
}

void AudioFilter::setQFactor(double newQFactor)
{
    qFactor = newQFactor;
}

double AudioFilter::getQFactor() const
{
    return qFactor;
}

void AudioFilter::setFilterType(int type)
{
    filterType = type;
}

int AudioFilter::getFilterType() const
{
    return filterType;
}

//Default filter response implementations
double AudioFilter::getMagnitudeResponse(double freq) const
{
    return 0.0;
}

double AudioFilter::getPhaseResponse(double freq) const
{
    return 0.0;
}

