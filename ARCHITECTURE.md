# Broken Arrow Tab & Score Studio - Architecture Diagram

## Application Component Hierarchy

```
BrokenArrowTabStudioApplication (JUCE Application)
│
└── MainWindow (DocumentWindow)
    ├── CustomLookAndFeel (Dark Theme)
    ├── Score (Data Model)
    ├── CommandStack (Undo/Redo)
    ├── PlaybackEngine (MIDI Playback)
    │   └── Synthesiser
    │
    └── MainComponent (Content Component)
        ├── ToolbarComponent
        │   ├── File Buttons (New, Open, Save)
        │   ├── Duration Buttons (Whole, Half, Quarter, 8th, 16th)
        │   ├── Rest Toggle
        │   └── Playback Controls (Play, Stop, Metronome)
        │
        ├── TrackPanel (Left Sidebar)
        │   ├── ListBox (Track List)
        │   ├── Add Track Button
        │   └── Remove Track Button
        │
        ├── Viewport (Center)
        │   └── ScoreEditor
        │       └── ScoreRenderer
        │           ├── Staff Rendering
        │           └── Tab Rendering
        │
        ├── InspectorPanel (Right Sidebar)
        │   └── Property Labels
        │       ├── Pitch
        │       ├── Duration
        │       ├── Velocity
        │       ├── String
        │       ├── Fret
        │       └── Tied
        │
        └── StatusBar (Bottom)
            ├── Track Info
            ├── Position Info
            └── Duration Info
```

## Data Model Structure

```
Score
├── Metadata (Title, Artist, Album)
├── Tempo Map (Default BPM + Changes)
├── Default Time Signature
├── Default Key Signature
│
└── Tracks (Vector)
    ├── Track 1
    │   ├── Name (String)
    │   ├── Instrument Type
    │   ├── MIDI Channel/Program
    │   ├── Tab Settings (Tuning, String Count)
    │   │
    │   └── Measures (Vector)
    │       ├── Measure 1
    │       │   ├── Time Signature
    │       │   ├── Key Signature
    │       │   │
    │       │   └── Notes (Vector)
    │       │       ├── Note 1
    │       │       │   ├── Pitch (MIDI 0-127)
    │       │       │   ├── Duration (Enum)
    │       │       │   ├── Dotted (Bool)
    │       │       │   ├── Rest (Bool)
    │       │       │   ├── Tied (Bool)
    │       │       │   ├── Articulation (Enum)
    │       │       │   ├── Velocity (0-127)
    │       │       │   ├── String (-1 or 0-5)
    │       │       │   └── Fret (-1 or 0-24)
    │       │       ├── Note 2
    │       │       └── Note N...
    │       ├── Measure 2
    │       └── Measure N...
    │
    ├── Track 2
    └── Track N...
```

## Command Pattern Implementation

```
User Action
    ↓
MainWindow/UI Component
    ↓
Create Command
    ↓
CommandStack.execute(command)
    ↓
Command.execute()
    ↓
Modify Data Model
    ↓
UI Repaint
    ↓
[Command stored in history]

Undo Request
    ↓
CommandStack.undo()
    ↓
Command.undo()
    ↓
Restore Previous State
    ↓
UI Repaint
```

## Event Flow Examples

### Adding a Note

```
1. User presses 'C' key
   ↓
2. ScoreEditor::keyPressed(key)
   ↓
3. Create AddNoteCommand
   ↓
4. CommandStack::execute(command)
   ↓
5. Command adds note to Measure
   ↓
6. ScoreEditor::repaint()
   ↓
7. ScoreRenderer draws new note
   ↓
8. InspectorPanel updates (listener callback)
```

### Playback

```
1. User clicks Play button
   ↓
2. ToolbarComponent fires listener callback
   ↓
3. MainWindow::playButtonClicked()
   ↓
4. PlaybackEngine::play()
   ↓
5. Timer starts (60 fps)
   ↓
6. Each timer tick:
   - Calculate current measure/beat
   - Trigger MIDI notes
   - Play metronome click
   - Notify listeners
   ↓
7. MainWindow receives position update
   ↓
8. ScoreEditor updates caret position
   ↓
9. Repaint with new caret
```

### Save/Load

```
Save:
1. User: File → Save
   ↓
2. MainWindow::saveScore()
   ↓
3. ScoreSerializer::saveToFile()
   ↓
4. Score::toVar() (recursive)
   ↓
5. JSON::toString()
   ↓
6. File::replaceWithText()

Load:
1. User: File → Open
   ↓
2. FileChooser dialog
   ↓
3. MainWindow::openScore()
   ↓
4. ScoreSerializer::loadFromFile()
   ↓
5. File::loadFileAsString()
   ↓
6. JSON::parse()
   ↓
7. Score::fromVar() (recursive)
   ↓
8. Update UI with new score
```

## Rendering Pipeline

