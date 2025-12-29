#include "MainWindow.h"

// MainComponent implementation
MainWindow::MainComponent::MainComponent(Score& score, CommandStack& commandStack)
    : trackPanel(score, commandStack)
    , inspectorPanel(score)
    , scoreEditor(score, commandStack)
{
    addAndMakeVisible(toolbar);
    addAndMakeVisible(trackPanel);
    addAndMakeVisible(inspectorPanel);
    addAndMakeVisible(scoreViewport);
    addAndMakeVisible(statusBar);
    
    scoreViewport.setViewedComponent(&scoreEditor, false);
    scoreViewport.setScrollBarsShown(true, true);
    
    setSize(1200, 800);
}

MainWindow::MainComponent::~MainComponent()
{
}

void MainWindow::MainComponent::resized()
{
    auto bounds = getLocalBounds();
    
    toolbar.setBounds(bounds.removeFromTop(50));
    statusBar.setBounds(bounds.removeFromBottom(25));
    
    auto leftPanel = bounds.removeFromLeft(200);
    trackPanel.setBounds(leftPanel);
    
    auto rightPanel = bounds.removeFromRight(200);
    inspectorPanel.setBounds(rightPanel);
    
    scoreViewport.setBounds(bounds);
    scoreEditor.setSize(juce::jmax(bounds.getWidth(), scoreEditor.getWidth()), 
                       juce::jmax(bounds.getHeight(), 600));
}

// MainWindow implementation
MainWindow::MainWindow(juce::String name)
    : DocumentWindow(name, juce::Colours::darkgrey, DocumentWindow::allButtons)
    , playbackEngine(score)
{
    setUsingNativeTitleBar(true);
    setLookAndFeel(&customLookAndFeel);
    
    createNewScore();
    
    mainComponent = std::make_unique<MainComponent>(score, commandStack);
    setContentOwned(mainComponent.get(), true);
    
    setMenuBar(this);
    
    // Connect listeners
    mainComponent->toolbar.addListener(this);
    mainComponent->trackPanel.addListener(this);
    mainComponent->scoreEditor.addListener(this);
    playbackEngine.addListener(this);
    
    centreWithSize(getWidth(), getHeight());
    setVisible(true);
    setResizable(true, true);
    
    updateTitle();
}

MainWindow::~MainWindow()
{
    setMenuBar(nullptr);
    setLookAndFeel(nullptr);
}

void MainWindow::closeButtonPressed()
{
    if (hasUnsavedChanges)
    {
        int result = juce::AlertWindow::showYesNoCancelBox(
            juce::AlertWindow::QuestionIcon,
            "Unsaved Changes",
            "Do you want to save your changes?",
            "Save", "Don't Save", "Cancel");
        
        if (result == 0) // Cancel
            return;
        else if (result == 1) // Save
            saveScore();
    }
    
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}

juce::StringArray MainWindow::getMenuBarNames()
{
    return {"File", "Edit", "View", "Track", "Note", "Playback", "Help"};
}

juce::PopupMenu MainWindow::getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName)
{
    juce::PopupMenu menu;
    
    if (menuName == "File")
    {
        menu.addItem(menuNew, "New", true);
        menu.addItem(menuOpen, "Open...", true);
        menu.addItem(menuSave, "Save", true);
        menu.addItem(menuSaveAs, "Save As...", true);
        menu.addSeparator();
        menu.addItem(menuExportMidi, "Export MIDI...", true);
        menu.addItem(menuExportMusicXML, "Export MusicXML...", true);
        menu.addSeparator();
        menu.addItem(menuQuit, "Quit", true);
    }
    else if (menuName == "Edit")
    {
        menu.addItem(menuUndo, "Undo", commandStack.canUndo());
        menu.addItem(menuRedo, "Redo", commandStack.canRedo());
        menu.addSeparator();
        menu.addItem(menuCut, "Cut", false);
        menu.addItem(menuCopy, "Copy", false);
        menu.addItem(menuPaste, "Paste", false);
    }
    else if (menuName == "View")
    {
        menu.addItem(menuZoomIn, "Zoom In", true);
        menu.addItem(menuZoomOut, "Zoom Out", true);
    }
    else if (menuName == "Track")
    {
        menu.addItem(menuAddMeasure, "Add Measure", true);
        menu.addItem(menuRemoveMeasure, "Remove Measure", true);
    }
    else if (menuName == "Playback")
    {
        menu.addItem(menuPlay, "Play", !playbackEngine.isPlaying());
        menu.addItem(menuStop, "Stop", playbackEngine.isPlaying());
        menu.addSeparator();
        menu.addItem(menuMetronome, "Metronome", true, playbackEngine.isMetronomeEnabled());
    }
    else if (menuName == "Help")
    {
        menu.addItem(1000, "About", true);
    }
    
    juce::ignoreUnused(topLevelMenuIndex);
    return menu;
}

