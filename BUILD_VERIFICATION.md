# Build Verification Checklist

Use this checklist when building and testing the application on Windows.

## Prerequisites Setup
- [ ] JUCE 7.0.9 or later installed
- [ ] Visual Studio 2022 or 2026 installed with C++ desktop development workload
- [ ] C++20 support enabled in Visual Studio
- [ ] Projucer executable accessible (from JUCE installation)

## Project Configuration
- [ ] Opened Projucer
- [ ] Set JUCE module path in File → Global Paths
- [ ] Opened BrokenArrowTabStudio.jucer successfully
- [ ] All source files appear in project tree (38 source files)
- [ ] Visual Studio 2022 exporter visible
- [ ] C++ Language Standard set to "C++20"

## Code Generation
- [ ] Clicked "Save Project and Open in IDE" or saved manually
- [ ] Builds/VisualStudio2022 directory created
- [ ] JuceLibraryCode directory created
- [ ] .sln file opens in Visual Studio without errors
- [ ] All .cpp files listed in Solution Explorer
- [ ] JUCE modules properly referenced

## Debug Build
- [ ] Selected "Debug" configuration
- [ ] Selected "x64" platform
- [ ] Build → Build Solution (Ctrl+Shift+B)
- [ ] Build succeeded with 0 errors
- [ ] Only minor warnings (JUCE-related acceptable)
- [ ] BrokenArrowTabStudio.exe created in Builds/VisualStudio2022/x64/Debug/App/

## Release Build
- [ ] Selected "Release" configuration
- [ ] Selected "x64" platform
- [ ] Build → Rebuild Solution
- [ ] Build succeeded with 0 errors
- [ ] BrokenArrowTabStudio.exe created in Builds/VisualStudio2022/x64/Release/App/

## Application Launch
- [ ] Debug .exe launches without crash
- [ ] Release .exe launches without crash
- [ ] Window appears with title "Broken Arrow Tab & Score Studio - Untitled"
- [ ] Window is resizable
- [ ] Window has minimize, maximize, close buttons

## UI Verification

### Theme & Layout
- [ ] Dark theme applied (dark gray backgrounds)
- [ ] Toolbar visible at top
- [ ] Track panel visible on left (shows "Guitar")
- [ ] Inspector panel visible on right
- [ ] Status bar visible at bottom
- [ ] Score editor visible in center with viewport scrollbars

### Menu Bar
- [ ] File menu exists with items: New, Open, Save, Save As, Export MIDI, Export MusicXML, Quit
- [ ] Edit menu exists with items: Undo, Redo, Cut, Copy, Paste
- [ ] View menu exists with items: Zoom In, Zoom Out
- [ ] Track menu exists with items: Add Measure, Remove Measure
- [ ] Note menu exists
- [ ] Playback menu exists with items: Play, Stop, Metronome
- [ ] Help menu exists with About item

### Toolbar
- [ ] "New" button visible
- [ ] "Open" button visible
- [ ] "Save" button visible
- [ ] Duration buttons visible (Whole, Half, Quarter, 8th, 16th)
- [ ] "Rest" toggle button visible
- [ ] "Play" button visible
- [ ] "Stop" button visible
- [ ] "Metronome" toggle button visible (checked by default)

### Score Display
- [ ] Staff lines visible (5 horizontal lines)
- [ ] Tab staff visible below (6 horizontal lines)
- [ ] Barlines visible (vertical lines)
- [ ] 4 measures visible initially
- [ ] Time signature "4/4" visible in first measure

## Functional Testing

### File Operations
- [ ] New: Creates blank score
- [ ] Save As: Opens file chooser, saves .json file
- [ ] Save: Saves to current file
- [ ] Open: Opens file chooser, loads .json file
- [ ] Opened file displays correctly
- [ ] Window title updates with filename

### Note Entry
- [ ] Click in measure positions cursor
- [ ] Press 'c' adds note at cursor
- [ ] Press 'd' adds note at cursor
- [ ] Press 'e' adds note at cursor
- [ ] Press '0'-'9' adds notes
- [ ] Note appears on staff as notehead with stem
- [ ] Note appears on tab with fret number

### Note Selection & Editing
- [ ] Click note selects it (visible in inspector)
- [ ] Inspector shows note properties (pitch, duration, velocity, string, fret)
- [ ] Delete key removes selected note
- [ ] Backspace key removes selected note
- [ ] Arrow keys navigate between notes

### Duration Changes
- [ ] Click "Quarter" button, add note → quarter note created
- [ ] Click "Half" button, add note → half note created (hollow)
- [ ] Click "Whole" button, add note → whole note created (hollow, no stem)
- [ ] Click "8th" button, add note → eighth note created (with flag)
- [ ] Click "16th" button, add note → sixteenth note created (with two flags)

### Rest Mode
- [ ] Click "Rest" toggle (should highlight)
- [ ] Add note → rest symbol appears instead
- [ ] Click "Rest" again (should unhighlight)
- [ ] Add note → regular note appears

### Undo/Redo
- [ ] Add several notes
- [ ] Edit → Undo (Ctrl+Z) removes last note
- [ ] Undo multiple times works
- [ ] Edit → Redo (Ctrl+Y) restores note
- [ ] Redo multiple times works
- [ ] Undo disabled when nothing to undo
- [ ] Redo disabled when nothing to redo

