#include "PluginProcessor.h"
#include "PluginEditor.h"

SongStarterProcessor::SongStarterProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input",   juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

SongStarterProcessor::~SongStarterProcessor() {}

bool SongStarterProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void SongStarterProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    // Pass-through — this plugin only displays info, doesn't process audio
    juce::ignoreUnused(buffer);
}

juce::AudioProcessorEditor* SongStarterProcessor::createEditor()
{
    return new SongStarterEditor(*this);
}

void SongStarterProcessor::getStateInformation(juce::MemoryBlock& dest)
{
    juce::MemoryOutputStream stream(dest, true);
    stream.writeInt(genreIndex);
    stream.writeInt(keyIndex);
    stream.writeBool(keyIsMajor);
    stream.writeInt(bpm);
}

void SongStarterProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    juce::MemoryInputStream stream(data, static_cast<size_t>(sizeInBytes), false);
    if (stream.getDataSize() < 13) return;
    genreIndex  = stream.readInt();
    keyIndex    = stream.readInt();
    keyIsMajor  = stream.readBool();
    bpm         = stream.readInt();
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SongStarterProcessor();
}
