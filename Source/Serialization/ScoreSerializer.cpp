#include "ScoreSerializer.h"

bool ScoreSerializer::saveToFile(const Score& score, const juce::File& file)
{
    juce::var scoreData = score.toVar();
    juce::String jsonString = juce::JSON::toString(scoreData, true);
    
    return file.replaceWithText(jsonString);
}

bool ScoreSerializer::loadFromFile(Score& score, const juce::File& file)
{
    if (!file.existsAsFile())
        return false;
    
    juce::String jsonString = file.loadFileAsString();
    juce::var parsedData;
    
    juce::Result result = juce::JSON::parse(jsonString, parsedData);
    if (result.failed())
        return false;
    
    score = Score::fromVar(parsedData);
    return true;
}

bool ScoreSerializer::exportToMidi(const Score& score, const juce::File& file)
{
    juce::MidiFile midiFile;
    
    // Time division (ticks per quarter note)
    midiFile.setTicksPerQuarterNote(480);
    
    // Create a MIDI track for each score track
    for (int trackIdx = 0; trackIdx < score.getTrackCount(); ++trackIdx)
    {
        const Track* track = const_cast<Score&>(score).getTrackAt(trackIdx);
        if (!track)
            continue;
        
        juce::MidiMessageSequence sequence;
        
        // Set program change (instrument)
        sequence.addEvent(juce::MidiMessage::programChange(track->getMidiChannel(), track->getMidiProgram()), 0.0);
        
        double currentTime = 0.0;
        double tempo = score.getDefaultTempo();
        double ticksPerBeat = 480.0;
        
        // Convert BPM to microseconds per quarter note
        double microsecondsPerQuarterNote = 60000000.0 / tempo;
        
        // Add tempo meta event
        sequence.addEvent(juce::MidiMessage::tempoMetaEvent(static_cast<int>(microsecondsPerQuarterNote)), 0.0);
        
        // Process each measure
        for (int measureIdx = 0; measureIdx < track->getMeasureCount(); ++measureIdx)
        {
            const Measure* measure = const_cast<Track*>(track)->getMeasureAt(measureIdx);
            if (!measure)
                continue;
            
            // Process each note in the measure
            for (int noteIdx = 0; noteIdx < measure->getNoteCount(); ++noteIdx)
            {
                const Note& note = measure->getNotes()[noteIdx];
                
                if (!note.isRest())
                {
                    double durationInBeats = note.getDurationInBeats();
                    double durationInTicks = durationInBeats * ticksPerBeat;
                    
                    // Note on
                    sequence.addEvent(juce::MidiMessage::noteOn(track->getMidiChannel(), 
                                                                note.getPitch(), 
                                                                static_cast<juce::uint8>(note.getVelocity())),
                                     currentTime);
                    
                    // Note off
                    sequence.addEvent(juce::MidiMessage::noteOff(track->getMidiChannel(), 
                                                                 note.getPitch()),
                                     currentTime + durationInTicks);
                }
                
                currentTime += note.getDurationInBeats() * ticksPerBeat;
            }
        }
        
        sequence.updateMatchedPairs();
        midiFile.addTrack(sequence);
    }
    
    juce::FileOutputStream stream(file);
    if (!stream.openedOk())
        return false;
    
    return midiFile.writeTo(stream);
}

bool ScoreSerializer::exportToMusicXML(const Score& score, const juce::File& file)
{
    juce::String xml = generateMusicXML(score);
    return file.replaceWithText(xml);
}

