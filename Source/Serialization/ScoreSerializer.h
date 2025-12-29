#pragma once
#include <JuceHeader.h>
#include "../Model/Score.h"

class ScoreSerializer
{
public:
    static bool saveToFile(const Score& score, const juce::File& file);
    static bool loadFromFile(Score& score, const juce::File& file);
    
    static bool exportToMidi(const Score& score, const juce::File& file);
    static bool exportToMusicXML(const Score& score, const juce::File& file);
    
private:
    static juce::String generateMusicXML(const Score& score);
};
