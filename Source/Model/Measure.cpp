#include "Measure.h"

Measure::Measure(const TimeSignature& ts)
    : timeSignature(ts)
{
}

void Measure::insertNote(int index, const Note& note)
{
    if (index >= 0 && index <= static_cast<int>(notes.size()))
        notes.insert(notes.begin() + index, note);
}

void Measure::removeNote(int index)
{
    if (index >= 0 && index < static_cast<int>(notes.size()))
        notes.erase(notes.begin() + index);
}

Note* Measure::getNoteAt(int index)
{
    if (index >= 0 && index < static_cast<int>(notes.size()))
        return &notes[index];
    return nullptr;
}

double Measure::getTotalBeats() const
{
    double total = 0.0;
    for (const auto& note : notes)
        total += note.getDurationInBeats();
    return total;
}

juce::var Measure::toVar() const
{
    auto* obj = new juce::DynamicObject();
    obj->setProperty("timeSignature", timeSignature.toVar());
    obj->setProperty("keySignature", keySignature.toVar());
    
    juce::Array<juce::var> notesArray;
    for (const auto& note : notes)
        notesArray.add(note.toVar());
    obj->setProperty("notes", notesArray);
    
    return juce::var(obj);
}

Measure Measure::fromVar(const juce::var& v)
{
    Measure measure;
    if (auto* obj = v.getDynamicObject())
    {
        measure.timeSignature = TimeSignature::fromVar(obj->getProperty("timeSignature"));
        measure.keySignature = KeySignature::fromVar(obj->getProperty("keySignature"));
        
        if (auto* notesArray = obj->getProperty("notes").getArray())
        {
            for (const auto& noteVar : *notesArray)
                measure.notes.push_back(Note::fromVar(noteVar));
        }
    }
    return measure;
}
