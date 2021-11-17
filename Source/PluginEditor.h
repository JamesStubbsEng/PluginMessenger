/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LookAndFeel/PluginMessengerLAF.h"

//==============================================================================
/**
*/
class PluginMessengerAudioProcessorEditor  : public juce::AudioProcessorEditor, public ValueTree::Listener
{
public:
    PluginMessengerAudioProcessorEditor (PluginMessengerAudioProcessor&, ValueTree&);
    ~PluginMessengerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override;
    void valueTreePropertyChanged(ValueTree& tree, const Identifier& propertyID) override;
    
    void updateMessageDisplayWidget();
    void updateConnectionButton();

    PluginMessengerLAF pluginMessengerLAF;

    PluginMessengerAudioProcessor& audioProcessor;

    ValueTree messageValueTree;

    TextEditor nameEditor;
    TextEditor connectionNameEditor;
    TextEditor messageDisplayWidget;
    TextEditor messageInputEditor;

    TextButton connectButton;
    TextButton sendButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginMessengerAudioProcessorEditor)
};
