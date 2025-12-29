#pragma once
#include <JuceHeader.h>
#include "../Model/Score.h"

class InspectorPanel : public juce::Component
{
public:
    InspectorPanel(Score& score);
    ~InspectorPanel() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void setSelection(int trackIndex, int measureIndex, int noteIndex);
    
private:
    void updateControls();
    
    Score& score;
    
    int currentTrack = -1;
    int currentMeasure = -1;
    int currentNote = -1;
    
    juce::Label pitchLabel;
    juce::Label pitchValue;
    juce::Label durationLabel;
    juce::Label durationValue;
    juce::Label velocityLabel;
    juce::Label velocityValue;
    juce::Label stringLabel;
    juce::Label stringValue;
    juce::Label fretLabel;
    juce::Label fretValue;
    juce::Label tiedLabel;
    juce::Label tiedValue;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InspectorPanel)
};
