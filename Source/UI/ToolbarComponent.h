#pragma once
#include <JuceHeader.h>
#include "../Model/Note.h"

class ToolbarComponent : public juce::Component
{
public:
    ToolbarComponent();
    ~ToolbarComponent() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void newButtonClicked() = 0;
        virtual void openButtonClicked() = 0;
        virtual void saveButtonClicked() = 0;
        virtual void durationChanged(NoteDuration duration) = 0;
        virtual void restToggled(bool enabled) = 0;
        virtual void playButtonClicked() = 0;
        virtual void stopButtonClicked() = 0;
        virtual void metronomeToggled(bool enabled) = 0;
    };
    
    void addListener(Listener* listener) { listeners.add(listener); }
    void removeListener(Listener* listener) { listeners.remove(listener); }
    
    void setRestEnabled(bool enabled) { restButton.setToggleState(enabled, juce::dontSendNotification); }
    void setMetronomeEnabled(bool enabled) { metronomeButton.setToggleState(enabled, juce::dontSendNotification); }
    
private:
    juce::TextButton newButton;
    juce::TextButton openButton;
    juce::TextButton saveButton;
    
    juce::TextButton wholeButton;
    juce::TextButton halfButton;
    juce::TextButton quarterButton;
    juce::TextButton eighthButton;
    juce::TextButton sixteenthButton;
    
    juce::ToggleButton restButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::ToggleButton metronomeButton;
    
    juce::ListenerList<Listener> listeners;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToolbarComponent)
};
