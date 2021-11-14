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
    nameEditor.setBounds(32, 20, 180, 32);
    roomEditor.setBounds(32, 72, 180, 32);
    connectButton.setBounds(220, 72, 68, 32);
    messageDisplayWidget.setBounds(32, 124, 256, 320);
    messageInputEditor.setBounds(32, 464, 196, 64);
    sendButton.setBounds(236, 480, 52, 32);
}
