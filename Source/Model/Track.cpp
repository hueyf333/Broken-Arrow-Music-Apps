#include "Track.h"

Track::Track(const juce::String& trackName, InstrumentType instr)
    : name(trackName)
    , instrument(instr)
{
    setStandardGuitarTuning();
}

void Track::insertMeasure(int index, const Measure& measure)
{
    if (index >= 0 && index <= static_cast<int>(measures.size()))
        measures.insert(measures.begin() + index, measure);
}

void Track::removeMeasure(int index)
{
    if (index >= 0 && index < static_cast<int>(measures.size()))
        measures.erase(measures.begin() + index);
}

Measure* Track::getMeasureAt(int index)
{
    if (index >= 0 && index < static_cast<int>(measures.size()))
        return &measures[index];
    return nullptr;
}

void Track::setStandardGuitarTuning()
{
    // Standard guitar: E2(40), A2(45), D3(50), G3(55), B3(59), E4(64)
    tuning = {40, 45, 50, 55, 59, 64};
    stringCount = 6;
}

juce::var Track::toVar() const
{
    auto* obj = new juce::DynamicObject();
    obj->setProperty("name", name);
    obj->setProperty("instrument", static_cast<int>(instrument));
    obj->setProperty("midiChannel", midiChannel);
    obj->setProperty("midiProgram", midiProgram);
    obj->setProperty("showTab", showTab);
    obj->setProperty("stringCount", stringCount);
    
    juce::Array<juce::var> tuningArray;
    for (int t : tuning)
        tuningArray.add(t);
    obj->setProperty("tuning", tuningArray);
    
    juce::Array<juce::var> measuresArray;
    for (const auto& measure : measures)
        measuresArray.add(measure.toVar());
    obj->setProperty("measures", measuresArray);
    
    return juce::var(obj);
}

Track Track::fromVar(const juce::var& v)
{
    Track track;
    if (auto* obj = v.getDynamicObject())
    {
        track.name = obj->getProperty("name").toString();
        track.instrument = static_cast<InstrumentType>(static_cast<int>(obj->getProperty("instrument")));
        track.midiChannel = obj->getProperty("midiChannel");
        track.midiProgram = obj->getProperty("midiProgram");
        track.showTab = obj->getProperty("showTab");
        track.stringCount = obj->getProperty("stringCount");
        
        if (auto* tuningArray = obj->getProperty("tuning").getArray())
        {
            track.tuning.clear();
            for (const auto& t : *tuningArray)
                track.tuning.push_back(static_cast<int>(t));
        }
        
        if (auto* measuresArray = obj->getProperty("measures").getArray())
        {
            track.measures.clear();
            for (const auto& measureVar : *measuresArray)
                track.measures.push_back(Measure::fromVar(measureVar));
        }
    }
    return track;
}
