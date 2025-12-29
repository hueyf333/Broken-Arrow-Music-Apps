#include "Note.h"

Note::Note(int midiPitch, NoteDuration dur, bool isRest)
    : pitch(juce::jlimit(0, 127, midiPitch))
    , duration(dur)
    , rest(isRest)
{
}

double Note::getDurationInBeats() const
{
    double beats = 4.0 / static_cast<int>(duration);
    if (dotted)
        beats *= 1.5;
    return beats;
}

int Note::getAccidental(int keySignature) const
{
    // Simplified: return 0 for natural, 1 for sharp, -1 for flat
    // Based on pitch class and key signature
    int pitchClass = pitch % 12;
    
    // Natural notes: C, D, E, F, G, A, B (0, 2, 4, 5, 7, 9, 11)
    const int naturalNotes[] = {0, 2, 4, 5, 7, 9, 11};
    
    for (int natural : naturalNotes)
    {
        if (pitchClass == natural)
            return 0; // Natural
    }
    
    // Sharp notes (1, 3, 6, 8, 10)
    if (pitchClass == 1 || pitchClass == 3 || pitchClass == 6 || pitchClass == 8 || pitchClass == 10)
        return 1; // Sharp (could also be flat, but default to sharp)
    
    return 0;
}

juce::var Note::toVar() const
{
    auto* obj = new juce::DynamicObject();
    obj->setProperty("pitch", pitch);
    obj->setProperty("duration", static_cast<int>(duration));
    obj->setProperty("dotted", dotted);
    obj->setProperty("rest", rest);
    obj->setProperty("tied", tied);
    obj->setProperty("articulation", static_cast<int>(articulation));
    obj->setProperty("velocity", velocity);
    obj->setProperty("string", stringNumber);
    obj->setProperty("fret", fret);
    return juce::var(obj);
}

Note Note::fromVar(const juce::var& v)
{
    Note note;
    if (auto* obj = v.getDynamicObject())
    {
        note.pitch = obj->getProperty("pitch");
        note.duration = static_cast<NoteDuration>(static_cast<int>(obj->getProperty("duration")));
        note.dotted = obj->getProperty("dotted");
        note.rest = obj->getProperty("rest");
        note.tied = obj->getProperty("tied");
        note.articulation = static_cast<Articulation>(static_cast<int>(obj->getProperty("articulation")));
        note.velocity = obj->getProperty("velocity");
        note.stringNumber = obj->getProperty("string");
        note.fret = obj->getProperty("fret");
    }
    return note;
}
