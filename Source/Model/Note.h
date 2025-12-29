#pragma once
#include <JuceHeader.h>

enum class NoteDuration
{
    Whole = 1,
    Half = 2,
    Quarter = 4,
    Eighth = 8,
    Sixteenth = 16
};

enum class Articulation
{
    None,
    Staccato,
    Accent,
    Tenuto
};

class Note
{
public:
    Note() = default;
    Note(int midiPitch, NoteDuration duration, bool isRest = false);
    
    // Pitch (MIDI note number 0-127)
    int getPitch() const { return pitch; }
    void setPitch(int p) { pitch = juce::jlimit(0, 127, p); }
    
    // Duration
    NoteDuration getDuration() const { return duration; }
    void setDuration(NoteDuration d) { duration = d; }
    
    // Dotted (increases duration by 50%)
    bool isDotted() const { return dotted; }
    void setDotted(bool d) { dotted = d; }
    
    // Rest (no pitch played)
    bool isRest() const { return rest; }
    void setRest(bool r) { rest = r; }
    
    // Tie (sustain into next note)
    bool isTied() const { return tied; }
    void setTied(bool t) { tied = t; }
    
    // Articulation
    Articulation getArticulation() const { return articulation; }
    void setArticulation(Articulation a) { articulation = a; }
    
    // Velocity (0-127, for playback)
    int getVelocity() const { return velocity; }
    void setVelocity(int v) { velocity = juce::jlimit(0, 127, v); }
    
    // Tab properties
    int getString() const { return stringNumber; }
    void setString(int s) { stringNumber = s; }
    
    int getFret() const { return fret; }
    void setFret(int f) { fret = f; }
    
    // Duration in beats (considering dotted)
    double getDurationInBeats() const;
    
    // Accidental calculation (sharp, flat, natural) based on key signature
    int getAccidental(int keySignature) const;
    
    // Serialization
    juce::var toVar() const;
    static Note fromVar(const juce::var& v);
    
private:
    int pitch = 60; // Middle C
    NoteDuration duration = NoteDuration::Quarter;
    bool dotted = false;
    bool rest = false;
    bool tied = false;
    Articulation articulation = Articulation::None;
    int velocity = 80;
    
    // Tab-specific
    int stringNumber = -1; // -1 means not set
    int fret = -1;
};
