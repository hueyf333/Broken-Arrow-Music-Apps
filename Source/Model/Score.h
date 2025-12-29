#pragma once
#include <JuceHeader.h>
#include "Track.h"

struct TempoChange
{
    int measureIndex = 0;
    double beat = 0.0;
    double bpm = 120.0;
    
    juce::var toVar() const
    {
        auto* obj = new juce::DynamicObject();
        obj->setProperty("measureIndex", measureIndex);
        obj->setProperty("beat", beat);
        obj->setProperty("bpm", bpm);
        return juce::var(obj);
    }
    
    static TempoChange fromVar(const juce::var& v)
    {
        TempoChange tc;
        if (auto* obj = v.getDynamicObject())
        {
            tc.measureIndex = obj->getProperty("measureIndex");
            tc.beat = obj->getProperty("beat");
            tc.bpm = obj->getProperty("bpm");
        }
        return tc;
    }
};

class Score
{
public:
    Score();
    
    // Metadata
    juce::String getTitle() const { return title; }
    void setTitle(const juce::String& t) { title = t; }
    
    juce::String getArtist() const { return artist; }
    void setArtist(const juce::String& a) { artist = a; }
    
    juce::String getAlbum() const { return album; }
    void setAlbum(const juce::String& a) { album = a; }
    
    // Tempo
    double getDefaultTempo() const { return defaultTempo; }
    void setDefaultTempo(double bpm) { defaultTempo = bpm; }
    
    const std::vector<TempoChange>& getTempoChanges() const { return tempoChanges; }
    void addTempoChange(const TempoChange& tc) { tempoChanges.push_back(tc); }
    double getTempoAtMeasure(int measureIndex) const;
    
    // Time and key signatures (global defaults)
    TimeSignature getDefaultTimeSignature() const { return defaultTimeSignature; }
    void setDefaultTimeSignature(const TimeSignature& ts) { defaultTimeSignature = ts; }
    
    KeySignature getDefaultKeySignature() const { return defaultKeySignature; }
    void setDefaultKeySignature(const KeySignature& ks) { defaultKeySignature = ks; }
    
    // Tracks
    const std::vector<Track>& getTracks() const { return tracks; }
    void addTrack(const Track& track) { tracks.push_back(track); }
    void insertTrack(int index, const Track& track);
    void removeTrack(int index);
    Track* getTrackAt(int index);
    int getTrackCount() const { return static_cast<int>(tracks.size()); }
    
    // Serialization
    juce::var toVar() const;
    static Score fromVar(const juce::var& v);
    
    // File format version
    static constexpr int CURRENT_VERSION = 1;
    
private:
    juce::String title = "Untitled";
    juce::String artist;
    juce::String album;
    
    double defaultTempo = 120.0;
    std::vector<TempoChange> tempoChanges;
    
    TimeSignature defaultTimeSignature;
    KeySignature defaultKeySignature;
    
    std::vector<Track> tracks;
};
