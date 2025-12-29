#pragma once
#include <JuceHeader.h>

class StatusBar : public juce::Component
{
public:
    StatusBar();
    ~StatusBar() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void setTrackInfo(const juce::String& trackName);
    void setPositionInfo(int measure, int beat);
    void setDurationInfo(const juce::String& duration);
    
private:
    juce::Label trackLabel;
    juce::Label positionLabel;
    juce::Label durationLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StatusBar)
};
