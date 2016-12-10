/*
  ==============================================================================

    MyFilters.cpp
    Created: 16 Nov 2016 5:36:41pm
    Author:  Lindley P Cannon Jr

  ==============================================================================
*/
#if JUCE_INTEL
#define JUCE_SNAP_TO_ZERO(n)    if (! (n < -1.0e-8f || n > 1.0e-8f)) n = 0;
#else
#define JUCE_SNAP_TO_ZERO(n)
#endif

#include "MyFilters.h"

MyFilters::MyFilters()
{
}

MyFilters::~MyFilters()
{
}

//Set variables and flush state registers in initialization function. Must be called before playback starts
void MyFilters::initializeFilter(double initSampleRate, double initMinFrequency, double initMaxFrequency)
{
    this->sampleRate = initSampleRate;
    this->minFrequency = initMinFrequency;
    this->maxFrequency = initMaxFrequency;
    //clear the delay elements/state holders
    z1[0] = 0.0;
    z1[1] = 0.0;
    z2[0] = 0.0;
    z2[1] = 0.0;
}


void MyFilters::setCutoff(double newCutoff)
{
    this->cutoffFrequency = newCutoff;
    biquadCutoff = newCutoff / this->sampleRate;
    
    //Cutoff prewarping, billinear transform filters
    double wd = 2 * M_PI * this->cutoffFrequency;
    double T = 1/this->sampleRate;
    
    // Desired analogue frequency / these are virtual analogue filters so this is the cutoff / frequency response we require for our filter algorithm
    double wa = (2/T) * tan(wd*T/2); //analog frequency response function
    double g = wa * T/2; //output gain, cutoff
    G = g/(1.0 + g); //freq response
}

void MyFilters::setQFactor(double newQ)
{
    this->qFactor = newQ;
}

void MyFilters::calcCoeff(){
    double norm;
   // double V = pow(10, fabs(this->filterGain) / 20.0);
    double K = tan(M_PI * biquadCutoff);

    switch (this->filterType) {
        case AudioFilter::filterTypeList::bqLowPass:
            norm = 1 / (1 + K / getQFactor() + K * K);
            a0 = K * K * norm;
            a1 = 2 * a0;
            a2 = a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / getQFactor()+ K * K) * norm;
            break;
        case AudioFilter::filterTypeList::bqHighPass:
            norm = 1 / (1 + K / getQFactor() + K * K);
            a0 = 1 * norm;
            a1 = -2 * a0;
            a2 = a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / this->qFactor + K * K) * norm;
            break;
        case AudioFilter::filterTypeList::bqBandPass:
            norm = 1 / (1 + K / this->qFactor + K * K);
            a0 = K / this->qFactor * norm;
            a1 = 0;
            a2 = -a0;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - K / this->qFactor + K * K) * norm;
            break;
        default:
            break;
    }
    return;
}

double MyFilters::designLowPass(double input, int channel)
{
    double v = (input - z1[channel]) * G;
    double lowpassOutput = v + z1[channel];
    
    z1[channel] = lowpassOutput + v;
    
    return lowpassOutput;
}
double MyFilters::designHighPass(double input, int channel)
{
    double v = (input - z1[channel]) * G;
    double lowpassOutput = v + z1[channel];
    
    z1[channel] = lowpassOutput + v;
    
    double highpassOutput = input - lowpassOutput;
    
    return highpassOutput;
}

double MyFilters::designBiquad(double input){
    double cutoff = AudioFilter::getCutoff();
    double nyq = 2.0 / AudioFilter::getSampleRate();
    
    //double cutoff = this->cutoffFrequency / this->sampleRate;
     cutoff = (cutoff < this->minFrequency) ? this->minFrequency*nyq : cutoff*nyq;
    if(cutoff> 0.99) { cutoff = 0.99; }
    // parameter update check
    if(cutoff != lastCutoff){
        calcCoeff();
        b0 = b0*this->filterGain;
        b1 = b1*this->filterGain;
        b2 = b2*this->filterGain;
        lastCutoff = cutoff;
    }

//third way
    
//second way
//    double out = 0.0;
//    //Update States First
//    s3 = s2;
//    s2 = s1;
//    if ((a1 && a2 <= 2) && (b0 && b1 && b2 <= 2))
//    {
//        s1 = (input/this->filterGain) - a1*s2 - a2*s3;  //compute feedback
//    
//        out = this->filterGain*(b0*s1 + b1*s2 + b2*s3); //compute feedforward
//    }
//original way
//    double outputSignal = a0 * input + a1 * z1[0] + a2 * z1[1] - b1 * z2[0] - b2 * z2[1];
//    
//    z1[1] = z1[0];
//    z1[0] = input;
//    z2[1] = z2[0];
//    z2[0] = outputSignal; 
    
    return out;
}

double MyFilters::processFilter(double input, int channel)
{
    double output = 0.0;

    switch (this->filterType) {
        case AudioFilter::filterTypeList::LowPass:
            output = this->filterGain * designLowPass(input, channel);
            break;
        case AudioFilter::filterTypeList::HighPass:
            output = this->filterGain * designHighPass(input, channel);
            break;
        case AudioFilter::filterTypeList::bqLowPass:
            output = designBiquad(input);
            break;
        case AudioFilter::filterTypeList::bqHighPass:
            output = this->filterGain * designBiquad(input);
            break;
        case AudioFilter::filterTypeList::bqBandPass:
            output = this->filterGain * designBiquad(input);
            break;
        default:
            break;
    }
    return output;
}

double MyFilters::getMagnitudeResponse(double frequency) const
{
    double magnitude = 0.0;
    double T = 1/this->sampleRate;
    
    double wdCutoff = 2 * M_PI * this->cutoffFrequency;
    
    //Calculating pre-warped/analogue cutoff frequency to use in virtual analogue frequeny response calculations
    double cutOff = (2/T) * tan(wdCutoff*T/2);
    
    //Digital frequency to evaluate
    double wdEval =  2 * M_PI * frequency;
    double sValue =  (2/T) * tan(wdEval*T/2);
    
    
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
            //2-pole lowpass frequency response wc^2/s^2 + 2Rwc*s + wc^2
        case AudioFilter::filterTypeList::bqLowPass:
            magnitude = (cutOff*cutOff)/( (sValue*sValue) + (2*this->qFactor*cutOff*sValue) + (cutOff*cutOff) );
            break;
            //2-pole lowpass frequency response s^2 /s^2 + 2Rwc*s + wc^2
        case AudioFilter::filterTypeList::bqHighPass:
            magnitude = (sValue*sValue)/( (sValue*sValue) + (2*this->qFactor*cutOff*sValue) + (cutOff*cutOff) );
            break;
        case AudioFilter::filterTypeList::bqBandPass:
            //  s2 +2(1 k/2)+1
            magnitude = sValue*(sValue+1) / ( (sValue*sValue) + 2 * (1 - cutOff / 2)*sValue +1);
            break;
        default:
            break;
    }
    
    magnitude = magnitude * this->filterGain;
    
    //Convert to db for log db response display
    magnitude = Decibels::gainToDecibels(magnitude);
    return  magnitude;
}


#undef JUCE_SNAP_TO_ZERO
