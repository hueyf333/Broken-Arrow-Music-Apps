# Broken Arrow Tab & Score Studio - Quick Reference

## Keyboard Shortcuts

### File Operations
- `Ctrl+N` - New Score
- `Ctrl+O` - Open Score
- `Ctrl+S` - Save Score
- `Ctrl+Shift+S` - Save As
- `Ctrl+Q` - Quit

### Editing
- `Ctrl+Z` - Undo
- `Ctrl+Y` or `Ctrl+Shift+Z` - Redo
- `Delete` or `Backspace` - Delete selected note

### Navigation
- `←` Left Arrow - Move cursor left
- `→` Right Arrow - Move cursor right

### Note Entry
- `A-G` - Add note (A, B, C, D, E, F, G)
- `0-9` - Add note with pitch offset or fret number
- Click **Rest** button first to add rests instead of notes

### View
- Menu: **View** → **Zoom In** - Increase zoom
- Menu: **View** → **Zoom Out** - Decrease zoom

## Mouse Controls

### Score Editor
- **Left Click** - Select note or measure
- **Click in measure** - Position cursor for note entry

### Track Panel
- **Left Click** - Select track
- **+ Button** - Add new track
- **- Button** - Remove selected track

## Toolbar Buttons

### File Operations
- **New** - Create new score
- **Open** - Open existing score (.json)
- **Save** - Save current score

### Note Durations
- **Whole** - Whole note (4 beats in 4/4)
- **Half** - Half note (2 beats)
- **Quarter** - Quarter note (1 beat)
- **8th** - Eighth note (1/2 beat)
- **16th** - Sixteenth note (1/4 beat)

### Note Modifiers
- **Rest** - Toggle rest mode (checkable button)

### Playback
- **Play** - Start playback
- **Stop** - Stop playback
- **Metronome** - Toggle metronome click (checkable button)

## Menu Reference

### File Menu
- **New** - Create new score
- **Open...** - Open score file
- **Save** - Save to current file
- **Save As...** - Save with new filename
- **Export MIDI...** - Export as MIDI file
- **Export MusicXML...** - Export as MusicXML file
- **Quit** - Exit application

### Edit Menu
- **Undo** - Undo last action
- **Redo** - Redo last undone action
- **Cut** - (Not yet implemented)
- **Copy** - (Not yet implemented)
- **Paste** - (Not yet implemented)

### View Menu
- **Zoom In** - Increase score size
- **Zoom Out** - Decrease score size

### Track Menu
- **Add Measure** - Add measure to current track
- **Remove Measure** - Remove measure at cursor position

### Note Menu
- (Reserved for future note-specific operations)

### Playback Menu
- **Play** - Start playback
- **Stop** - Stop playback
- **Metronome** - Toggle metronome (checkable)

### Help Menu
- **About** - Show about dialog

## Workflow Examples

### Creating a Simple Song

1. **Start New Score**
   - Click **New** or **File** → **New**
   - Default score created with one guitar track, 4 measures

2. **Add Notes**
   - Click in first measure to position cursor
   - Select duration from toolbar (e.g., **Quarter**)
   - Press letter keys (A-G) to add notes
   - Use **Rest** button to add rests

3. **Edit Notes**
   - Click note to select
   - Press **Delete** to remove
   - Inspector panel shows note properties

4. **Add More Measures**
   - **Track** → **Add Measure**
   - Or right-click in track panel

5. **Save Work**
   - Click **Save** or **File** → **Save As...**
   - Choose location and filename
   - Files saved as `.json`

### Adding a Second Track

1. **Open Track Panel** (left sidebar)
2. **Click + Button**
3. **New track appears** in list
4. **Click track name** to select
5. **Edit in score editor**

### Playback

1. **Click Play Button** or **Playback** → **Play**
2. **Cursor follows** playback position
3. **Toggle Metronome** for click track
4. **Click Stop** to halt

### Exporting

#### MIDI Export
1. **File** → **Export MIDI...**
2. Choose save location
3. Enter filename (e.g., `mysong.mid`)
4. Click **Save**
5. File ready for use in DAWs

#### MusicXML Export
1. **File** → **Export MusicXML...**
2. Choose save location
3. Enter filename (e.g., `mysong.musicxml`)
4. Click **Save**
5. Open in Finale, Sibelius, MuseScore, etc.

## Panel Reference

### Track Panel (Left)
- Lists all tracks in score
- Shows track names
- Click to select active track
- **+** button adds track
- **-** button removes selected track

### Inspector Panel (Right)
Shows properties of selected note:
- **Pitch** - Note name (e.g., "C4", "E3") or "Rest"
- **Duration** - Note length (e.g., "Quarter", "Half (dotted)")
- **Velocity** - MIDI velocity (0-127)
- **String** - String number for tab (1-6)
- **Fret** - Fret number (0-24)
- **Tied** - Whether note is tied to next ("Yes"/"No")

### Status Bar (Bottom)
- **Track** - Current track name
- **Position** - Measure and beat (e.g., "Measure 2, Beat 3")
- **Duration** - Currently selected note duration

## Tips & Tricks

### Efficient Note Entry
1. Select duration first
2. Click measure to position cursor
3. Type letters rapidly for quick entry
4. Arrow keys to navigate
5. Delete to fix mistakes

### Working with Tab
- Tab positions auto-calculated from pitch
- Lower strings (thicker) at bottom of tab staff
- Numbers show fret positions
- Standard tuning: E-A-D-G-B-E (low to high)

### Zoom for Detail
- Zoom in to see fine details
- Zoom out for overview
- Score scrolls horizontally and vertically

### Undo is Your Friend
- Every edit is undoable
- Use Ctrl+Z liberally
- Redo with Ctrl+Y

### Save Often
- No auto-save feature
- Save after major edits
- Use **Save As** for versions

## Common Issues

### "No sound during playback"
- Check Windows audio settings
- Ensure speakers/headphones connected
- Playback uses basic synthesis (may be quiet)

### "Can't see my note"
- Try zooming in
- Scroll viewport
- Check if note is in visible area

### "Toolbar buttons don't work"
- Ensure track is selected in track panel
- Some operations require selection
- Check status bar for current state

### "Can't undo"
- Only actions done through commands are undoable
- File operations (open/save) clear undo history
- Direct model manipulation (none in normal use) not undoable

## File Format Notes

### JSON Project Files (.json)
- Human-readable text format
- Can edit in text editor (advanced users)
- Contains all score data
- Version 1 format

### MIDI Files (.mid)
- Standard MIDI format
- Compatible with all DAWs
- Contains tempo, tracks, notes
- No notation-specific data (ties, articulation)

### MusicXML Files (.musicxml)
- Industry-standard notation format
- Preserves most notation elements
- Compatible with major notation software
- Human-readable XML

## Support

For issues, questions, or contributions:
- GitHub: https://github.com/hueyf333/Broken-Arrow-Music-Apps
- See README.md for build instructions
- See IMPLEMENTATION.md for technical details
