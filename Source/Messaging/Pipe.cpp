/*
  ==============================================================================

    Pipe.cpp
    Created: 14 Nov 2021 3:09:27pm
    Author:  James

  ==============================================================================
*/

#include "Pipe.h"

Pipe::~Pipe()
{
    disconnect();
}

void Pipe::connectionMade()
{
    onConnectionMade();
}

void Pipe::connectionLost()
{
    onConnectionLost();
}

void Pipe::messageReceived(const MemoryBlock& message)
{
    onMessageReceived(message);
}