juce::String ScoreSerializer::generateMusicXML(const Score& score)
{
    juce::String xml;
    
    xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml << "<!DOCTYPE score-partwise PUBLIC \"-//Recordare//DTD MusicXML 3.1 Partwise//EN\" \"http://www.musicxml.org/dtds/partwise.dtd\">\n";
    xml << "<score-partwise version=\"3.1\">\n";
    
    // Metadata
    xml << "  <work>\n";
    xml << "    <work-title>" << score.getTitle() << "</work-title>\n";
    xml << "  </work>\n";
    
    xml << "  <identification>\n";
    xml << "    <creator type=\"composer\">" << score.getArtist() << "</creator>\n";
    xml << "    <encoding>\n";
    xml << "      <software>Broken Arrow Tab &amp; Score Studio</software>\n";
    xml << "    </encoding>\n";
    xml << "  </identification>\n";
    
    // Part list
    xml << "  <part-list>\n";
    for (int i = 0; i < score.getTrackCount(); ++i)
    {
        const Track* track = const_cast<Score&>(score).getTrackAt(i);
        if (track)
        {
            xml << "    <score-part id=\"P" << (i + 1) << "\">\n";
            xml << "      <part-name>" << track->getName() << "</part-name>\n";
            xml << "    </score-part>\n";
        }
    }
    xml << "  </part-list>\n";
    
    // Parts
    for (int trackIdx = 0; trackIdx < score.getTrackCount(); ++trackIdx)
    {
        const Track* track = const_cast<Score&>(score).getTrackAt(trackIdx);
        if (!track)
            continue;
        
        xml << "  <part id=\"P" << (trackIdx + 1) << "\">\n";
        
        // Measures
        for (int measureIdx = 0; measureIdx < track->getMeasureCount(); ++measureIdx)
        {
            const Measure* measure = const_cast<Track*>(track)->getMeasureAt(measureIdx);
            if (!measure)
                continue;
            
            xml << "    <measure number=\"" << (measureIdx + 1) << "\">\n";
            
            // Attributes (time signature, key signature) on first measure
            if (measureIdx == 0)
            {
                xml << "      <attributes>\n";
                xml << "        <divisions>1</divisions>\n";
                
                auto ts = measure->getTimeSignature();
                xml << "        <time>\n";
                xml << "          <beats>" << ts.numerator << "</beats>\n";
                xml << "          <beat-type>" << ts.denominator << "</beat-type>\n";
                xml << "        </time>\n";
                
                auto ks = measure->getKeySignature();
                xml << "        <key>\n";
                xml << "          <fifths>" << ks.sharps << "</fifths>\n";
                xml << "        </key>\n";
                
                xml << "        <clef>\n";
                xml << "          <sign>G</sign>\n";
                xml << "          <line>2</line>\n";
                xml << "        </clef>\n";
                xml << "      </attributes>\n";
            }
            
            // Notes
            for (int noteIdx = 0; noteIdx < measure->getNoteCount(); ++noteIdx)
            {
                const Note& note = measure->getNotes()[noteIdx];
                
                xml << "      <note>\n";
                
                if (note.isRest())
                {
                    xml << "        <rest/>\n";
                }
                else
                {
                    // Pitch
                    int midiPitch = note.getPitch();
                    int octave = (midiPitch / 12) - 1;
                    int pitchClass = midiPitch % 12;
                    
                    const char* stepNames[] = {"C", "C", "D", "D", "E", "F", "F", "G", "G", "A", "A", "B"};
                    const int alterations[] = {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0};
                    
                    xml << "        <pitch>\n";
                    xml << "          <step>" << stepNames[pitchClass] << "</step>\n";
                    if (alterations[pitchClass] != 0)
                        xml << "          <alter>" << alterations[pitchClass] << "</alter>\n";
                    xml << "          <octave>" << octave << "</octave>\n";
                    xml << "        </pitch>\n";
                }
                
                // Duration (simplified)
                int duration = static_cast<int>(note.getDuration());
                xml << "        <duration>" << (4 / duration) << "</duration>\n";
                
                xml << "        <type>";
                switch (note.getDuration())
                {
                    case NoteDuration::Whole:     xml << "whole"; break;
                    case NoteDuration::Half:      xml << "half"; break;
                    case NoteDuration::Quarter:   xml << "quarter"; break;
                    case NoteDuration::Eighth:    xml << "eighth"; break;
                    case NoteDuration::Sixteenth: xml << "16th"; break;
                }
                xml << "</type>\n";
                
                if (note.isDotted())
                    xml << "        <dot/>\n";
                
                xml << "      </note>\n";
            }
            
            xml << "    </measure>\n";
        }
        
        xml << "  </part>\n";
    }
    
    xml << "</score-partwise>\n";
    
    return xml;
}
