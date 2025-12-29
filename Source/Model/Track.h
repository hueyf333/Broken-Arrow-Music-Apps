#pragma once
#include <JuceHeader.h>
#include "Measure.h"

enum class InstrumentType
{
    AcousticGuitar,
    ElectricGuitar,
    Bass,
    Piano,
    Drums,
    Voice
};

class Track
{
public:
    Track() = default;
    explicit Track(const juce::String& name, InstrumentType instrument = InstrumentType::AcousticGuitar);
    
    // Name
    juce::String getName() const { return name; }
    void setName(const juce::String& n) { name = n; }
    
    // Instrument
    InstrumentType getInstrument() const { return instrument; }
    void setInstrument(InstrumentType i) { instrument = i; }
    
    // MIDI channel
    int getMidiChannel() const { return midiChannel; }
    void setMidiChannel(int ch) { midiChannel = juce::jlimit(1, 16, ch); }
    
    // MIDI program (instrument sound)
    int getMidiProgram() const { return midiProgram; }
    void setMidiProgram(int prog) { midiProgram = juce::jlimit(0, 127, prog); }
    
    // Measures
    const std::vector<Measure>& getMeasures() const { return measures; }
    void addMeasure(const Measure& measure) { measures.push_back(measure); }
    void insertMeasure(int index, const Measure& measure);
    void removeMeasure(int index);
    Measure* getMeasureAt(int index);
    int getMeasureCount() const { return static_cast<int>(measures.size()); }
    
    // Tab display
    bool isTabEnabled() const { return showTab; }
    void setTabEnabled(bool enabled) { showTab = enabled; }
    
    int getStringCount() const { return stringCount; }
    void setStringCount(int count) { stringCount = count; }
    
    // String tuning (MIDI notes for each string, low to high)
    const std::vector<int>& getTuning() const { return tuning; }
    void setTuning(const std::vector<int>& t) { tuning = t; }
    void setStandardGuitarTuning(); // E2, A2, D3, G3, B3, E4
    
    // Serialization
    juce::var toVar() const;
    static Track fromVar(const juce::var& v);
    
private:
    juce::String name = "Track 1";
    InstrumentType instrument = InstrumentType::AcousticGuitar;
    int midiChannel = 1;
    int midiProgram = 24; // Acoustic Guitar
    std::vector<Measure> measures;
    
    bool showTab = true;
    int stringCount = 6;
    std::vector<int> tuning = {40, 45, 50, 55, 59, 64}; // Standard guitar tuning
};
