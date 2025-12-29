#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    darkBackground = juce::Colour(0xff1e1e1e);
    darkerBackground = juce::Colour(0xff141414);
    accentColour = juce::Colour(0xff0e639c);
    textColour = juce::Colours::white;
    
    setColour(juce::ResizableWindow::backgroundColourId, darkBackground);
    setColour(juce::DocumentWindow::backgroundColourId, darkBackground);
    setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2d2d2d));
    setColour(juce::TextButton::textColourOffId, textColour);
    setColour(juce::TextButton::textColourOnId, textColour);
    setColour(juce::ListBox::backgroundColourId, darkerBackground);
    setColour(juce::ListBox::textColourId, textColour);
    setColour(juce::Label::textColourId, textColour);
    setColour(juce::TextEditor::backgroundColourId, darkerBackground);
    setColour(juce::TextEditor::textColourId, textColour);
    setColour(juce::TextEditor::outlineColourId, juce::Colour(0xff3d3d3d));
    setColour(juce::ComboBox::backgroundColourId, darkerBackground);
    setColour(juce::ComboBox::textColourId, textColour);
    setColour(juce::PopupMenu::backgroundColourId, darkerBackground);
    setColour(juce::PopupMenu::textColourId, textColour);
    setColour(juce::PopupMenu::highlightedBackgroundColourId, accentColour);
}

void CustomLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button, 
                                             const juce::Colour& backgroundColour,
                                             bool isMouseOverButton, bool isButtonDown)
{
    auto bounds = button.getLocalBounds().toFloat();
    
    auto baseColour = backgroundColour.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
                                     .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);
    
    if (isButtonDown || button.getToggleState())
        baseColour = baseColour.brighter(0.2f);
    else if (isMouseOverButton)
        baseColour = baseColour.brighter(0.1f);
    
    g.setColour(baseColour);
    g.fillRoundedRectangle(bounds, 3.0f);
    
    g.setColour(baseColour.brighter(0.1f));
    g.drawRoundedRectangle(bounds, 3.0f, 1.0f);
}

void CustomLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                         bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds();
    auto tickBounds = bounds.removeFromLeft(20).reduced(2);
    
    g.setColour(button.findColour(juce::ToggleButton::tickDisabledColourId));
    g.drawRoundedRectangle(tickBounds.toFloat(), 3.0f, 1.0f);
    
    if (button.getToggleState())
    {
        g.setColour(accentColour);
        g.fillRoundedRectangle(tickBounds.reduced(2).toFloat(), 2.0f);
    }
    
    g.setColour(button.findColour(juce::ToggleButton::textColourId));
    g.drawText(button.getButtonText(), bounds, juce::Justification::centredLeft);
    
    juce::ignoreUnused(shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
}

juce::Font CustomLookAndFeel::getTextButtonFont(juce::TextButton&, int buttonHeight)
{
    return juce::Font(juce::jmin(14.0f, buttonHeight * 0.6f));
}
