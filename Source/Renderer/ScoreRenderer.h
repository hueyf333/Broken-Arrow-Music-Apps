#pragma once
#include <JuceHeader.h>
#include "../Model/Score.h"

class ScoreRenderer
{
public:
    ScoreRenderer();
    
    void setScore(Score* score) { this->score = score; }
    void setActiveTrack(int trackIndex) { activeTrackIndex = trackIndex; }
    void setZoom(float zoomLevel) { zoom = juce::jlimit(0.5f, 3.0f, zoomLevel); }
    float getZoom() const { return zoom; }
    
    void setCaretPosition(int measureIndex, int noteIndex);
    void getCaretPosition(int& measureIndex, int& noteIndex) const;
    
    void render(juce::Graphics& g, juce::Rectangle<int> area);
    
    // Get layout information
    juce::Rectangle<float> getMeasureBounds(int measureIndex) const;
    juce::Rectangle<float> getNoteBounds(int measureIndex, int noteIndex) const;
    
    int getTotalWidth() const { return totalWidth; }
    int getTotalHeight() const { return totalHeight; }
    
private:
    void renderStaff(juce::Graphics& g, juce::Rectangle<float> area, const Track& track, int measureStart, int measureEnd);
    void renderMeasure(juce::Graphics& g, juce::Rectangle<float> area, const Measure& measure, int measureIndex);
    void renderNote(juce::Graphics& g, juce::Point<float> position, const Note& note, const KeySignature& keySig);
    void renderRest(juce::Graphics& g, juce::Point<float> position, const Note& note);
    void renderTabStaff(juce::Graphics& g, juce::Rectangle<float> area, const Track& track, int measureStart, int measureEnd);
    void renderTabNote(juce::Graphics& g, juce::Point<float> position, const Note& note, int stringNumber);
    void renderCaret(juce::Graphics& g);
    
    float getNoteWidth(const Note& note) const;
    int getNoteY(int pitch) const; // Y offset from staff center for given pitch
    
    Score* score = nullptr;
    int activeTrackIndex = 0;
    float zoom = 1.0f;
    
    // Caret
    int caretMeasure = 0;
    int caretNote = 0;
    
    // Layout constants
    float staffLineSpacing = 10.0f;
    float measureWidth = 200.0f;
    float staffHeight = 80.0f;
    float tabStaffHeight = 60.0f;
    
    // Cached layout
    struct MeasureLayout
    {
        juce::Rectangle<float> bounds;
        std::vector<juce::Rectangle<float>> noteBounds;
    };
    std::vector<MeasureLayout> measureLayouts;
    
    int totalWidth = 0;
    int totalHeight = 0;
};