```
ScoreEditor::paint(g)
    ↓
ScoreRenderer::render(g, bounds)
    ↓
For each track:
    renderStaff()
    ├── Draw staff lines (5 horizontal)
    ├── Draw barlines (vertical)
    │
    └── For each measure:
        renderMeasure()
        ├── Draw time signature (first measure)
        │
        └── For each note:
            renderNote() or renderRest()
            ├── Calculate Y position from pitch
            ├── Draw notehead (filled/hollow)
            ├── Draw stem
            ├── Draw flags (8th, 16th)
            ├── Draw accidental (#, b, ♮)
            ├── Draw dot (if dotted)
            └── Draw tie (if tied)
    │
    renderTabStaff() (if enabled)
    ├── Draw tab lines (6 horizontal)
    ├── Draw barlines
    │
    └── For each measure:
        └── For each note:
            renderTabNote()
            └── Draw fret number at string position
    │
    renderCaret()
    └── Draw red vertical line at cursor
```

## Listener Pattern Usage

```
Observer Pattern Connections:

ToolbarComponent ──[Listener]──> MainWindow
    - newButtonClicked()
    - openButtonClicked()
    - saveButtonClicked()
    - durationChanged()
    - restToggled()
    - playButtonClicked()
    - stopButtonClicked()
    - metronomeToggled()

TrackPanel ──[Listener]──> MainWindow
    - selectedTrackChanged()

ScoreEditor ──[Listener]──> MainWindow
    - selectionChanged()

PlaybackEngine ──[Listener]──> MainWindow
    - playbackPositionChanged()

MainWindow acts as central coordinator,
receiving events and coordinating between components.
```

## File Format Flow

```
In-Memory Score Object
    ↓
Score::toVar()
    ├── Convert to juce::var (nested objects/arrays)
    │   ├── Primitives → int, double, bool, String
    │   ├── Objects → DynamicObject
    │   └── Arrays → Array<var>
    ↓
JSON Format (.json)
    └── JSON::toString(var, prettyPrint)
        └── Human-readable text file

MIDI Format (.mid)
    ├── MidiFile object
    ├── For each track:
    │   └── MidiMessageSequence
    │       ├── Program change
    │       ├── Tempo meta event
    │       └── For each note:
    │           ├── Note on (pitch, velocity, time)
    │           └── Note off (pitch, time)
    └── writeTo(FileOutputStream)

MusicXML Format (.musicxml)
    ├── XML header
    ├── Score metadata (title, composer)
    ├── Part list (instruments)
    └── For each part:
        └── For each measure:
            ├── Attributes (time sig, key sig, clef)
            └── For each note:
                ├── Pitch (step, alter, octave)
                ├── Duration
                └── Type (whole, half, quarter...)
```

## Memory Management Strategy

```
Ownership Model:

MainWindow (owns)
    ├── CustomLookAndFeel (member)
    ├── Score (member)
    ├── CommandStack (member)
    ├── PlaybackEngine (member)
    └── MainComponent (unique_ptr)
        ├── ToolbarComponent (member)
        ├── TrackPanel (member)
        ├── InspectorPanel (member)
        ├── ScoreEditor (member)
        ├── Viewport (member)
        └── StatusBar (member)

Data Model (STL containers):
    Score
    └── vector<Track> (owns)
        └── vector<Measure> (owns)
            └── vector<Note> (owns)

Command Stack:
    vector<unique_ptr<Command>> (owns)

All pointers use:
    - unique_ptr for sole ownership
    - Raw pointers for references (non-owning)
    - No manual new/delete in user code
```

## Thread Safety

```
Main Thread (UI Thread):
    ├── All UI components
    ├── ScoreRenderer
    ├── ScoreEditor
    └── User input handling

Timer Thread (PlaybackEngine):
    ├── Playback position calculation
    ├── MIDI event triggering
    └── Listener callbacks (dispatched to main thread)

Audio Thread (JUCE):
    └── Synthesiser rendering (future enhancement)

Note: Current implementation is single-threaded.
All operations on main/message thread.
```

## Build System Flow

```
Source Code (.cpp, .h)
    ↓
BrokenArrowTabStudio.jucer
    ├── Project settings
    ├── File list
    ├── Module configuration
    └── Exporter settings
    ↓
Projucer
    ├── Generates JuceLibraryCode/
    │   ├── AppConfig.h
    │   ├── JuceHeader.h
    │   └── Module wrappers
    │
    └── Generates Builds/VisualStudio2022/
        ├── BrokenArrowTabStudio.sln
        └── BrokenArrowTabStudio.vcxproj
        ↓
Visual Studio
    ├── Compile .cpp files → .obj
    ├── Link .obj + JUCE libs → .exe
    │
    └── Output:
        Builds/VisualStudio2022/x64/
        └── [Debug|Release]/App/
            └── BrokenArrowTabStudio.exe
```

## Summary

This architecture provides:

✅ **Separation of Concerns**: Model, View, Controller clearly separated  
✅ **Extensibility**: Easy to add new commands, instruments, exporters  
✅ **Maintainability**: Clean interfaces, minimal coupling  
✅ **Testability**: Components can be tested independently  
✅ **Performance**: Efficient rendering with caching  
✅ **User Experience**: Responsive UI, undo/redo, real-time feedback  
✅ **Reliability**: Proper memory management, no leaks  
✅ **Portability**: JUCE framework supports multiple platforms  

The design follows SOLID principles and industry-standard patterns
for music notation software.
