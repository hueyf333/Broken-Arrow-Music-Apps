#pragma once
#include "Command.h"
#include "../Model/Score.h"

// Add Note Command
class AddNoteCommand : public Command
{
public:
    AddNoteCommand(Score& score, int trackIndex, int measureIndex, const Note& note);
    
    bool execute() override;
    bool undo() override;
    juce::String getDescription() const override { return "Add Note"; }
    
private:
    Score& score;
    int trackIndex;
    int measureIndex;
    Note note;
    int insertedIndex = -1;
};

// Remove Note Command
class RemoveNoteCommand : public Command
{
public:
    RemoveNoteCommand(Score& score, int trackIndex, int measureIndex, int noteIndex);
    
    bool execute() override;
    bool undo() override;
    juce::String getDescription() const override { return "Remove Note"; }
    
private:
    Score& score;
    int trackIndex;
    int measureIndex;
    int noteIndex;
    Note removedNote;
};

// Change Duration Command
class ChangeDurationCommand : public Command
{
public:
    ChangeDurationCommand(Score& score, int trackIndex, int measureIndex, int noteIndex, NoteDuration newDuration);
    
    bool execute() override;
    bool undo() override;
    juce::String getDescription() const override { return "Change Duration"; }
    
private:
    Score& score;
    int trackIndex;
    int measureIndex;
    int noteIndex;
    NoteDuration newDuration;
    NoteDuration oldDuration;
};

// Toggle Dotted Command
class ToggleDottedCommand : public Command
{
public:
    ToggleDottedCommand(Score& score, int trackIndex, int measureIndex, int noteIndex);
    
    bool execute() override;
    bool undo() override;
    juce::String getDescription() const override { return "Toggle Dotted"; }
    
private:
    Score& score;
    int trackIndex;
    int measureIndex;
    int noteIndex;
};

// Toggle Tie Command
class ToggleTieCommand : public Command
{
public:
    ToggleTieCommand(Score& score, int trackIndex, int measureIndex, int noteIndex);
    
    bool execute() override;
    bool undo() override;
    juce::String getDescription() const override { return "Toggle Tie"; }
    
private:
    Score& score;
    int trackIndex;
    int measureIndex;
    int noteIndex;
};

// Add Measure Command
class AddMeasureCommand : public Command
{
public:
    AddMeasureCommand(Score& score, int trackIndex);
    
    bool execute() override;
    bool undo() override;
    juce::String getDescription() const override { return "Add Measure"; }
    
private:
    Score& score;
    int trackIndex;
};

// Remove Measure Command
class RemoveMeasureCommand : public Command
{
public:
    RemoveMeasureCommand(Score& score, int trackIndex, int measureIndex);
    
    bool execute() override;
    bool undo() override;
    juce::String getDescription() const override { return "Remove Measure"; }
    
private:
    Score& score;
    int trackIndex;
    int measureIndex;
    Measure removedMeasure;
};

// Add Track Command
class AddTrackCommand : public Command
{
public:
    AddTrackCommand(Score& score, const Track& track);
    
    bool execute() override;
    bool undo() override;
    juce::String getDescription() const override { return "Add Track"; }
    
private:
    Score& score;
    Track track;
};

// Remove Track Command
class RemoveTrackCommand : public Command
{
public:
    RemoveTrackCommand(Score& score, int trackIndex);
    
    bool execute() override;
    bool undo() override;
    juce::String getDescription() const override { return "Remove Track"; }
    
private:
    Score& score;
    int trackIndex;
    Track removedTrack;
};
