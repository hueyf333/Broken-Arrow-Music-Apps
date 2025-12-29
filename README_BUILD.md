# Broken Arrow Tab & Score Studio - Build Instructions

## Overview
Broken Arrow Tab & Score Studio is a JUCE-based C++20 Windows desktop application for music notation and tablature editing, inspired by Guitar Pro.

## Prerequisites

### Required Software
1. **JUCE Framework** (Version 7.0.9 or later)
   - Download from: https://juce.com/get-juce/download
   - Install to a known location (e.g., `C:\JUCE`)
   
2. **Visual Studio 2022 or 2026**
   - Download from: https://visualstudio.microsoft.com/
   - Required workloads:
     - Desktop development with C++
     - C++20 support

3. **Projucer** (included with JUCE)
   - Located in the JUCE installation directory
   - Used to generate Visual Studio project files

## Build Steps

### 1. Set Up JUCE Module Path

1. Open Projucer (e.g., `C:\JUCE\Projucer.exe`)
2. Go to **File** → **Global Paths...**
3. Set the JUCE modules path to your JUCE installation (e.g., `C:\JUCE\modules`)
4. Click **OK**

### 2. Open the Project

1. In Projucer, go to **File** → **Open...**
2. Navigate to the repository root directory
3. Open `BrokenArrowTabStudio.jucer`

### 3. Generate Visual Studio Project

1. In Projucer, the project should now be loaded
2. The Visual Studio 2022 exporter should be visible in the left panel
3. Click **File** → **Save Project and Open in IDE**
   - This will generate the Visual Studio solution and project files in `Builds/VisualStudio2022/`
   - Visual Studio should open automatically

Alternatively:
- Click the **Visual Studio 2022** icon in the exporters list
- Click **Save and Open in IDE**

### 4. Build in Visual Studio

#### Debug Build
1. In Visual Studio, select **Debug** configuration from the dropdown
2. Select **x64** platform (or **Win32** if preferred)
3. Go to **Build** → **Build Solution** (or press `Ctrl+Shift+B`)
4. The executable will be created in `Builds/VisualStudio2022/x64/Debug/App/BrokenArrowTabStudio.exe`

#### Release Build
1. In Visual Studio, select **Release** configuration from the dropdown
2. Select **x64** platform
3. Go to **Build** → **Build Solution** (or press `Ctrl+Shift+B`)
4. The executable will be created in `Builds/VisualStudio2022/x64/Release/App/BrokenArrowTabStudio.exe`

### 5. Run the Application

- **From Visual Studio**: Press `F5` to run with debugging, or `Ctrl+F5` to run without debugging
- **From Explorer**: Navigate to the build output directory and double-click `BrokenArrowTabStudio.exe`

## Project Structure

```
Broken-Arrow-Music-Apps/
├── BrokenArrowTabStudio.jucer    # JUCE project file
├── Source/                        # Source code
│   ├── Main.cpp                   # Application entry point
│   ├── MainWindow.h/cpp           # Main application window
│   ├── Model/                     # Data model (Score, Track, Measure, Note)
│   ├── Commands/                  # Command pattern for undo/redo
│   ├── Renderer/                  # Score rendering engine
│   ├── Editor/                    # Score editor component
│   ├── Playback/                  # Playback engine
│   ├── Serialization/             # Save/load and export functionality
│   └── UI/                        # UI components (panels, toolbar, etc.)
├── Builds/                        # Generated build files (not in repo)
│   └── VisualStudio2022/          # Visual Studio solution/projects
└── JuceLibraryCode/               # Auto-generated JUCE code (not in repo)
```

## Features

### Implemented Features
- ✅ Create new scores with multiple tracks
- ✅ Guitar tablature and standard notation display
- ✅ Note entry (keyboard and mouse input)
- ✅ Multiple note durations (whole, half, quarter, 8th, 16th)
- ✅ Dotted notes and ties
- ✅ Rests
- ✅ Undo/redo system
- ✅ Add/remove measures and tracks
- ✅ Save/load scores (JSON format)
- ✅ Export to MIDI
- ✅ Export to MusicXML
- ✅ Playback with metronome
- ✅ Dark, modern UI theme
- ✅ Inspector panel showing note properties
- ✅ Track management panel
- ✅ Status bar with position information
- ✅ Zoom in/out functionality

### Usage

#### Creating a New Score
1. Launch the application
2. Go to **File** → **New** (or click the **New** toolbar button)
3. A default score with one guitar track and 4 measures will be created

#### Adding Notes
1. Select a track from the left panel
2. Choose a note duration from the toolbar (Whole, Half, Quarter, 8th, 16th)
3. Click in the score editor where you want to add a note
4. Press letter keys (a-g) to add notes, or number keys (0-9)
5. Toggle **Rest** to add rest symbols instead of notes

#### Editing
- **Delete**: Select a note and press Delete or Backspace
- **Undo**: Ctrl+Z or **Edit** → **Undo**
- **Redo**: Ctrl+Y or **Edit** → **Redo**
- **Add Measure**: **Track** → **Add Measure**
- **Zoom**: **View** → **Zoom In/Out**

#### Playback
1. Click **Play** button or **Playback** → **Play**
2. Toggle **Metronome** for click track
3. Click **Stop** to halt playback

#### Saving
- **Save**: **File** → **Save** (or toolbar button)
- **Save As**: **File** → **Save As...**
- Files are saved in JSON format with `.json` extension

#### Exporting
- **MIDI**: **File** → **Export MIDI...** (creates `.mid` file)
- **MusicXML**: **File** → **Export MusicXML...** (creates `.musicxml` file)

## Troubleshooting

### Build Errors

**"Cannot open JUCE modules"**
- Solution: Set the JUCE modules path in Projucer Global Paths

**"C++20 features not available"**
- Solution: Ensure Visual Studio has C++20 support installed
- Check project settings: C/C++ → Language → C++ Language Standard should be "ISO C++20"

**"LNK1104: cannot open file"**
- Solution: Clean and rebuild the solution
- Go to **Build** → **Clean Solution**, then **Build** → **Rebuild Solution**

### Runtime Issues

**Application doesn't launch**
- Check that all JUCE modules are properly linked
- Verify Windows has required Visual C++ redistributables installed

**No sound during playback**
- Verify audio output device is connected and enabled in Windows

## Development Notes

- **C++ Version**: C++20
- **JUCE Version**: 7.0.9 (or compatible)
- **Compiler**: MSVC (Visual Studio 2022/2026)
- **Platform**: Windows x64
- **No external dependencies** beyond JUCE framework

## License

See LICENSE file in the repository root.

## Support

For issues or questions, please visit the repository at:
https://github.com/hueyf333/Broken-Arrow-Music-Apps
