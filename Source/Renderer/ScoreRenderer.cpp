#include "ScoreRenderer.h"

ScoreRenderer::ScoreRenderer()
{
}

void ScoreRenderer::setCaretPosition(int measureIndex, int noteIndex)
{
    caretMeasure = measureIndex;
    caretNote = noteIndex;
}

void ScoreRenderer::getCaretPosition(int& measureIndex, int& noteIndex) const
{
    measureIndex = caretMeasure;
    noteIndex = caretNote;
}

void ScoreRenderer::render(juce::Graphics& g, juce::Rectangle<int> area)
{
    if (!score)
        return;
    
    g.fillAll(juce::Colour(0xff1e1e1e)); // Dark background
    
    Track* track = score->getTrackAt(activeTrackIndex);
    if (!track)
        return;
    
    g.setColour(juce::Colours::white);
    
    // Render staff
    float yPos = 50.0f * zoom;
    juce::Rectangle<float> staffArea(20.0f * zoom, yPos, 
                                     static_cast<float>(area.getWidth()) - 40.0f * zoom, 
                                     staffHeight * zoom);
    
    renderStaff(g, staffArea, *track, 0, track->getMeasureCount());
    
    // Render tab staff if enabled
    if (track->isTabEnabled())
    {
        yPos += (staffHeight + 30.0f) * zoom;
        juce::Rectangle<float> tabArea(20.0f * zoom, yPos,
                                       static_cast<float>(area.getWidth()) - 40.0f * zoom,
                                       tabStaffHeight * zoom);
        renderTabStaff(g, tabArea, *track, 0, track->getMeasureCount());
    }
    
    // Update total dimensions
    totalHeight = static_cast<int>(yPos + tabStaffHeight * zoom + 50.0f * zoom);
    totalWidth = static_cast<int>(track->getMeasureCount() * measureWidth * zoom + 40.0f * zoom);
    
    // Render caret
    renderCaret(g);
}

void ScoreRenderer::renderStaff(juce::Graphics& g, juce::Rectangle<float> area, const Track& track, int measureStart, int measureEnd)
{
    measureLayouts.clear();
    
    float xPos = area.getX();
    float yCenter = area.getY() + area.getHeight() / 2.0f;
    
    // Draw staff lines (5 lines)
    g.setColour(juce::Colour(0xff666666));
    for (int i = -2; i <= 2; ++i)
    {
        float y = yCenter + i * staffLineSpacing * zoom;
        g.drawLine(xPos, y, xPos + (measureEnd - measureStart) * measureWidth * zoom, y, 1.0f);
    }
    
    // Draw measures
    g.setColour(juce::Colours::white);
    for (int i = measureStart; i < measureEnd; ++i)
    {
        const Measure* measure = const_cast<Track&>(track).getMeasureAt(i);
        if (!measure)
            continue;
        
        juce::Rectangle<float> measureBounds(xPos, area.getY(), measureWidth * zoom, area.getHeight());
        
        // Draw barline at start of measure
        g.drawLine(measureBounds.getX(), measureBounds.getY(), 
                  measureBounds.getX(), measureBounds.getBottom(), 2.0f);
        
        renderMeasure(g, measureBounds, *measure, i);
        
        xPos += measureWidth * zoom;
    }
    
    // Draw final barline
    g.drawLine(xPos, area.getY(), xPos, area.getBottom(), 2.0f);
}

void ScoreRenderer::renderMeasure(juce::Graphics& g, juce::Rectangle<float> area, const Measure& measure, int measureIndex)
{
    MeasureLayout layout;
    layout.bounds = area;
    
    float noteSpacing = (area.getWidth() - 20.0f * zoom) / juce::jmax(1, measure.getNoteCount());
    float xPos = area.getX() + 10.0f * zoom;
    float yCenter = area.getY() + area.getHeight() / 2.0f;
    
    // Render time signature on first measure
    if (measureIndex == 0)
    {
        auto ts = measure.getTimeSignature();
        g.setFont(juce::Font(14.0f * zoom, juce::Font::bold));
        g.drawText(juce::String(ts.numerator), xPos, yCenter - 20.0f * zoom, 20.0f * zoom, 15.0f * zoom, 
                   juce::Justification::centred);
        g.drawText(juce::String(ts.denominator), xPos, yCenter + 5.0f * zoom, 20.0f * zoom, 15.0f * zoom,
                   juce::Justification::centred);
        xPos += 30.0f * zoom;
    }
    
    // Render notes
    for (int i = 0; i < measure.getNoteCount(); ++i)
    {
        const Note& note = measure.getNotes()[i];
        juce::Point<float> notePos(xPos, yCenter);
        
        if (note.isRest())
            renderRest(g, notePos, note);
        else
            renderNote(g, notePos, note, measure.getKeySignature());
        
        juce::Rectangle<float> noteBounds(xPos - 10.0f * zoom, yCenter - 20.0f * zoom, 
                                         20.0f * zoom, 40.0f * zoom);
        layout.noteBounds.push_back(noteBounds);
        
        xPos += noteSpacing;
    }
    
    measureLayouts.push_back(layout);
}

