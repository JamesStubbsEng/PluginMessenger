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

    // setup LAF to be used TextEditors and TextButtons
    auto outlineColour = Colour(0xff77737a);
    pluginMessengerLAF.setColour(TextEditor::backgroundColourId, Colours::black.withAlpha(0.1f));
    pluginMessengerLAF.setColour(TextEditor::outlineColourId, outlineColour);
    pluginMessengerLAF.setColour(TextEditor::textColourId, Colours::white);
    pluginMessengerLAF.setColour(TextButton::buttonColourId, Colours::black.withAlpha(0.1f));
    pluginMessengerLAF.setColour(ComboBox::outlineColourId, outlineColour);
    getLookAndFeel().setDefaultLookAndFeel(&pluginMessengerLAF);

    // initialize GUI components
    nameEditor.setTextToShowWhenEmpty(
        "Enter your name...", juce::Colours::lightgrey.withAlpha(0.4f));
    nameEditor.getTextValue().referTo(
        messageValueTree.getPropertyAsValue("name", 0));
    nameEditor.setFont(pluginMessengerLAF.getFont());
    nameEditor.setJustification(Justification::centredLeft);
    nameEditor.setIndents(10, 0);
    addAndMakeVisible(nameEditor);

    connectionNameEditor.setTextToShowWhenEmpty(
        "Enter connection name...", juce::Colours::lightgrey.withAlpha(0.4f));
    connectionNameEditor.getTextValue().referTo(
        messageValueTree.getPropertyAsValue("currentConnectionName", 0));
    connectionNameEditor.setFont(pluginMessengerLAF.getFont());
    connectionNameEditor.setJustification(Justification::centredLeft);
    connectionNameEditor.setIndents(10, 0);
    addAndMakeVisible(connectionNameEditor);

    messageDisplayWidget.setMultiLine(true);
    messageDisplayWidget.setReadOnly(true);
    messageDisplayWidget.setCaretVisible(false);
    messageDisplayWidget.setFont(pluginMessengerLAF.getFont());
    addAndMakeVisible(messageDisplayWidget);

    messageInputEditor.setMultiLine(true);
    messageInputEditor.setReturnKeyStartsNewLine(true);
    messageInputEditor.setFont(pluginMessengerLAF.getFont());
    messageInputEditor.setJustification(Justification::topLeft);
    messageInputEditor.setIndents(10, 6);
    addAndMakeVisible(messageInputEditor);

    connectButton.onClick = [this]
    {   
        if (connectionNameEditor.getText().isEmpty())
            return;
        auto& pipe = audioProcessor.getMessagingPipe();
        if (!pipe.isConnected())
        {
            auto pipeAlreadyExists = 
                pipe.connectToPipe(connectionNameEditor.getText(), -1);
               
            if (!pipeAlreadyExists)
                pipe.createPipe(connectionNameEditor.getText(), -1, true);
        }
        else
        {
            pipe.getPipe()->close();
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

    updateMessageDisplayWidget();
    updateConnectionButton();

    setSize (340, 550);
    setResizable(true, true);
}

PluginMessengerAudioProcessorEditor::~PluginMessengerAudioProcessorEditor()
{
    messageValueTree.removeListener(this);
}

//==============================================================================
void PluginMessengerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (Colour(0xff373846));
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
    updateMessageDisplayWidget();   
}

void PluginMessengerAudioProcessorEditor::valueTreePropertyChanged(ValueTree& tree, const Identifier& propertyID) 
{
    //update connect button text
    if (propertyID == Identifier("isConnected"))
        updateConnectionButton();
}

void PluginMessengerAudioProcessorEditor::updateMessageDisplayWidget()
{
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

void PluginMessengerAudioProcessorEditor::updateConnectionButton()
{
    auto isConnected = messageValueTree.getProperty("isConnected");
    if (isConnected)
        connectButton.setButtonText("Disconnect");
    else
        connectButton.setButtonText("Connect");
}
