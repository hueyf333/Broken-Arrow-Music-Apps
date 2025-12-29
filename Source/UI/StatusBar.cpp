#include "StatusBar.h"

StatusBar::StatusBar()
{
    auto setupLabel = [this](juce::Label& label) {
        addAndMakeVisible(label);
        label.setJustificationType(juce::Justification::centredLeft);
        label.setColour(juce::Label::textColourId, juce::Colours::white);
    };
    
    setupLabel(trackLabel);
    setupLabel(positionLabel);
    setupLabel(durationLabel);
    
    trackLabel.setText("Track: -", juce::dontSendNotification);
    positionLabel.setText("Position: Measure 1, Beat 1", juce::dontSendNotification);
    durationLabel.setText("Duration: Quarter", juce::dontSendNotification);
}

StatusBar::~StatusBar()
{
}

void StatusBar::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff2d2d2d));
}

void StatusBar::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    
    trackLabel.setBounds(bounds.removeFromLeft(200));
    positionLabel.setBounds(bounds.removeFromLeft(200));
    durationLabel.setBounds(bounds);
}

void StatusBar::setTrackInfo(const juce::String& trackName)
{
    trackLabel.setText("Track: " + trackName, juce::dontSendNotification);
}

void StatusBar::setPositionInfo(int measure, int beat)
{
    positionLabel.setText("Position: Measure " + juce::String(measure + 1) + ", Beat " + juce::String(beat + 1),
                         juce::dontSendNotification);
}

void StatusBar::setDurationInfo(const juce::String& duration)
{
    durationLabel.setText("Duration: " + duration, juce::dontSendNotification);
}
