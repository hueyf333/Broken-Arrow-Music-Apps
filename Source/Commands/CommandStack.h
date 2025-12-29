#pragma once
#include <JuceHeader.h>
#include "Command.h"

class CommandStack
{
public:
    CommandStack() = default;
    
    void execute(std::unique_ptr<Command> command);
    
    bool canUndo() const { return currentIndex > 0; }
    bool canRedo() const { return currentIndex < static_cast<int>(commands.size()); }
    
    void undo();
    void redo();
    
    void clear();
    
    juce::String getUndoDescription() const;
    juce::String getRedoDescription() const;
    
private:
    std::vector<std::unique_ptr<Command>> commands;
    int currentIndex = 0;
};
