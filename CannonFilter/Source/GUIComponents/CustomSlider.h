
#include "../JuceLibraryCode/JuceHeader.h"

#ifndef CUSTOMSLIDER_H_INCLUDED
#define CUSTOMSLIDER_H_INCLUDED

class CustomSlider   : public Slider,
                       private Timer
{
public:
    CustomSlider(AudioProcessorParameter& p);
    
    void valueChanged() override;
    void timerCallback() override;
   
    void startedDragging() override;
    void stoppedDragging() override;
  
    double getValueFromText (const String& text) override;
    String getTextFromValue (double value) override;
    
    void updateSliderPos();

private:
    
    AudioProcessorParameter& param;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomSlider)
};



#endif  


/* void NDSLookAndFeel::drawRotarySlider (Graphics &g, int x, int y, int width, int height, float sliderPosProportional, const float rotaryStartAngle, const float rotaryEndAngle, Slider &slider)
 {
 AffineTransform rotator;
 if (!slider.isMouseOverOrDragging())
 {
 g.drawImage (knobNormal, x,y,width,height,0,0,width,height,false);
 }
 else
 {
 g.drawImage (knobOver, x,y,width,height,0,0,width,height,false);
 }
 g.drawImageTransformed (knobMask, x, y, width, height, rotator.rotated ((float)sliderPosProportional*rotaryEndAngle, (float)(knobMask->getWidth()/2), (float)(knobMask->getHeight()/2)), false);
 }-*/
