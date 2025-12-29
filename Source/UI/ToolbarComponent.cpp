#include "ToolbarComponent.h"

ToolbarComponent::ToolbarComponent()
{
    auto setupButton = [this](juce::Button& button, const juce::String& text, std::function<void()> callback) {
        addAndMakeVisible(button);
        button.setButtonText(text);
        button.onClick = callback;
    };
    
    setupButton(newButton, "New", [this] { listeners.call(&Listener::newButtonClicked); });
    setupButton(openButton, "Open", [this] { listeners.call(&Listener::openButtonClicked); });
    setupButton(saveButton, "Save", [this] { listeners.call(&Listener::saveButtonClicked); });
    
    setupButton(wholeButton, "Whole", [this] { listeners.call([](Listener& l) { l.durationChanged(NoteDuration::Whole); }); });
    setupButton(halfButton, "Half", [this] { listeners.call([](Listener& l) { l.durationChanged(NoteDuration::Half); }); });
    setupButton(quarterButton, "Quarter", [this] { listeners.call([](Listener& l) { l.durationChanged(NoteDuration::Quarter); }); });
    setupButton(eighthButton, "8th", [this] { listeners.call([](Listener& l) { l.durationChanged(NoteDuration::Eighth); }); });
    setupButton(sixteenthButton, "16th", [this] { listeners.call([](Listener& l) { l.durationChanged(NoteDuration::Sixteenth); }); });
    
    addAndMakeVisible(restButton);
    restButton.setButtonText("Rest");
    restButton.onClick = [this] { listeners.call([this](Listener& l) { l.restToggled(restButton.getToggleState()); }); };
    
    setupButton(playButton, "Play", [this] { listeners.call(&Listener::playButtonClicked); });
    setupButton(stopButton, "Stop", [this] { listeners.call(&Listener::stopButtonClicked); });
    
    addAndMakeVisible(metronomeButton);
    metronomeButton.setButtonText("Metronome");
    metronomeButton.setToggleState(true, juce::dontSendNotification);
    metronomeButton.onClick = [this] { listeners.call([this](Listener& l) { l.metronomeToggled(metronomeButton.getToggleState()); }); };
}

ToolbarComponent::~ToolbarComponent()
{
}

void ToolbarComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff2d2d2d));
}

void ToolbarComponent::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    int buttonWidth = 60;
    int spacing = 5;
    
    newButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing);
    
    openButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing);
    
    saveButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing * 3);
    
    wholeButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing);
    
    halfButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing);
    
    quarterButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing);
    
    eighthButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing);
    
    sixteenthButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing * 3);
    
    restButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing * 3);
    
    playButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing);
    
    stopButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing * 3);
    
    metronomeButton.setBounds(bounds.removeFromLeft(buttonWidth + 30));
}
