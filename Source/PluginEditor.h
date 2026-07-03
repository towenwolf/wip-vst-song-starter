#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <array>

class SongStarterEditor : public juce::AudioProcessorEditor
{
public:
    explicit SongStarterEditor(SongStarterProcessor&);
    ~SongStarterEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void randomize();
    void copyToClipboard();
    juce::String buildSummary() const;

    SongStarterProcessor& audioProcessor;

    juce::TextButton randomizeButton { "RANDOMIZE" };
    juce::TextButton copyButton      { "COPY" };

    static const juce::StringArray genreNames;
    static const juce::StringArray genreTips;
    static const juce::String      noteNames[12];
    static const std::array<std::pair<int,int>, 16> bpmRanges;

    static constexpr juce::uint32 kBg      = 0xff0d0d1a;
    static constexpr juce::uint32 kCard    = 0xff1a1a2e;
    static constexpr juce::uint32 kAccent  = 0xff6c63ff;
    static constexpr juce::uint32 kPink    = 0xffff6584;
    static constexpr juce::uint32 kText    = 0xffffffff;
    static constexpr juce::uint32 kMuted   = 0xff8888aa;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SongStarterEditor)
};
