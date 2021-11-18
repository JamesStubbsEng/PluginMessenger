/*
  ==============================================================================

    MessageDisplayWidget.h
    Created: 17 Nov 2021 6:26:52pm
    Author:  James

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "MessageDisplayRow.h"

class MessageDisplayWidget : public Component
{
public:
    MessageDisplayWidget();

    void paint(juce::Graphics&) override;
    void resized() override;
    void parentSizeChanged() override;

    void addRowComponent(StringRef senderName, StringRef messageBody, bool isMessageOutgoing);
    void positionListRows();
    void clear();
private:
    Viewport viewport;
    Component viewedComponent;
    OwnedArray<MessageDisplayRow> rows;

};