void MainWindow::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
    juce::ignoreUnused(topLevelMenuIndex);
    
    switch (menuItemID)
    {
        case menuNew: createNewScore(); break;
        case menuOpen: openScore(); break;
        case menuSave: saveScore(); break;
        case menuSaveAs: saveScoreAs(); break;
        case menuExportMidi: exportMidi(); break;
        case menuExportMusicXML: exportMusicXML(); break;
        case menuQuit: closeButtonPressed(); break;
        case menuUndo: undo(); break;
        case menuRedo: redo(); break;
        case menuAddMeasure: addMeasure(); break;
        case menuRemoveMeasure: removeMeasure(); break;
        case menuZoomIn: mainComponent->scoreEditor.setZoom(mainComponent->scoreEditor.getZoom() * 1.2f); break;
        case menuZoomOut: mainComponent->scoreEditor.setZoom(mainComponent->scoreEditor.getZoom() / 1.2f); break;
        case menuPlay: playButtonClicked(); break;
        case menuStop: stopButtonClicked(); break;
        case menuMetronome: 
            playbackEngine.setMetronomeEnabled(!playbackEngine.isMetronomeEnabled()); 
            mainComponent->toolbar.setMetronomeEnabled(playbackEngine.isMetronomeEnabled());
            break;
        case 1000: // About
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                "About Broken Arrow Tab & Score Studio",
                "Version 1.0\n\nA JUCE-based music notation and tablature editor.");
            break;
    }
}

void MainWindow::newButtonClicked()
{
    createNewScore();
}

void MainWindow::openButtonClicked()
{
    openScore();
}

void MainWindow::saveButtonClicked()
{
    saveScore();
}

void MainWindow::durationChanged(NoteDuration duration)
{
    mainComponent->scoreEditor.setCurrentDuration(duration);
    
    juce::String durText;
    switch (duration)
    {
        case NoteDuration::Whole: durText = "Whole"; break;
        case NoteDuration::Half: durText = "Half"; break;
        case NoteDuration::Quarter: durText = "Quarter"; break;
        case NoteDuration::Eighth: durText = "Eighth"; break;
        case NoteDuration::Sixteenth: durText = "16th"; break;
    }
    mainComponent->statusBar.setDurationInfo(durText);
}

void MainWindow::restToggled(bool enabled)
{
    mainComponent->scoreEditor.setRestMode(enabled);
}

void MainWindow::playButtonClicked()
{
    playbackEngine.play();
}

void MainWindow::stopButtonClicked()
{
    playbackEngine.stop();
}

void MainWindow::metronomeToggled(bool enabled)
{
    playbackEngine.setMetronomeEnabled(enabled);
}

void MainWindow::selectedTrackChanged(int trackIndex)
{
    mainComponent->scoreEditor.setActiveTrack(trackIndex);
    
    Track* track = score.getTrackAt(trackIndex);
    if (track)
    {
        mainComponent->statusBar.setTrackInfo(track->getName());
    }
}

void MainWindow::selectionChanged(int trackIndex, int measureIndex, int noteIndex)
{
    mainComponent->inspectorPanel.setSelection(trackIndex, measureIndex, noteIndex);
    mainComponent->statusBar.setPositionInfo(measureIndex, 0);
}

void MainWindow::playbackPositionChanged(int measureIndex, double beat)
{
    juce::ignoreUnused(beat);
    mainComponent->scoreEditor.setCaretPosition(measureIndex, 0);
}

