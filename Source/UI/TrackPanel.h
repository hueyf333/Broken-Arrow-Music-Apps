#pragma once
#include <JuceHeader.h>
#include "../Model/Score.h"
#include "../Commands/CommandStack.h"

class TrackPanel : public juce::Component,
                   public juce::ListBoxModel
{
public:
    TrackPanel(Score& score, CommandStack& commandStack);
    ~TrackPanel() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void selectedRowsChanged(int lastRowSelected) override;
    
    void addTrack();
    void removeTrack();
    
    int getSelectedTrackIndex() const { return selectedTrack; }
    
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void selectedTrackChanged(int trackIndex) = 0;
    };
    
    void addListener(Listener* listener) { listeners.add(listener); }
    void removeListener(Listener* listener) { listeners.remove(listener); }
    
private:
    Score& score;
    CommandStack& commandStack;
    
    juce::ListBox trackList;
    juce::TextButton addButton;
    juce::TextButton removeButton;
    
    int selectedTrack = 0;
    
    juce::ListenerList<Listener> listeners;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackPanel)
};
