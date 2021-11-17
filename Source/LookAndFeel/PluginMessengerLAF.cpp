/*
  ==============================================================================

    PluginMessengerLAF.cpp
    Created: 17 Nov 2021 2:22:50pm
    Author:  James

  ==============================================================================
*/

#include "PluginMessengerLAF.h"

//==============================================================================
void PluginMessengerLAF::fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& textEditor)
{
    g.setColour(textEditor.findColour(TextEditor::backgroundColourId));
    g.fillRoundedRectangle(0, 0, width, height, roundingCornerSize);
}

void PluginMessengerLAF::drawTextEditorOutline(Graphics& g, int width, int height, TextEditor& textEditor)
{
    if (textEditor.isEnabled())
    {
        auto bounds = textEditor.getLocalBounds().toFloat().reduced(outlineThickness / 2);

        if (textEditor.hasKeyboardFocus(true) && !textEditor.isReadOnly())
            g.setColour(textEditor.findColour(TextEditor::focusedOutlineColourId));
        else
            g.setColour(textEditor.findColour(TextEditor::outlineColourId));
        g.drawRoundedRectangle(bounds, roundingCornerSize, outlineThickness);
    }
}

//==============================================================================
Font PluginMessengerLAF::getTextButtonFont(TextButton&, int buttonHeight)
{
    return font.withHeight(fontSize);
}

Font PluginMessengerLAF::getFont()
{
    return font.withHeight(fontSize);
}

void PluginMessengerLAF::drawButtonBackground(Graphics& g,
    Button& button,
    const Colour& backgroundColour,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(outlineThickness/2);

    auto baseColour = backgroundColour.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
        .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

    if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
        baseColour = baseColour.contrasting(shouldDrawButtonAsDown ? 0.2f : 0.05f);

    g.setColour(baseColour);
    g.fillRoundedRectangle(bounds, roundingCornerSize);

    g.setColour(button.findColour(ComboBox::outlineColourId));
    g.drawRoundedRectangle(bounds, roundingCornerSize, outlineThickness);
}
