/*
  ==============================================================================

    BiquadFilter.cpp
    Created: 30 Oct 2016 12:06:48am
    Author:  Lindley P Cannon Jr

  ==============================================================================
*/

#include "BiquadFilter.h"

BiquadFilter::BiquadFilter()
{
    filterType = bqLowPass;
    a0 = 1.0;
    a1 = a2 = b1 = b2 = 0.0;
    cutoffFrequency = 0.50;
    qFactor = 0.707;
    peakGain = 0.0;
    z2 = 0.0;
}

BiquadFilter::~BiquadFilter()
{

}


//Set variables and flush state registers in initialization function. Must be called before playback starts
void BiquadFilter::initializeFilter(float initSampleRate, float initMinFrequency, float initMaxFrequency)
{
    this->sampleRate = initSampleRate;
    this->minFrequency = initMinFrequency;
    this->maxFrequency = initMaxFrequency;
    
    //clear the delay elements/state holders
    z1[0] = 0.0;
    z1[1] = 0.0;
}

void BiquadFilter::calcBiquad(void)
{
    double norm;
    //double V = pow(10, fabs(peakGain) / 20.0);
    double K = tan(M_PI * Fc);
    switch (this->filterType) {
        case bqLowPass:
            norm = 1 / (1 + K / Q + K * K);
            a0 = K * K * norm;
            a1 = 2 * a0;
            a2 = a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;
            
        case bqHighPass:
            norm = 1 / (1 + K / Q + K * K);
            a0 = 1 * norm;
            a1 = -2 * a0;
            a2 = a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;
    }
    return;
}

float BiquadFilter::processFilter(float input, int channel)
{
    calcBiquad();
    float output = input * a0 + z1[channel];
    //float v = (input - z1[channel]) * G;

    //Update the z1 delay / state holder with new filter output for use in next sample
    z1 [channel] = input * a1 + z2 - b1 * output;
    z2 = input * a2 - b2 * output;
    
    //float highpassOutput = input - lowpassOutput;
    
    switch (this->filterType) {
        case AudioFilter::filterTypeList::bqLowPass:
            output = this->filterGain * output;
        break;
        case AudioFilter::filterTypeList::bqHighPass:
            output = this->filterGain * output;
            break;
        default:
            break;
    }

    return output;
}

float BiquadFilter::getMagnitudeResponse(float frequency) const
{
    float magnitude = 0.0;
    float T = 1/this->sampleRate;
    
    float wdCutoff = 2 * M_PI * this->cutoffFrequency;
    
    //Calculating pre-warped/analogue cutoff frequency to use in virtual analogue frequeny response calculations
    float cutOff = (2/T) * tan(wdCutoff*T/2);
    
    //Digital frequency to evaluate
    float wdEval =  2 * M_PI * frequency;
    float sValue =  (2/T) * tan(wdEval*T/2);
    
    
    /* This is the digital transfer function which is equal to the analogue transfer function
     evaluated at H(s) where s = (2/T) * tan(wd*T/2) hence why the cutoff used is the pre warped analogue equivalent.
     See Art Of VA Filter Design 3.8 Bilinear Transform Section */
    switch (this->filterType) {
        case AudioFilter::filterTypeList::LowPass:
            //VA Lowpass Frequency response wc/s+wc
            magnitude = cutOff/(sValue + cutOff);
            break;
        case AudioFilter::filterTypeList::HighPass:
            //VA Highpass Frequency response s/s+wc
            magnitude = sValue/(sValue + cutOff);
            break;
        default:
            break;
    }
    
    magnitude = magnitude * this->filterGain;
    
    //Convert to db for log db response display
    magnitude = Decibels::gainToDecibels(magnitude);
    return  magnitude;
}






