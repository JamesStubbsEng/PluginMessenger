/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginMessengerAudioProcessorEditor::PluginMessengerAudioProcessorEditor (PluginMessengerAudioProcessor& p, ValueTree& vt)
    : AudioProcessorEditor (&p), audioProcessor (p), messageValueTree(vt)
{
    // setup messageValueTree listener
    messageValueTree.addListener(this);

    // initialize GUI components
    nameEditor.setTextToShowWhenEmpty(
        "Enter your name...", juce::Colours::lightgrey.withAlpha(0.4f));
    addAndMakeVisible(nameEditor);

    connectionNameEditor.setTextToShowWhenEmpty(
        "Enter connection name...", juce::Colours::lightgrey.withAlpha(0.4f));
    connectionNameEditor.getTextValue().referTo(
        messageValueTree.getPropertyAsValue("currentConnectionName", 0));
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
                audioProcessor.getMessagingPipe().connectToPipe(connectionNameEditor.getText(), -1);
               
            if (!pipeAlreadyExists)
                audioProcessor.getMessagingPipe().createPipe(connectionNameEditor.getText(), -1, true);
        }
    };
    addAndMakeVisible(connectButton);

    sendButton.setButtonText("Send");
    sendButton.onClick = [this]
    {
        ValueTree messageToSend = { "message",
            {
                {"senderName", nameEditor.getText()},
                {"messageBody", messageInputEditor.getText()}
            }
        };

        // send over pipe
        MemoryOutputStream os;
        messageToSend.writeToStream(os);

        audioProcessor.getMessagingPipe().sendMessage(os.getMemoryBlock());

        //put outgoing message into valuetree
        auto connectionVt = messageValueTree.getChildWithProperty(
            "connectionName", messageValueTree.getProperty("currentConnectionName"));
        if (!connectionVt.isValid())
        {
            connectionVt = { "connection",
                {
                    {"connectionName", messageValueTree.getProperty("currentConnectionName")}
                }
            };
            connectionVt.appendChild(messageToSend, 0);
            messageValueTree.appendChild(connectionVt, 0);
        }
        else
        {
            connectionVt.appendChild(messageToSend, 0);
        }
        

        //clear input
        messageInputEditor.setText("");

        DBG(messageValueTree.toXmlString());
    };
    addAndMakeVisible(sendButton);

    setSize (320, 548);
    setResizable(true, true);
}

PluginMessengerAudioProcessorEditor::~PluginMessengerAudioProcessorEditor()
{
    messageValueTree.removeListener(this);
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

void PluginMessengerAudioProcessorEditor::valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded)
{
    //update messageDisplayWidget (for now just used TextEditor instead of widget)

    auto connectionVt = messageValueTree.getChildWithProperty(
        "connectionName", messageValueTree.getProperty("currentConnectionName"));
    if (connectionVt.isValid())
    {
        String displayMessage;
        for (int i = 0; i < connectionVt.getNumChildren(); i++)
        {
            auto senderName = connectionVt.getChild(i).getProperty("senderName");
            auto messageBody = connectionVt.getChild(i).getProperty("messageBody");
            displayMessage = displayMessage + messageBody + " - " + senderName + "\n";        
        }
           
        messageDisplayWidget.setText(displayMessage, false);
    }  
}
