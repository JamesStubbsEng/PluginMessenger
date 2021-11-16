/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginMessengerAudioProcessor::PluginMessengerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), messageValueTree(
                           { "root", {{"name", ""}, {"currentConnectionName", ""}, {"isConnected", false}}}
                       )
#endif
{
    //receiving messages callbacks from pipe

    messagingPipe.onMessageReceived = [this](const MemoryBlock& message)
    {
        // update valuetree
        auto connectionVt = messageValueTree.getChildWithProperty(
            "connectionName", messageValueTree.getProperty("currentConnectionName"));
        if (!connectionVt.isValid())
        {
            connectionVt = { "connection",
                {
                    {"connectionName", messagingPipe.getPipe()->getName()}
                }
            };
            connectionVt.appendChild(
                ValueTree::readFromData(message.getData(), message.getSize()), 0);
            messageValueTree.appendChild(connectionVt, 0);
        }
        else
        {
            connectionVt.appendChild(
                ValueTree::readFromData(message.getData(), message.getSize()), 0);
        }
        


    };

    messagingPipe.onConnectionMade = [this]
    {
        DBG("Connection made");
        messageValueTree.setProperty("isConnected", true, 0);
    };

    messagingPipe.onConnectionLost = [this]
    {
        DBG("Connection lost");
        messageValueTree.setProperty("isConnected", false, 0);
    };

}

PluginMessengerAudioProcessor::~PluginMessengerAudioProcessor()
{
}

//==============================================================================
const juce::String PluginMessengerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginMessengerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginMessengerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginMessengerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PluginMessengerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginMessengerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginMessengerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PluginMessengerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PluginMessengerAudioProcessor::getProgramName (int index)
{
    return {};
}

void PluginMessengerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PluginMessengerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void PluginMessengerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PluginMessengerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PluginMessengerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool PluginMessengerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginMessengerAudioProcessor::createEditor()
{
    return new PluginMessengerAudioProcessorEditor (*this, messageValueTree);
}

//==============================================================================
void PluginMessengerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement> xml(messageValueTree.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginMessengerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        messageValueTree.copyPropertiesAndChildrenFrom(juce::ValueTree::fromXml(*xmlState), 0);
}

Pipe& PluginMessengerAudioProcessor::getMessagingPipe()
{
    return messagingPipe;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginMessengerAudioProcessor();
}
