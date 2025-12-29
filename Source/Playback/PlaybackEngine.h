#pragma once
#include <JuceHeader.h>
#include "../Model/Score.h"

class PlaybackEngine : public juce::Timer
{
public:
    PlaybackEngine(Score& score);
    ~PlaybackEngine() override;
    
    void play();
    void stop();
    void pause();
    
    bool isPlaying() const { return playing; }
    
    void setMetronomeEnabled(bool enabled) { metronomeEnabled = enabled; }
    bool isMetronomeEnabled() const { return metronomeEnabled; }
    
    double getCurrentTime() const { return currentTime; }
    void setCurrentTime(double time) { currentTime = time; }
    
    int getCurrentMeasure() const { return currentMeasure; }
    int getCurrentBeat() const { return currentBeat; }
    
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void playbackPositionChanged(int measureIndex, double beat) = 0;
    };
    
    void addListener(Listener* listener) { listeners.add(listener); }
    void removeListener(Listener* listener) { listeners.remove(listener); }
    
private:
    void timerCallback() override;
    void playNote(const Note& note, int midiChannel, double startTime);
    void playMetronomeClick(bool isDownbeat);
    
    Score& score;
    juce::Synthesiser synthesiser;
    juce::MidiMessageCollector midiCollector;
    
    bool playing = false;
    bool metronomeEnabled = true;
    
    double currentTime = 0.0; // in seconds
    int currentMeasure = 0;
    int currentBeat = 0;
    
    double sampleRate = 44100.0;
    
    juce::ListenerList<Listener> listeners;
};