void ScoreRenderer::renderNote(juce::Graphics& g, juce::Point<float> position, const Note& note, const KeySignature& keySig)
{
    int yOffset = getNoteY(note.getPitch());
    float noteY = position.y + yOffset * staffLineSpacing * zoom / 2.0f;
    
    // Draw notehead
    float noteheadRadius = 5.0f * zoom;
    
    // Filled or hollow based on duration
    if (note.getDuration() == NoteDuration::Whole || note.getDuration() == NoteDuration::Half)
    {
        g.setColour(juce::Colour(0xff1e1e1e));
        g.fillEllipse(position.x - noteheadRadius, noteY - noteheadRadius, 
                     noteheadRadius * 2, noteheadRadius * 2);
        g.setColour(juce::Colours::white);
        g.drawEllipse(position.x - noteheadRadius, noteY - noteheadRadius,
                     noteheadRadius * 2, noteheadRadius * 2, 1.5f);
    }
    else
    {
        g.setColour(juce::Colours::white);
        g.fillEllipse(position.x - noteheadRadius, noteY - noteheadRadius,
                     noteheadRadius * 2, noteheadRadius * 2);
    }
    
    // Draw stem (except for whole notes)
    if (note.getDuration() != NoteDuration::Whole)
    {
        float stemHeight = 35.0f * zoom;
        bool stemUp = (note.getPitch() >= 71); // B4 and above: stem down
        
        if (stemUp)
        {
            g.drawLine(position.x + noteheadRadius, noteY,
                      position.x + noteheadRadius, noteY - stemHeight, 1.5f);
        }
        else
        {
            g.drawLine(position.x - noteheadRadius, noteY,
                      position.x - noteheadRadius, noteY + stemHeight, 1.5f);
        }
        
        // Draw flags for 8th and 16th notes
        if (note.getDuration() == NoteDuration::Eighth || note.getDuration() == NoteDuration::Sixteenth)
        {
            float flagX = stemUp ? position.x + noteheadRadius : position.x - noteheadRadius;
            float flagY = stemUp ? noteY - stemHeight : noteY + stemHeight;
            
            juce::Path flag;
            flag.startNewSubPath(flagX, flagY);
            flag.lineTo(flagX + 8.0f * zoom, flagY + 10.0f * zoom);
            g.strokePath(flag, juce::PathStrokeType(2.0f));
            
            if (note.getDuration() == NoteDuration::Sixteenth)
            {
                juce::Path flag2;
                flag2.startNewSubPath(flagX, flagY + 5.0f * zoom);
                flag2.lineTo(flagX + 8.0f * zoom, flagY + 15.0f * zoom);
                g.strokePath(flag2, juce::PathStrokeType(2.0f));
            }
        }
    }
    
    // Draw dot if dotted
    if (note.isDotted())
    {
        g.fillEllipse(position.x + 10.0f * zoom, noteY - 2.0f * zoom, 3.0f * zoom, 3.0f * zoom);
    }
    
    // Draw accidental (simplified)
    int accidental = note.getAccidental(keySig.sharps);
    if (accidental != 0)
    {
        g.setFont(juce::Font(16.0f * zoom));
        juce::String accidentalText = accidental > 0 ? "#" : "b";
        g.drawText(accidentalText, position.x - 15.0f * zoom, noteY - 8.0f * zoom, 
                  10.0f * zoom, 16.0f * zoom, juce::Justification::centred);
    }
    
    // Draw tie
    if (note.isTied())
    {
        juce::Path tie;
        tie.startNewSubPath(position.x + noteheadRadius, noteY + 10.0f * zoom);
        tie.quadraticTo(position.x + 20.0f * zoom, noteY + 20.0f * zoom,
                       position.x + 40.0f * zoom, noteY + 10.0f * zoom);
        g.strokePath(tie, juce::PathStrokeType(1.5f));
    }
}

void ScoreRenderer::renderRest(juce::Graphics& g, juce::Point<float> position, const Note& note)
{
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(20.0f * zoom));
    
    juce::String restSymbol;
    switch (note.getDuration())
    {
        case NoteDuration::Whole:     restSymbol = "-"; break;
        case NoteDuration::Half:      restSymbol = "-"; break;
        case NoteDuration::Quarter:   restSymbol = "𝄽"; break;
        case NoteDuration::Eighth:    restSymbol = "𝄾"; break;
        case NoteDuration::Sixteenth: restSymbol = "𝄿"; break;
    }
    
    g.drawText(restSymbol, position.x - 10.0f * zoom, position.y - 10.0f * zoom,
              20.0f * zoom, 20.0f * zoom, juce::Justification::centred);
}

