#include "CommandStack.h"

void CommandStack::execute(std::unique_ptr<Command> command)
{
    if (!command)
        return;
    
    if (command->execute())
    {
        // Remove any commands after current index (redo history)
        if (currentIndex < static_cast<int>(commands.size()))
            commands.erase(commands.begin() + currentIndex, commands.end());
        
        commands.push_back(std::move(command));
        currentIndex = static_cast<int>(commands.size());
    }
}

void CommandStack::undo()
{
    if (!canUndo())
        return;
    
    currentIndex--;
    commands[currentIndex]->undo();
}

void CommandStack::redo()
{
    if (!canRedo())
        return;
    
    commands[currentIndex]->execute();
    currentIndex++;
}

void CommandStack::clear()
{
    commands.clear();
    currentIndex = 0;
}

juce::String CommandStack::getUndoDescription() const
{
    if (canUndo())
        return commands[currentIndex - 1]->getDescription();
    return {};
}

juce::String CommandStack::getRedoDescription() const
{
    if (canRedo())
        return commands[currentIndex]->getDescription();
    return {};
}
