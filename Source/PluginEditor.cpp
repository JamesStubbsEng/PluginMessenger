/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginMessengerAudioProcessorEditor::PluginMessengerAudioProcessorEditor (PluginMessengerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    nameEditor.setTextToShowWhenEmpty(
        "Enter your name...", juce::Colours::lightgrey.withAlpha(0.4f));
    addAndMakeVisible(nameEditor);

    roomEditor.setTextToShowWhenEmpty(
        "Enter connection name...", juce::Colours::lightgrey.withAlpha(0.4f));
    addAndMakeVisible(roomEditor);

    messageDisplayWidget.setMultiLine(true);
    messageDisplayWidget.setReadOnly(true);
    messageDisplayWidget.setCaretVisible(false);
    addAndMakeVisible(messageDisplayWidget);

    messageInputEditor.setMultiLine(true);
    messageInputEditor.setReturnKeyStartsNewLine(true);
    addAndMakeVisible(messageInputEditor);

    connectButton.setButtonText("Connect");
    addAndMakeVisible(connectButton);

    sendButton.setButtonText("Send");
    sendButton.onClick = [this]
    {
        messageDisplayWidget.setText(
            messageDisplayWidget.getText() + "\n" + messageInputEditor.getText(), false);
        messageInputEditor.setText("");
    };
    addAndMakeVisible(sendButton);

    setSize (320, 548);
    setResizable(true, true);
}

PluginMessengerAudioProcessorEditor::~PluginMessengerAudioProcessorEditor()
{
}

//==============================================================================
void PluginMessengerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void PluginMessengerAudioProcessorEditor::resized()
{
    nameEditor.setBoundsRelative(0.1f, 0.0365f, 0.5625f, 0.05839f);
    roomEditor.setBoundsRelative(0.1f, 0.1314f, 0.5625f, 0.05839f);
    connectButton.setBoundsRelative(0.6875f, 0.1314f, 0.2125f, 0.05839f);
    messageDisplayWidget.setBoundsRelative(0.1f, 0.2263f, 0.8f, 0.5839f);
    messageInputEditor.setBoundsRelative(0.1f, 0.8467f, 0.6125f, 0.1168f);
    sendButton.setBoundsRelative(0.7375f, 0.8759f, 0.1625f, 0.05839f);
}
