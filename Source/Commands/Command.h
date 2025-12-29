#pragma once
#include <JuceHeader.h>

class Command
{
public:
    virtual ~Command() = default;
    
    virtual bool execute() = 0;
    virtual bool undo() = 0;
    
    virtual juce::String getDescription() const = 0;
};
