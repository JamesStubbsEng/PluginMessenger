/*
  ==============================================================================

    Pipe.h
    Created: 14 Nov 2021 3:09:27pm
    Author:  James

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Pipe : public juce::InterprocessConnection
{
public:
    ~Pipe();
    //used to create callbacks in editor
    std::function<void()> onConnectionMade;
    std::function<void()> onConnectionLost;
    std::function<void(const MemoryBlock& message)> onMessageReceived;
private:
    void connectionMade() override;
    void connectionLost() override;
    void messageReceived(const MemoryBlock& message) override;
};