#include "ScoreEditor.h"
#include "../Commands/EditCommands.h"

ScoreEditor::ScoreEditor(Score& s, CommandStack& cs)
    : score(s), commandStack(cs)
{
    renderer.setScore(&score);
    setWantsKeyboardFocus(true);
}

ScoreEditor::~ScoreEditor()
{
}

void ScoreEditor::paint(juce::Graphics& g)
{
    renderer.render(g, getLocalBounds());
}

void ScoreEditor::resized()
{
    repaint();
}

void ScoreEditor::mouseDown(const juce::MouseEvent& event)
{
    Track* track = score.getTrackAt(activeTrackIndex);
    if (!track)
        return;
    
    // Find clicked note/measure
    for (int m = 0; m < track->getMeasureCount(); ++m)
    {
        auto measureBounds = renderer.getMeasureBounds(m);
        if (measureBounds.contains(event.position))
        {
            selectedMeasure = m;
            selectedNote = -1;
            
            Measure* measure = track->getMeasureAt(m);
            if (measure)
            {
                for (int n = 0; n < measure->getNoteCount(); ++n)
                {
                    auto noteBounds = renderer.getNoteBounds(m, n);
                    if (noteBounds.contains(event.position))
                    {
                        selectedNote = n;
                        break;
                    }
                }
            }
            
            renderer.setCaretPosition(selectedMeasure, selectedNote >= 0 ? selectedNote : 0);
            notifySelectionChanged();
            repaint();
            return;
        }
    }
}

void ScoreEditor::mouseUp(const juce::MouseEvent& event)
{
    juce::ignoreUnused(event);
}

void ScoreEditor::mouseDrag(const juce::MouseEvent& event)
{
    juce::ignoreUnused(event);
}

bool ScoreEditor::keyPressed(const juce::KeyPress& key)
{
    // Number keys for pitch input (simplified: C major scale)
    if (key.getKeyCode() >= '0' && key.getKeyCode() <= '9')
    {
        int digit = key.getKeyCode() - '0';
        if (digit >= 0 && digit <= 9)
        {
            // Use digit as fret number for tab
            if (restMode)
            {
                addNoteAtCaret(-1); // Rest
            }
            else
            {
                addNoteAtCaret(60 + digit); // Middle C + offset
            }
        }
        return true;
    }
    
    // Letter keys for note names
    if (key.getKeyCode() >= 'a' && key.getKeyCode() <= 'g')
    {
        const int pitchOffsets[] = {9, 11, 0, 2, 4, 5, 7}; // A, B, C, D, E, F, G
        int pitch = 60 + pitchOffsets[key.getKeyCode() - 'a'];
        addNoteAtCaret(pitch);
        return true;
    }
    
    // Delete key
    if (key.getKeyCode() == juce::KeyPress::deleteKey || key.getKeyCode() == juce::KeyPress::backspaceKey)
    {
        deleteSelectedNote();
        return true;
    }
    
    // Arrow keys for navigation
    if (key.getKeyCode() == juce::KeyPress::leftKey)
    {
        if (selectedNote > 0)
            selectedNote--;
        renderer.setCaretPosition(selectedMeasure, selectedNote);
        notifySelectionChanged();
        repaint();
        return true;
    }
    
    if (key.getKeyCode() == juce::KeyPress::rightKey)
    {
        Track* track = score.getTrackAt(activeTrackIndex);
        if (track)
        {
            Measure* measure = track->getMeasureAt(selectedMeasure);
            if (measure && selectedNote < measure->getNoteCount() - 1)
            {
                selectedNote++;
                renderer.setCaretPosition(selectedMeasure, selectedNote);
                notifySelectionChanged();
                repaint();
            }
        }
        return true;
    }
    
    return false;
}

void ScoreEditor::setActiveTrack(int trackIndex)
{
    activeTrackIndex = trackIndex;
    renderer.setActiveTrack(trackIndex);
    repaint();
}

void ScoreEditor::getCaretPosition(int& measureIndex, int& noteIndex) const
{
    renderer.getCaretPosition(measureIndex, noteIndex);
}

void ScoreEditor::setCaretPosition(int measureIndex, int noteIndex)
{
    selectedMeasure = measureIndex;
    selectedNote = noteIndex;
    renderer.setCaretPosition(measureIndex, noteIndex);
    notifySelectionChanged();
    repaint();
}

void ScoreEditor::setZoom(float zoom)
{
    renderer.setZoom(zoom);
    repaint();
}

void ScoreEditor::addNoteAtCaret(int pitch)
{
    Note note(pitch, currentDuration, restMode || pitch < 0);
    
    // Auto-calculate tab position for guitar (simplified)
    if (!note.isRest())
    {
        Track* track = score.getTrackAt(activeTrackIndex);
        if (track && track->isTabEnabled())
        {
            // Simple algorithm: find first string that can play this note
            const auto& tuning = track->getTuning();
            for (int str = 0; str < static_cast<int>(tuning.size()); ++str)
            {
                int openString = tuning[str];
                if (pitch >= openString && pitch - openString <= 24) // Max 24 frets
                {
                    note.setString(str);
                    note.setFret(pitch - openString);
                    break;
                }
            }
        }
    }
    
    auto command = std::make_unique<AddNoteCommand>(score, activeTrackIndex, selectedMeasure, note);
    commandStack.execute(std::move(command));
    
    // Move caret forward
    Track* track = score.getTrackAt(activeTrackIndex);
    if (track)
    {
        Measure* measure = track->getMeasureAt(selectedMeasure);
        if (measure)
        {
            selectedNote = measure->getNoteCount() - 1;
            renderer.setCaretPosition(selectedMeasure, selectedNote);
        }
    }
    
    notifySelectionChanged();
    repaint();
}

void ScoreEditor::deleteSelectedNote()
{
    if (selectedNote < 0)
        return;
    
    auto command = std::make_unique<RemoveNoteCommand>(score, activeTrackIndex, selectedMeasure, selectedNote);
    commandStack.execute(std::move(command));
    
    if (selectedNote > 0)
        selectedNote--;
    
    renderer.setCaretPosition(selectedMeasure, selectedNote);
    notifySelectionChanged();
    repaint();
}

void ScoreEditor::toggleDotted()
{
    if (selectedNote < 0)
        return;
    
    auto command = std::make_unique<ToggleDottedCommand>(score, activeTrackIndex, selectedMeasure, selectedNote);
    commandStack.execute(std::move(command));
    
    notifySelectionChanged();
    repaint();
}

void ScoreEditor::toggleTie()
{
    if (selectedNote < 0)
        return;
    
    auto command = std::make_unique<ToggleTieCommand>(score, activeTrackIndex, selectedMeasure, selectedNote);
    commandStack.execute(std::move(command));
    
    notifySelectionChanged();
    repaint();
}

void ScoreEditor::notifySelectionChanged()
{
    listeners.call([this](Listener& l) {
        l.selectionChanged(activeTrackIndex, selectedMeasure, selectedNote);
    });
}