void MainWindow::createNewScore()
{
    score = Score();
    score.setTitle("Untitled");
    
    // Create default guitar track with 4 measures
    Track guitarTrack("Guitar", InstrumentType::AcousticGuitar);
    guitarTrack.setStandardGuitarTuning();
    
    for (int i = 0; i < 4; ++i)
    {
        Measure measure;
        measure.setTimeSignature(score.getDefaultTimeSignature());
        measure.setKeySignature(score.getDefaultKeySignature());
        guitarTrack.addMeasure(measure);
    }
    
    score.addTrack(guitarTrack);
    
    commandStack.clear();
    currentFile = juce::File();
    hasUnsavedChanges = false;
    
    if (mainComponent)
    {
        mainComponent->trackPanel.addListener(this);
        mainComponent->scoreEditor.setActiveTrack(0);
        mainComponent->scoreEditor.repaint();
    }
    
    updateTitle();
}

void MainWindow::openScore()
{
    juce::FileChooser chooser("Open Score", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
                             "*.json");
    
    if (chooser.browseForFileToOpen())
    {
        juce::File file = chooser.getResult();
        
        if (ScoreSerializer::loadFromFile(score, file))
        {
            currentFile = file;
            hasUnsavedChanges = false;
            commandStack.clear();
            
            if (mainComponent)
            {
                mainComponent->scoreEditor.setActiveTrack(0);
                mainComponent->scoreEditor.repaint();
            }
            
            updateTitle();
        }
        else
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                "Error", "Failed to open file.");
        }
    }
}

void MainWindow::saveScore()
{
    if (currentFile == juce::File())
    {
        saveScoreAs();
        return;
    }
    
    if (ScoreSerializer::saveToFile(score, currentFile))
    {
        hasUnsavedChanges = false;
        updateTitle();
    }
    else
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
            "Error", "Failed to save file.");
    }
}

void MainWindow::saveScoreAs()
{
    juce::FileChooser chooser("Save Score As", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
                             "*.json");
    
    if (chooser.browseForFileToSave(true))
    {
        currentFile = chooser.getResult();
        
        if (!currentFile.getFileExtension().equalsIgnoreCase(".json"))
            currentFile = currentFile.withFileExtension(".json");
        
        saveScore();
    }
}

void MainWindow::exportMidi()
{
    juce::FileChooser chooser("Export MIDI", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
                             "*.mid");
    
    if (chooser.browseForFileToSave(true))
    {
        juce::File file = chooser.getResult();
        
        if (!file.getFileExtension().equalsIgnoreCase(".mid"))
            file = file.withFileExtension(".mid");
        
        if (ScoreSerializer::exportToMidi(score, file))
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                "Success", "MIDI file exported successfully.");
        }
        else
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                "Error", "Failed to export MIDI file.");
        }
    }
}

void MainWindow::exportMusicXML()
{
    juce::FileChooser chooser("Export MusicXML", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
                             "*.xml;*.musicxml");
    
    if (chooser.browseForFileToSave(true))
    {
        juce::File file = chooser.getResult();
        
        if (!file.hasFileExtension(".xml") && !file.hasFileExtension(".musicxml"))
            file = file.withFileExtension(".musicxml");
        
        if (ScoreSerializer::exportToMusicXML(score, file))
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                "Success", "MusicXML file exported successfully.");
        }
        else
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                "Error", "Failed to export MusicXML file.");
        }
    }
}

void MainWindow::undo()
{
    commandStack.undo();
    mainComponent->scoreEditor.repaint();
}

void MainWindow::redo()
{
    commandStack.redo();
    mainComponent->scoreEditor.repaint();
}

void MainWindow::addMeasure()
{
    int trackIndex = mainComponent->trackPanel.getSelectedTrackIndex();
    auto command = std::make_unique<AddMeasureCommand>(score, trackIndex);
    commandStack.execute(std::move(command));
    mainComponent->scoreEditor.repaint();
}

void MainWindow::removeMeasure()
{
    int trackIndex = mainComponent->trackPanel.getSelectedTrackIndex();
    Track* track = score.getTrackAt(trackIndex);
    if (track && track->getMeasureCount() > 0)
    {
        int measureIndex, noteIndex;
        mainComponent->scoreEditor.getCaretPosition(measureIndex, noteIndex);
        
        auto command = std::make_unique<RemoveMeasureCommand>(score, trackIndex, measureIndex);
        commandStack.execute(std::move(command));
        mainComponent->scoreEditor.repaint();
    }
}

void MainWindow::updateTitle()
{
    juce::String title = "Broken Arrow Tab & Score Studio - ";
    
    if (currentFile != juce::File())
        title += currentFile.getFileName();
    else
        title += score.getTitle();
    
    if (hasUnsavedChanges)
        title += " *";
    
    setName(title);
}
