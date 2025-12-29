#include "InspectorPanel.h"

InspectorPanel::InspectorPanel(Score& s)
    : score(s)
{
    auto setupLabel = [this](juce::Label& label, const juce::String& text, bool isValue = false) {
        addAndMakeVisible(label);
        label.setText(text, juce::dontSendNotification);
        label.setJustificationType(isValue ? juce::Justification::centredRight : juce::Justification::centredLeft);
        label.setColour(juce::Label::textColourId, isValue ? juce::Colour(0xffaaaaaa) : juce::Colours::white);
    };
    
    setupLabel(pitchLabel, "Pitch:");
    setupLabel(pitchValue, "-", true);
    setupLabel(durationLabel, "Duration:");
    setupLabel(durationValue, "-", true);
    setupLabel(velocityLabel, "Velocity:");
    setupLabel(velocityValue, "-", true);
    setupLabel(stringLabel, "String:");
    setupLabel(stringValue, "-", true);
    setupLabel(fretLabel, "Fret:");
    setupLabel(fretValue, "-", true);
    setupLabel(tiedLabel, "Tied:");
    setupLabel(tiedValue, "-", true);
}

InspectorPanel::~InspectorPanel()
{
}

void InspectorPanel::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e1e));
    
    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("Inspector", 10, 5, getWidth() - 20, 20, juce::Justification::centredLeft);
}

void InspectorPanel::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(30);
    
    auto row = [&bounds]() {
        return bounds.removeFromTop(25);
    };
    
    auto labelRow = row();
    pitchLabel.setBounds(labelRow.removeFromLeft(100));
    pitchValue.setBounds(labelRow);
    
    labelRow = row();
    durationLabel.setBounds(labelRow.removeFromLeft(100));
    durationValue.setBounds(labelRow);
    
    labelRow = row();
    velocityLabel.setBounds(labelRow.removeFromLeft(100));
    velocityValue.setBounds(labelRow);
    
    labelRow = row();
    stringLabel.setBounds(labelRow.removeFromLeft(100));
    stringValue.setBounds(labelRow);
    
    labelRow = row();
    fretLabel.setBounds(labelRow.removeFromLeft(100));
    fretValue.setBounds(labelRow);
    
    labelRow = row();
    tiedLabel.setBounds(labelRow.removeFromLeft(100));
    tiedValue.setBounds(labelRow);
}

void InspectorPanel::setSelection(int trackIndex, int measureIndex, int noteIndex)
{
    currentTrack = trackIndex;
    currentMeasure = measureIndex;
    currentNote = noteIndex;
    
    updateControls();
}

void InspectorPanel::updateControls()
{
    if (currentTrack < 0 || currentMeasure < 0 || currentNote < 0)
    {
        pitchValue.setText("-", juce::dontSendNotification);
        durationValue.setText("-", juce::dontSendNotification);
        velocityValue.setText("-", juce::dontSendNotification);
        stringValue.setText("-", juce::dontSendNotification);
        fretValue.setText("-", juce::dontSendNotification);
        tiedValue.setText("-", juce::dontSendNotification);
        return;
    }
    
    Track* track = score.getTrackAt(currentTrack);
    if (!track)
        return;
    
    Measure* measure = track->getMeasureAt(currentMeasure);
    if (!measure)
        return;
    
    Note* note = measure->getNoteAt(currentNote);
    if (!note)
        return;
    
    // Update labels with note information
    if (note->isRest())
    {
        pitchValue.setText("Rest", juce::dontSendNotification);
    }
    else
    {
        pitchValue.setText(juce::MidiMessage::getMidiNoteName(note->getPitch(), true, true, 4), 
                          juce::dontSendNotification);
    }
    
    juce::String durText;
    switch (note->getDuration())
    {
        case NoteDuration::Whole:     durText = "Whole"; break;
        case NoteDuration::Half:      durText = "Half"; break;
        case NoteDuration::Quarter:   durText = "Quarter"; break;
        case NoteDuration::Eighth:    durText = "Eighth"; break;
        case NoteDuration::Sixteenth: durText = "16th"; break;
    }
    if (note->isDotted())
        durText += " (dotted)";
    durationValue.setText(durText, juce::dontSendNotification);
    
    velocityValue.setText(juce::String(note->getVelocity()), juce::dontSendNotification);
    
    if (note->getString() >= 0)
        stringValue.setText(juce::String(note->getString() + 1), juce::dontSendNotification);
    else
        stringValue.setText("-", juce::dontSendNotification);
    
    if (note->getFret() >= 0)
        fretValue.setText(juce::String(note->getFret()), juce::dontSendNotification);
    else
        fretValue.setText("-", juce::dontSendNotification);
    
    tiedValue.setText(note->isTied() ? "Yes" : "No", juce::dontSendNotification);
}
