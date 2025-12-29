#pragma once
#include <JuceHeader.h>
#include "Model/Score.h"
#include "Commands/CommandStack.h"
#include "Editor/ScoreEditor.h"
#include "Playback/PlaybackEngine.h"
#include "Serialization/ScoreSerializer.h"
#include "UI/CustomLookAndFeel.h"
#include "UI/TrackPanel.h"
#include "UI/InspectorPanel.h"
#include "UI/ToolbarComponent.h"
#include "UI/StatusBar.h"

class MainWindow : public juce::DocumentWindow,
                   public juce::MenuBarModel,
                   public ToolbarComponent::Listener,
                   public TrackPanel::Listener,
                   public ScoreEditor::Listener,
                   public PlaybackEngine::Listener
{
public:
    MainWindow(juce::String name);
    ~MainWindow() override;
    
    void closeButtonPressed() override;
    
    // MenuBarModel
    juce::StringArray getMenuBarNames() override;
    juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName) override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;
    
    // ToolbarComponent::Listener
    void newButtonClicked() override;
    void openButtonClicked() override;
    void saveButtonClicked() override;
    void durationChanged(NoteDuration duration) override;
    void restToggled(bool enabled) override;
    void playButtonClicked() override;
    void stopButtonClicked() override;
    void metronomeToggled(bool enabled) override;
    
    // TrackPanel::Listener
    void selectedTrackChanged(int trackIndex) override;
    
    // ScoreEditor::Listener
    void selectionChanged(int trackIndex, int measureIndex, int noteIndex) override;
    
    // PlaybackEngine::Listener
    void playbackPositionChanged(int measureIndex, double beat) override;
    
private:
    void createNewScore();
    void openScore();
    void saveScore();
    void saveScoreAs();
    void exportMidi();
    void exportMusicXML();
    
    void undo();
    void redo();
    void addMeasure();
    void removeMeasure();
    
    void updateTitle();
    
    CustomLookAndFeel customLookAndFeel;
    
    Score score;
    CommandStack commandStack;
    PlaybackEngine playbackEngine;
    
    juce::File currentFile;
    bool hasUnsavedChanges = false;
    
    class MainComponent : public juce::Component
    {
    public:
        MainComponent(Score& score, CommandStack& commandStack);
        ~MainComponent() override;
        
        void resized() override;
        
        TrackPanel trackPanel;
        InspectorPanel inspectorPanel;
        ToolbarComponent toolbar;
        ScoreEditor scoreEditor;
        juce::Viewport scoreViewport;
        StatusBar statusBar;
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
    };
    
    std::unique_ptr<MainComponent> mainComponent;
    
    enum MenuItemIDs
    {
        menuNew = 1,
        menuOpen,
        menuSave,
        menuSaveAs,
        menuExportMidi,
        menuExportMusicXML,
        menuQuit,
        menuUndo,
        menuRedo,
        menuCut,
        menuCopy,
        menuPaste,
        menuAddMeasure,
        menuRemoveMeasure,
        menuZoomIn,
        menuZoomOut,
        menuPlay,
        menuStop,
        menuMetronome
    };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};