### Track Management
- [ ] Click "+" in track panel adds new track
- [ ] New track appears in list
- [ ] Click track name selects track
- [ ] Score editor switches to selected track
- [ ] Click "-" removes selected track
- [ ] Remaining tracks still work

### Measure Management
- [ ] Track → Add Measure adds measure to current track
- [ ] New measure appears in score editor
- [ ] New measure is empty
- [ ] Track → Remove Measure removes measure at cursor

### Zoom
- [ ] View → Zoom In enlarges score
- [ ] View → Zoom Out shrinks score
- [ ] Scrollbars appear/adjust with zoom
- [ ] Can zoom multiple times in/out

### Playback
- [ ] Click "Play" button starts playback
- [ ] Caret moves through measures during playback
- [ ] Click "Stop" button stops playback
- [ ] Caret returns to start after stop
- [ ] Metronome toggle works (checkbox state)
- [ ] Can hear metronome clicks when enabled (if audio working)

### Inspector Panel
- [ ] Select note → Inspector shows pitch name (e.g., "C4")
- [ ] Inspector shows duration (e.g., "Quarter")
- [ ] Inspector shows velocity number
- [ ] Inspector shows string number
- [ ] Inspector shows fret number
- [ ] Inspector shows tie status (Yes/No)
- [ ] No selection → Inspector shows "-"

### Status Bar
- [ ] Shows current track name
- [ ] Shows position (Measure #, Beat #)
- [ ] Shows selected duration
- [ ] Updates when track changes
- [ ] Updates when position changes

## Export Testing

### MIDI Export
- [ ] File → Export MIDI opens file chooser
- [ ] Enter filename "test.mid", save
- [ ] File created on disk
- [ ] File size > 0 bytes
- [ ] Open .mid file in DAW (e.g., Reaper, Ableton)
- [ ] MIDI file contains notes
- [ ] Notes play in DAW

### MusicXML Export
- [ ] File → Export MusicXML opens file chooser
- [ ] Enter filename "test.musicxml", save
- [ ] File created on disk
- [ ] File size > 0 bytes
- [ ] Open .musicxml file in text editor → valid XML
- [ ] Open .musicxml file in MuseScore/Finale
- [ ] Score displays correctly

## Advanced Testing

### Complex Score
- [ ] Add 3 tracks
- [ ] Add 8 measures to each
- [ ] Add various notes (different durations)
- [ ] Add rests
- [ ] Save as "complex.json"
- [ ] Close and reopen file
- [ ] All tracks, measures, notes preserved

### Long Session
- [ ] Work for 10+ minutes without crash
- [ ] Perform 50+ undo/redo operations
- [ ] Create, save, load multiple files
- [ ] No memory leaks (check Task Manager)

### Edge Cases
- [ ] Create score with 0 tracks (via remove all) → handles gracefully
- [ ] Create measure with 0 notes → displays empty
- [ ] Zoom to minimum (0.5x) → readable
- [ ] Zoom to maximum (3.0x) → readable
- [ ] Resize window to minimum → UI still functional
- [ ] Resize window to maximum → UI scales properly

## Performance Check
- [ ] UI responds instantly to clicks
- [ ] Rendering is smooth (no lag)
- [ ] Scrolling is smooth
- [ ] Playback is smooth (if audio works)
- [ ] No significant CPU usage when idle
- [ ] Memory usage reasonable (<200 MB)

## Error Handling
- [ ] Try to open non-existent file → error message shown
- [ ] Try to open invalid JSON → error message shown
- [ ] Unsaved changes on quit → prompts to save
- [ ] Cancel save dialog → operation cancelled
- [ ] Export to invalid path → error message shown

## Visual Quality
- [ ] Noteheads are round and clear
- [ ] Stems are straight and connected
- [ ] Staff lines are evenly spaced
- [ ] Barlines are vertical
- [ ] Text is readable (buttons, labels, inspector)
- [ ] Colors are consistent (dark theme)
- [ ] No visual glitches or artifacts
- [ ] Anti-aliasing is visible (smooth edges)

## Known Limitations (Expected Behavior)
- [ ] Playback may be silent if no audio samples loaded (this is expected)
- [ ] 8th/16th notes show flags, not beams (by design)
- [ ] Single voice per measure (no polyphony)
- [ ] Basic accidental display (sharp/flat only)
- [ ] No print/PDF export (use MusicXML for external rendering)

## Final Verification
- [ ] All core features working
- [ ] No crashes encountered
- [ ] No data loss during save/load
- [ ] Performance acceptable
- [ ] UI is polished and consistent
- [ ] Application ready for use

## Screenshot Documentation (Optional)
- [ ] Main window at startup
- [ ] Score with notes entered
- [ ] Inspector showing note properties
- [ ] Multiple tracks
- [ ] Export dialog
- [ ] About dialog

## Issues to Report
If any checklist items fail, document:
1. Which item failed
2. Steps to reproduce
3. Expected behavior
4. Actual behavior
5. Error messages (if any)
6. Visual Studio build output (if build fails)

## Sign-Off
- [ ] Build verification complete
- [ ] Application tested and working
- [ ] Ready for production use

**Verified by**: ___________________  
**Date**: ___________________  
**Build Configuration**: Debug ☐  Release ☐  
**Platform**: Windows x64 ☐  
**JUCE Version**: ___________________  
**Visual Studio Version**: ___________________  

---

## Notes

Use this space to document any issues, observations, or suggestions:

