#include "TrackPanel.h"
#include "../Commands/EditCommands.h"

TrackPanel::TrackPanel(Score& s, CommandStack& cs)
    : score(s), commandStack(cs), trackList("Tracks", this)
{
    addAndMakeVisible(trackList);
    trackList.setRowHeight(24);
    trackList.setColour(juce::ListBox::backgroundColourId, juce::Colour(0xff141414));
    
    addAndMakeVisible(addButton);
    addButton.setButtonText("+");
    addButton.onClick = [this] { addTrack(); };
    
    addAndMakeVisible(removeButton);
    removeButton.setButtonText("-");
    removeButton.onClick = [this] { removeTrack(); };
}

TrackPanel::~TrackPanel()
{
}

void TrackPanel::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e1e));
    
    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("Tracks", 10, 5, getWidth() - 20, 20, juce::Justification::centredLeft);
}

void TrackPanel::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(30);
    
    auto buttonArea = bounds.removeFromBottom(30);
    addButton.setBounds(buttonArea.removeFromLeft(buttonArea.getWidth() / 2).reduced(5));
    removeButton.setBounds(buttonArea.reduced(5));
    
    trackList.setBounds(bounds);
}

int TrackPanel::getNumRows()
{
    return score.getTrackCount();
}

void TrackPanel::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colour(0xff0e639c));
    
    g.setColour(juce::Colours::white);
    g.setFont(12.0f);
    
    Track* track = score.getTrackAt(rowNumber);
    if (track)
    {
        g.drawText(track->getName(), 10, 0, width - 20, height, juce::Justification::centredLeft);
    }
}

void TrackPanel::selectedRowsChanged(int lastRowSelected)
{
    if (lastRowSelected >= 0 && lastRowSelected < score.getTrackCount())
    {
        selectedTrack = lastRowSelected;
        listeners.call([this](Listener& l) {
            l.selectedTrackChanged(selectedTrack);
        });
    }
}

void TrackPanel::addTrack()
{
    Track newTrack("Track " + juce::String(score.getTrackCount() + 1), InstrumentType::AcousticGuitar);
    
    // Add some default measures
    for (int i = 0; i < 4; ++i)
    {
        Measure measure;
        measure.setTimeSignature(score.getDefaultTimeSignature());
        measure.setKeySignature(score.getDefaultKeySignature());
        newTrack.addMeasure(measure);
    }
    
    auto command = std::make_unique<AddTrackCommand>(score, newTrack);
    commandStack.execute(std::move(command));
    
    trackList.updateContent();
    trackList.selectRow(score.getTrackCount() - 1);
}

void TrackPanel::removeTrack()
{
    if (selectedTrack >= 0 && selectedTrack < score.getTrackCount())
    {
        auto command = std::make_unique<RemoveTrackCommand>(score, selectedTrack);
        commandStack.execute(std::move(command));
        
        trackList.updateContent();
        
        if (score.getTrackCount() > 0)
        {
            selectedTrack = juce::jmin(selectedTrack, score.getTrackCount() - 1);
            trackList.selectRow(selectedTrack);
        }
    }
}