void ScoreRenderer::renderTabStaff(juce::Graphics& g, juce::Rectangle<float> area, const Track& track, int measureStart, int measureEnd)
{
    float xPos = area.getX();
    float yCenter = area.getY() + area.getHeight() / 2.0f;
    
    int stringCount = track.getStringCount();
    float stringSpacing = area.getHeight() / juce::jmax(1, stringCount - 1);
    
    // Draw tab lines
    g.setColour(juce::Colour(0xff666666));
    for (int i = 0; i < stringCount; ++i)
    {
        float y = area.getY() + i * stringSpacing;
        g.drawLine(xPos, y, xPos + (measureEnd - measureStart) * measureWidth * zoom, y, 1.0f);
    }
    
    // Draw measures
    g.setColour(juce::Colours::white);
    for (int i = measureStart; i < measureEnd; ++i)
    {
        const Measure* measure = const_cast<Track&>(track).getMeasureAt(i);
        if (!measure)
            continue;
        
        juce::Rectangle<float> measureBounds(xPos, area.getY(), measureWidth * zoom, area.getHeight());
        
        // Draw barline
        g.drawLine(measureBounds.getX(), measureBounds.getY(),
                  measureBounds.getX(), measureBounds.getBottom(), 2.0f);
        
        // Render tab notes
        float noteSpacing = (measureBounds.getWidth() - 20.0f * zoom) / juce::jmax(1, measure->getNoteCount());
        float noteX = measureBounds.getX() + 10.0f * zoom;
        
        for (int j = 0; j < measure->getNoteCount(); ++j)
        {
            const Note& note = measure->getNotes()[j];
            if (!note.isRest() && note.getString() >= 0)
            {
                int stringNum = note.getString();
                float y = area.getY() + stringNum * stringSpacing;
                renderTabNote(g, juce::Point<float>(noteX, y), note, stringNum);
            }
            noteX += noteSpacing;
        }
        
        xPos += measureWidth * zoom;
    }
    
    // Draw final barline
    g.drawLine(xPos, area.getY(), xPos, area.getBottom(), 2.0f);
}

void ScoreRenderer::renderTabNote(juce::Graphics& g, juce::Point<float> position, const Note& note, int stringNumber)
{
    juce::ignoreUnused(stringNumber);
    
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(12.0f * zoom, juce::Font::bold));
    
    juce::String fretText = juce::String(note.getFret());
    g.drawText(fretText, position.x - 8.0f * zoom, position.y - 8.0f * zoom,
              16.0f * zoom, 16.0f * zoom, juce::Justification::centred);
}

void ScoreRenderer::renderCaret(juce::Graphics& g)
{
    if (caretMeasure < 0 || caretMeasure >= static_cast<int>(measureLayouts.size()))
        return;
    
    const auto& measureLayout = measureLayouts[caretMeasure];
    
    float x = measureLayout.bounds.getX() + 5.0f * zoom;
    if (caretNote >= 0 && caretNote < static_cast<int>(measureLayout.noteBounds.size()))
    {
        x = measureLayout.noteBounds[caretNote].getX();
    }
    
    g.setColour(juce::Colours::red.withAlpha(0.7f));
    g.drawLine(x, measureLayout.bounds.getY(), x, measureLayout.bounds.getBottom(), 2.0f);
}

int ScoreRenderer::getNoteY(int pitch) const
{
    // Middle C (60) is on the first ledger line below the staff
    // Staff center is 71 (B4)
    int middleC = 60;
    int staffCenter = 71;
    
    int offset = pitch - staffCenter;
    return offset;
}

float ScoreRenderer::getNoteWidth(const Note& note) const
{
    juce::ignoreUnused(note);
    return 20.0f * zoom;
}

juce::Rectangle<float> ScoreRenderer::getMeasureBounds(int measureIndex) const
{
    if (measureIndex >= 0 && measureIndex < static_cast<int>(measureLayouts.size()))
        return measureLayouts[measureIndex].bounds;
    return {};
}

juce::Rectangle<float> ScoreRenderer::getNoteBounds(int measureIndex, int noteIndex) const
{
    if (measureIndex >= 0 && measureIndex < static_cast<int>(measureLayouts.size()))
    {
        const auto& layout = measureLayouts[measureIndex];
        if (noteIndex >= 0 && noteIndex < static_cast<int>(layout.noteBounds.size()))
            return layout.noteBounds[noteIndex];
    }
    return {};
}
