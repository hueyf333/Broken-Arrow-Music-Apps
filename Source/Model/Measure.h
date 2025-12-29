#pragma once
#include <JuceHeader.h>
#include "Note.h"

struct TimeSignature
{
    int numerator = 4;
    int denominator = 4;
    
    juce::var toVar() const
    {
        auto* obj = new juce::DynamicObject();
        obj->setProperty("numerator", numerator);
        obj->setProperty("denominator", denominator);
        return juce::var(obj);
    }
    
    static TimeSignature fromVar(const juce::var& v)
    {
        TimeSignature ts;
        if (auto* obj = v.getDynamicObject())
        {
            ts.numerator = obj->getProperty("numerator");
            ts.denominator = obj->getProperty("denominator");
        }
        return ts;
    }
};

struct KeySignature
{
    int sharps = 0; // Positive for sharps, negative for flats
    bool isMajor = true;
    
    juce::var toVar() const
    {
        auto* obj = new juce::DynamicObject();
        obj->setProperty("sharps", sharps);
        obj->setProperty("isMajor", isMajor);
        return juce::var(obj);
    }
    
    static KeySignature fromVar(const juce::var& v)
    {
        KeySignature ks;
        if (auto* obj = v.getDynamicObject())
        {
            ks.sharps = obj->getProperty("sharps");
            ks.isMajor = obj->getProperty("isMajor");
        }
        return ks;
    }
};

class Measure
{
public:
    Measure() = default;
    explicit Measure(const TimeSignature& ts);
    
    // Time signature
    TimeSignature getTimeSignature() const { return timeSignature; }
    void setTimeSignature(const TimeSignature& ts) { timeSignature = ts; }
    
    // Key signature
    KeySignature getKeySignature() const { return keySignature; }
    void setKeySignature(const KeySignature& ks) { keySignature = ks; }
    
    // Notes (single voice for simplicity)
    const std::vector<Note>& getNotes() const { return notes; }
    void addNote(const Note& note) { notes.push_back(note); }
    void insertNote(int index, const Note& note);
    void removeNote(int index);
    void clearNotes() { notes.clear(); }
    Note* getNoteAt(int index);
    
    int getNoteCount() const { return static_cast<int>(notes.size()); }
    
    // Calculate total duration in beats
    double getTotalBeats() const;
    
    // Serialization
    juce::var toVar() const;
    static Measure fromVar(const juce::var& v);
    
private:
    TimeSignature timeSignature;
    KeySignature keySignature;
    std::vector<Note> notes;
};
