/*
  ==============================================================================

    PluginMessengerLAF.h
    Created: 17 Nov 2021 2:22:50pm
    Author:  James

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../fonts/RobotoCondensed.h"

class PluginMessengerLAF : public LookAndFeel_V4
{
public:
    void fillTextEditorBackground(Graphics&, int width, int height, TextEditor&) override;
    void drawTextEditorOutline(Graphics&, int width, int height, TextEditor&) override;
    void drawButtonBackground(Graphics&, Button&, const Colour& backgroundColour,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    Font getTextButtonFont(TextButton&, int buttonHeight) override;
    Font getFont();
    void drawScrollbar(Graphics& g, ScrollBar& scrollbar, int x, int y, int width, int height,
        bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOver, bool isMouseDown) override;
private:
    int roundingCornerSize = 6;
    float outlineThickness = 2;
    juce::Font font = PluginMessenger::fonts::robotoRegular();
    float fontSize = 17.0f;
};