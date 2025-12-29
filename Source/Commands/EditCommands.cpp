#include "EditCommands.h"

// AddNoteCommand
AddNoteCommand::AddNoteCommand(Score& s, int tIdx, int mIdx, const Note& n)
    : score(s), trackIndex(tIdx), measureIndex(mIdx), note(n)
{
}

bool AddNoteCommand::execute()
{
    Track* track = score.getTrackAt(trackIndex);
    if (!track) return false;
    
    Measure* measure = track->getMeasureAt(measureIndex);
    if (!measure) return false;
    
    insertedIndex = measure->getNoteCount();
    measure->addNote(note);
    return true;
}

bool AddNoteCommand::undo()
{
    Track* track = score.getTrackAt(trackIndex);
    if (!track) return false;
    
    Measure* measure = track->getMeasureAt(measureIndex);
    if (!measure) return false;
    
    if (insertedIndex >= 0 && insertedIndex < measure->getNoteCount())
    {
        measure->removeNote(insertedIndex);
        return true;
    }
    return false;
}

// RemoveNoteCommand
RemoveNoteCommand::RemoveNoteCommand(Score& s, int tIdx, int mIdx, int nIdx)
    : score(s), trackIndex(tIdx), measureIndex(mIdx), noteIndex(nIdx)
{
}

bool RemoveNoteCommand::execute()
{
    Track* track = score.getTrackAt(trackIndex);
    if (!track) return false;
    
    Measure* measure = track->getMeasureAt(measureIndex);
    if (!measure) return false;
    
    Note* notePtr = measure->getNoteAt(noteIndex);
    if (!notePtr) return false;
    
    removedNote = *notePtr;
    measure->removeNote(noteIndex);
    return true;
}

bool RemoveNoteCommand::undo()
{
    Track* track = score.getTrackAt(trackIndex);
    if (!track) return false;
    
    Measure* measure = track->getMeasureAt(measureIndex);
    if (!measure) return false;
    
    measure->insertNote(noteIndex, removedNote);
    return true;
}

// ChangeDurationCommand
ChangeDurationCommand::ChangeDurationCommand(Score& s, int tIdx, int mIdx, int nIdx, NoteDuration newDur)
    : score(s), trackIndex(tIdx), measureIndex(mIdx), noteIndex(nIdx), newDuration(newDur)
{
}

bool ChangeDurationCommand::execute()
{
    Track* track = score.getTrackAt(trackIndex);
    if (!track) return false;
    
    Measure* measure = track->getMeasureAt(measureIndex);
    if (!measure) return false;
    
    Note* note = measure->getNoteAt(noteIndex);
    if (!note) return false;
    
    oldDuration = note->getDuration();
    note->setDuration(newDuration);
    return true;
}

bool ChangeDurationCommand::undo()
{
    Track* track = score.getTrackAt(trackIndex);
    if (!track) return false;
    
    Measure* measure = track->getMeasureAt(measureIndex);
    if (!measure) return false;
    
    Note* note = measure->getNoteAt(noteIndex);
    if (!note) return false;
    
    note->setDuration(oldDuration);
    return true;
}

// ToggleDottedCommand
ToggleDottedCommand::ToggleDottedCommand(Score& s, int tIdx, int mIdx, int nIdx)
    : score(s), trackIndex(tIdx), measureIndex(mIdx), noteIndex(nIdx)
{
}

bool ToggleDottedCommand::execute()
{
    Track* track = score.getTrackAt(trackIndex);
    if (!track) return false;
    
    Measure* measure = track->getMeasureAt(measureIndex);
    if (!measure) return false;
    
    Note* note = measure->getNoteAt(noteIndex);
    if (!note) return false;
    
    note->setDotted(!note->isDotted());
    return true;
}

bool ToggleDottedCommand::undo()
{
    return execute(); // Toggle is reversible
}

// ToggleTieCommand
ToggleTieCommand::ToggleTieCommand(Score& s, int tIdx, int mIdx, int nIdx)
    : score(s), trackIndex(tIdx), measureIndex(mIdx), noteIndex(nIdx)
{
}

bool ToggleTieCommand::execute()
{
    Track* track = score.getTrackAt(trackIndex);
    if (!track) return false;
    
    Measure* measure = track->getMeasureAt(measureIndex);
    if (!measure) return false;
    
    Note* note = measure->getNoteAt(noteIndex);
    if (!note) return false;
    
    note->setTied(!note->isTied());
    return true;
}

bool ToggleTieCommand::undo()
{
    return execute(); // Toggle is reversible
}

// AddMeasureCommand
AddMeasureCommand::AddMeasureCommand(Score& s, int tIdx)
    : score(s), trackIndex(tIdx)
{
}

bool AddMeasureCommand::execute()
{
    Track* track = score.getTrackAt(trackIndex);
    if (!track) return false;
    
    Measure newMeasure;
    newMeasure.setTimeSignature(score.getDefaultTimeSignature());
    newMeasure.setKeySignature(score.getDefaultKeySignature());
    track->addMeasure(newMeasure);
    return true;
}

bool AddMeasureCommand::undo()
{
    Track* track = score.getTrackAt(trackIndex);
    if (!track) return false;
    
    int lastIndex = track->getMeasureCount() - 1;
    if (lastIndex >= 0)
    {
        track->removeMeasure(lastIndex);
        return true;
    }
    return false;
}

// RemoveMeasureCommand
RemoveMeasureCommand::RemoveMeasureCommand(Score& s, int tIdx, int mIdx)
    : score(s), trackIndex(tIdx), measureIndex(mIdx)
{
}

bool RemoveMeasureCommand::execute()
{
    Track* track = score.getTrackAt(trackIndex);
    if (!track) return false;
    
    Measure* measure = track->getMeasureAt(measureIndex);
    if (!measure) return false;
    
    removedMeasure = *measure;
    track->removeMeasure(measureIndex);
    return true;
}

bool RemoveMeasureCommand::undo()
{
    Track* track = score.getTrackAt(trackIndex);
    if (!track) return false;
    
    track->insertMeasure(measureIndex, removedMeasure);
    return true;
}

// AddTrackCommand
AddTrackCommand::AddTrackCommand(Score& s, const Track& t)
    : score(s), track(t)
{
}

bool AddTrackCommand::execute()
{
    score.addTrack(track);
    return true;
}

bool AddTrackCommand::undo()
{
    int lastIndex = score.getTrackCount() - 1;
    if (lastIndex >= 0)
    {
        score.removeTrack(lastIndex);
        return true;
    }
    return false;
}

// RemoveTrackCommand
RemoveTrackCommand::RemoveTrackCommand(Score& s, int tIdx)
    : score(s), trackIndex(tIdx)
{
}

bool RemoveTrackCommand::execute()
{
    Track* track = score.getTrackAt(trackIndex);
    if (!track) return false;
    
    removedTrack = *track;
    score.removeTrack(trackIndex);
    return true;
}

bool RemoveTrackCommand::undo()
{
    score.insertTrack(trackIndex, removedTrack);
    return true;
}
