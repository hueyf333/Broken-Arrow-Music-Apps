#include "PlaybackEngine.h"

PlaybackEngine::PlaybackEngine(Score& s)
    : score(s)
{
    // Initialize synthesiser with basic voices
    // Note: For a more complete implementation, you would add actual synthesizer voices
    // For now, this provides a basic framework that compiles
    for (int i = 0; i < 8; ++i)
        synthesiser.addVoice(new juce::SamplerVoice());
    
    // Note: In a production app, you would load actual audio samples here
    // This is a placeholder that allows compilation
    synthesiser.setNoteStealingEnabled(true);
}

PlaybackEngine::~PlaybackEngine()
{
    stop();
}

void PlaybackEngine::play()
{
    if (playing)
        return;
    
    playing = true;
    currentTime = 0.0;
    currentMeasure = 0;
    currentBeat = 0;
    
    // Start timer for playback updates (60 fps)
    startTimer(16);
    
    // Note: In a real implementation, this would interface with audio output
    // For now, we'll use JUCE's synthesiser in a simplified manner
}

void PlaybackEngine::stop()
{
    playing = false;
    stopTimer();
    synthesiser.allNotesOff(0, true);
    currentTime = 0.0;
    currentMeasure = 0;
    currentBeat = 0;
}

void PlaybackEngine::pause()
{
    if (playing)
    {
        playing = false;
        stopTimer();
        synthesiser.allNotesOff(0, true);
    }
    else
    {
        playing = true;
        startTimer(16);
    }
}

void PlaybackEngine::timerCallback()
{
    if (!playing)
        return;
    
    double tempo = score.getDefaultTempo();
    double secondsPerBeat = 60.0 / tempo;
    
    // Update current time
    currentTime += 0.016; // ~60 fps
    
    // Calculate current measure and beat
    double totalBeats = currentTime / secondsPerBeat;
    
    auto ts = score.getDefaultTimeSignature();
    int beatsPerMeasure = ts.numerator;
    
    currentMeasure = static_cast<int>(totalBeats / beatsPerMeasure);
    currentBeat = static_cast<int>(totalBeats) % beatsPerMeasure;
    
    // Metronome click
    if (metronomeEnabled)
    {
        double beatFraction = std::fmod(totalBeats, 1.0);
        if (beatFraction < 0.05) // Close to beat boundary
        {
            bool isDownbeat = (currentBeat == 0);
            playMetronomeClick(isDownbeat);
        }
    }
    
    // Notify listeners
    listeners.call([this](Listener& l) {
        l.playbackPositionChanged(currentMeasure, currentBeat);
    });
    
    // Stop at end of score
    if (score.getTrackCount() > 0)
    {
        Track* firstTrack = score.getTrackAt(0);
        if (firstTrack && currentMeasure >= firstTrack->getMeasureCount())
        {
            stop();
        }
    }
}

void PlaybackEngine::playNote(const Note& note, int midiChannel, double startTime)
{
    juce::ignoreUnused(startTime);
    
    if (note.isRest())
        return;
    
    synthesiser.noteOn(midiChannel, note.getPitch(), static_cast<float>(note.getVelocity()) / 127.0f);
    
    // Schedule note off (simplified)
    // In real implementation, this would be handled by audio callback
}

void PlaybackEngine::playMetronomeClick(bool isDownbeat)
{
    // Play a click sound
    int clickPitch = isDownbeat ? 76 : 74; // High and low woodblock
    synthesiser.noteOn(10, clickPitch, 0.8f); // Channel 10 is typically drums
}
