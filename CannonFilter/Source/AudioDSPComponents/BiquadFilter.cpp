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
    this->qFactor = .7071;
    
    z1[0] = 0.0;
    z1[1] = 0.0;
    z2[0] = 0.0;
    z2[1] = 0.0;
    
    //calcBiquad();
}

void BiquadFilter::setCutoff(float newCutoff)
{
    this->cutoffFrequency = newCutoff/this->sampleRate;

    //calcBiquad();
    //Cutoff prewarping, billinear transform filters
    float wd = 2 * M_PI * this->cutoffFrequency;
    float T = 1/this->sampleRate;
    
    // Desired analogue frequency / these are virtual analogue filters so this is the cutoff / frequency response we require for our filter algorithm
    float wa = (2/T) * tan(wd*T/2); //analog frequency response function
    float g = wa * T/2; //output gain, cutoff
    G = g/(1.0 + g); //freq response
}

void BiquadFilter::Reset(){
    z1[0] = 0.0;
    z1[1] = 0.0;
    z2[0] = 0.0;
    z2[1] = 0.0;
    
    // forces filter coefficient calculation
    lastCutoff = -1.0;
    lastRes = -1.0;
}
float BiquadFilter::processFilter(float input, int channel)
{
//    float output = 0.0;
//    float v = (input - z1[channel]) * G;
//    float lowpassOutput = v + z1[channel];
    
    
//    double output = a0 *input + z1[channel];
//    z1[channel] = input * a1 + z2[channel] - b1 * output;
//    z2[channel] = input * a2 - b2 * output;
    
    //Update the z1 delay / state holder with new filter output for use in next sample
    //z1 [channel]= lowpassOutput + v;
    
    //float highpassOutput = input - lowpassOutput;
    
    // cutoff bound check and scale
    if(this->cutoffFrequency > 0.99) { this->cutoffFrequency = 0.99; }
    
    // parameter update check
    if(this->cutoffFrequency != lastCutoff){
        calcCoeff();
        lastCutoff = this->cutoffFrequency;
        //lastRes = res;
    }
    
    float output = a0 * input + a1 * z1[0] + a2 * z1[1] - b1 * z2[0] - b2 * z2[1];
    
    z1[1] = z1[0];
    z1[0] = input;
    z2[1] = z2[0];
    z2[0] = output;

    return (this->filterGain * output);
}



void BiquadFilter::calcCoeff(){
    double norm;
    //double V = pow(10, fabs(peakGain) / 20.0);
    double K = tan(M_PI * this->cutoffFrequency);
    ////
    double w0 = 2*M_PI*this->cutoffFrequency/this->sampleRate;
    //double cosW = cos(w0);
    //double sinW = sin(w0);
    //double alpha = sinW/(2*Q);
    
    /////
    
    double r = pow(10.0, 0.05 * -1);
    double k = 0.5 * r * sin(M_PI * this->cutoffFrequency);
    double c1 = 0.5 * (1.0 - k) / (1.0 + k);
    double c2 = (0.5 + c1) * cos(M_PI * this->cutoffFrequency);
    double c3 = (0.5 + c1 - c2) * 0.25;
    
    
    switch (this->filterType) {
        case AudioFilter::filterTypeList::LowPass:
            norm = 1 / (1 + K / Q + K * K);
            a0 = K * K * norm;
            a1 = 2 * a0;
            a2 = a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / Q + K * K) * norm;
            break;
            ////////////////
//            a0 = 2.0 * c3;
//            a1 = 4.0 * c3;
//            a2 = 2.0 * c3;
//            b1 = 2.0 * -c2;
//            b2 = 2.0 * c1;
//            break;
            
            ////////////////
//            b0 =  (1 - cos(w0))/2;
//            b1 =   1 - cos(w0);
//            b2 =  (1 - cos(w0))/2;
//            a0 =   1 + alpha;
//            a1 =  -2*cos(w0);
//            a2 =   1 - alpha;
//            break;
        case AudioFilter::filterTypeList::HighPass:
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

