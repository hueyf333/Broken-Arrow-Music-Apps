#include "Score.h"

Score::Score()
{
    // Initialize with default time and key signatures
    defaultTimeSignature.numerator = 4;
    defaultTimeSignature.denominator = 4;
    defaultKeySignature.sharps = 0;
    defaultKeySignature.isMajor = true;
}

void Score::insertTrack(int index, const Track& track)
{
    if (index >= 0 && index <= static_cast<int>(tracks.size()))
        tracks.insert(tracks.begin() + index, track);
}

void Score::removeTrack(int index)
{
    if (index >= 0 && index < static_cast<int>(tracks.size()))
        tracks.erase(tracks.begin() + index);
}

Track* Score::getTrackAt(int index)
{
    if (index >= 0 && index < static_cast<int>(tracks.size()))
        return &tracks[index];
    return nullptr;
}

double Score::getTempoAtMeasure(int measureIndex) const
{
    double tempo = defaultTempo;
    
    for (const auto& tc : tempoChanges)
    {
        if (tc.measureIndex <= measureIndex)
            tempo = tc.bpm;
        else
            break;
    }
    
    return tempo;
}

juce::var Score::toVar() const
{
    auto* obj = new juce::DynamicObject();
    obj->setProperty("version", CURRENT_VERSION);
    obj->setProperty("title", title);
    obj->setProperty("artist", artist);
    obj->setProperty("album", album);
    obj->setProperty("defaultTempo", defaultTempo);
    obj->setProperty("defaultTimeSignature", defaultTimeSignature.toVar());
    obj->setProperty("defaultKeySignature", defaultKeySignature.toVar());
    
    juce::Array<juce::var> tempoArray;
    for (const auto& tc : tempoChanges)
        tempoArray.add(tc.toVar());
    obj->setProperty("tempoChanges", tempoArray);
    
    juce::Array<juce::var> tracksArray;
    for (const auto& track : tracks)
        tracksArray.add(track.toVar());
    obj->setProperty("tracks", tracksArray);
    
    return juce::var(obj);
}

Score Score::fromVar(const juce::var& v)
{
    Score score;
    if (auto* obj = v.getDynamicObject())
    {
        // Version check (for future compatibility)
        int version = obj->getProperty("version");
        juce::ignoreUnused(version);
        
        score.title = obj->getProperty("title").toString();
        score.artist = obj->getProperty("artist").toString();
        score.album = obj->getProperty("album").toString();
        score.defaultTempo = obj->getProperty("defaultTempo");
        score.defaultTimeSignature = TimeSignature::fromVar(obj->getProperty("defaultTimeSignature"));
        score.defaultKeySignature = KeySignature::fromVar(obj->getProperty("defaultKeySignature"));
        
        if (auto* tempoArray = obj->getProperty("tempoChanges").getArray())
        {
            score.tempoChanges.clear();
            for (const auto& tcVar : *tempoArray)
                score.tempoChanges.push_back(TempoChange::fromVar(tcVar));
        }
        
        if (auto* tracksArray = obj->getProperty("tracks").getArray())
        {
            score.tracks.clear();
            for (const auto& trackVar : *tracksArray)
                score.tracks.push_back(Track::fromVar(trackVar));
        }
    }
    return score;
}
