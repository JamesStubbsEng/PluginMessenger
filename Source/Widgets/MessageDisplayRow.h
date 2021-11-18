/*
  ==============================================================================

    MessageDisplayRow.h
    Created: 17 Nov 2021 6:28:03pm
    Author:  James

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../fonts/RobotoCondensed.h"

class MessageDisplayRow : public Component
{
public:
    MessageDisplayRow(StringRef senderName, StringRef messageBody, bool hasHeader, bool isMessageOutgoing);
    void paint(juce::Graphics&) override;
    void resized() override;
    void parentSizeChanged() override;
    void configureSize();
    const String& getSenderName();
private:
    String messageBody;
    String senderName;
    const int pillMarginX = 10;
    const int pillMarginY = 5;
    const int messagePaddingX = 5;
    const int messagePaddingY = 0;
    int messageWidth = 0;
    int messageHeight = 0;
    int headerHeight = 30;
    bool hasHeader;
    bool isMessageOutgoing;
    juce::Font robotoRegular = PluginMessenger::fonts::robotoRegular();
    juce::Font robotoBold = PluginMessenger::fonts::robotoBold();
};