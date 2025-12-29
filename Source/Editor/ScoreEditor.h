#pragma once
#include <JuceHeader.h>
#include "../Model/Score.h"
#include "../Renderer/ScoreRenderer.h"
#include "../Commands/CommandStack.h"

class ScoreEditor : public juce::Component
{
public:
    ScoreEditor(Score& score, CommandStack& commandStack);
    ~ScoreEditor() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    
    bool keyPressed(const juce::KeyPress& key) override;
    
    void setActiveTrack(int trackIndex);
    int getActiveTrack() const { return activeTrackIndex; }
    
    void setCurrentDuration(NoteDuration duration) { currentDuration = duration; }
    NoteDuration getCurrentDuration() const { return currentDuration; }
    
    void setRestMode(bool enabled) { restMode = enabled; }
    bool isRestMode() const { return restMode; }
    
    void getCaretPosition(int& measureIndex, int& noteIndex) const;
    void setCaretPosition(int measureIndex, int noteIndex);
    
    void setZoom(float zoom);
    float getZoom() const { return renderer.getZoom(); }
    
    // Actions
    void addNoteAtCaret(int pitch);
    void deleteSelectedNote();
    void toggleDotted();
    void toggleTie();
    
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void selectionChanged(int trackIndex, int measureIndex, int noteIndex) = 0;
    };
    
    void addListener(Listener* listener) { listeners.add(listener); }
    void removeListener(Listener* listener) { listeners.remove(listener); }
    
private:
    void notifySelectionChanged();
    
    Score& score;
    CommandStack& commandStack;
    ScoreRenderer renderer;
    
    int activeTrackIndex = 0;
    NoteDuration currentDuration = NoteDuration::Quarter;
    bool restMode = false;
    
    int selectedMeasure = 0;
    int selectedNote = -1;
    
    juce::ListenerList<Listener> listeners;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScoreEditor)
};
