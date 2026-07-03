#pragma once
#include <JuceHeader.h>

class SongStarterProcessor : public juce::AudioProcessor
{
public:
    SongStarterProcessor();
    ~SongStarterProcessor() override;

    void prepareToPlay(double, int) override {}
    void releaseResources() override {}

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Song Starter"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& dest) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    int genreIndex  = 8;  // Future Bass as default
    int keyIndex    = 5;  // F
    bool keyIsMajor = false;
    int bpm         = 150;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SongStarterProcessor)
};
