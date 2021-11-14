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

    connectionNameEditor.setTextToShowWhenEmpty(
        "Enter connection name...", juce::Colours::lightgrey.withAlpha(0.4f));
    addAndMakeVisible(connectionNameEditor);

    messageDisplayWidget.setMultiLine(true);
    messageDisplayWidget.setReadOnly(true);
    messageDisplayWidget.setCaretVisible(false);
    addAndMakeVisible(messageDisplayWidget);

    messageInputEditor.setMultiLine(true);
    messageInputEditor.setReturnKeyStartsNewLine(true);
    addAndMakeVisible(messageInputEditor);

    connectButton.setButtonText("Connect");
    connectButton.onClick = [this]
    {       
        if (connectionNameEditor.getText().isNotEmpty())
        {
            auto pipeAlreadyExists = 
                audioProcessor.messagingPipe.connectToPipe(connectionNameEditor.getText(), -1);
               
            if (!pipeAlreadyExists)
                audioProcessor.messagingPipe.createPipe(connectionNameEditor.getText(), -1, true);
        }
    };
    addAndMakeVisible(connectButton);

    sendButton.setButtonText("Send");
    sendButton.onClick = [this]
    {
        //put outgoing message into message display widget
        messageDisplayWidget.setText(
            messageDisplayWidget.getText() + messageInputEditor.getText() + "\n", false);

        //send outgoing message over pipe if there is a connection
        MemoryBlock message(messageInputEditor.getText().toUTF8(), messageInputEditor.getText().getNumBytesAsUTF8());
        //message.loadFromHexString(messageInputEditor.getText());
        audioProcessor.messagingPipe.sendMessage(message);
            
        messageInputEditor.setText("");
    };
    addAndMakeVisible(sendButton);

    // callbacks from pipe
    audioProcessor.messagingPipe.onMessageReceived = [this](const MemoryBlock& message)
    {
        messageDisplayWidget.setText(
            messageDisplayWidget.getText() + message.toString() + "\n", false);
    };

    audioProcessor.messagingPipe.onConnectionMade = [this]
    {
        DBG("Connection made");
    };

    audioProcessor.messagingPipe.onConnectionLost = [this]
    {
        DBG("Connection lost");
    };

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
    connectionNameEditor.setBoundsRelative(0.1f, 0.1314f, 0.5625f, 0.05839f);
    connectButton.setBoundsRelative(0.6875f, 0.1314f, 0.2125f, 0.05839f);
    messageDisplayWidget.setBoundsRelative(0.1f, 0.2263f, 0.8f, 0.5839f);
    messageInputEditor.setBoundsRelative(0.1f, 0.8467f, 0.6125f, 0.1168f);
    sendButton.setBoundsRelative(0.7375f, 0.8759f, 0.1625f, 0.05839f);
}
