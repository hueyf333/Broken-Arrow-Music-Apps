#pragma once
#include <JuceHeader.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();
    
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                             bool isMouseOverButton, bool isButtonDown) override;
    
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                         bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
    juce::Font getTextButtonFont(juce::TextButton&, int buttonHeight) override;
    
private:
    juce::Colour darkBackground;
    juce::Colour darkerBackground;
    juce::Colour accentColour;
    juce::Colour textColour;
};
