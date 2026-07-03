#include "PluginEditor.h"

//==============================================================================
// Static data

const juce::StringArray SongStarterEditor::genreNames = {
    "House", "Deep House", "Tech House", "Techno",
    "Trance", "Drum & Bass", "Jungle", "Dubstep",
    "Future Bass", "Trap", "Lo-Fi Hip-Hop", "Synthwave",
    "Ambient", "IDM", "UK Garage", "Breakbeat"
};

const juce::StringArray SongStarterEditor::genreTips = {
    "4-on-the-floor kick, offbeat hi-hats, soulful chords",
    "Deep pads, heavy sub, late-night atmosphere",
    "Driving groove, tech synths, peak-hour energy",
    "Industrial textures, hypnotic loops, raw and dark",
    "Uplifting arps, big buildups, euphoric drops",
    "Breakbeats at 170, sub bass, chopped Amen",
    "Dense percussion, ragga vocals, jungle energy",
    "Half-time drums, wobble bass, filthy drops",
    "Bright synths, emotional chords, vocal chops",
    "808 bass, trap hi-hats, minimal melody",
    "Dusty samples, mellow chords, chill vibes",
    "Analog synths, gated reverb, 80s nostalgia",
    "Evolving pads, field recordings, no rush",
    "Glitchy textures, complex rhythms, experimental",
    "2-step groove, syncopated bass, vocal chops",
    "Punchy breaks, filtered loops, high energy"
};

const juce::String SongStarterEditor::noteNames[12] = {
    "C", "C#", "D", "D#", "E", "F",
    "F#", "G", "G#", "A", "A#", "B"
};

const std::array<std::pair<int,int>, 16> SongStarterEditor::bpmRanges = {{
    {120, 130}, {115, 125}, {125, 135}, {130, 150},
    {128, 145}, {165, 180}, {160, 175}, {138, 145},
    {140, 160}, {130, 160}, {70,  90},  {100, 120},
    {60,  90},  {80,  160}, {130, 135}, {120, 140}
}};

//==============================================================================

class AccentLookAndFeel : public juce::LookAndFeel_V4
{
public:
    explicit AccentLookAndFeel(juce::uint32 bg, juce::uint32 fg)
        : bgCol(bg), fgCol(fg) {}

    void drawButtonBackground(juce::Graphics& g, juce::Button& btn,
                               const juce::Colour&, bool highlighted, bool down) override
    {
        auto bounds = btn.getLocalBounds().toFloat().reduced(1.0f);
        auto col = juce::Colour(bgCol);
        if (down)       col = col.darker(0.2f);
        else if (highlighted) col = col.brighter(0.15f);

        g.setColour(col);
        g.fillRoundedRectangle(bounds, 6.0f);
    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& btn,
                        bool, bool) override
    {
        g.setColour(juce::Colour(fgCol));
        g.setFont(juce::Font(juce::FontOptions().withHeight(13.0f).withStyle("Bold")));
        g.drawText(btn.getButtonText(), btn.getLocalBounds(),
                   juce::Justification::centred, false);
    }

private:
    juce::uint32 bgCol, fgCol;
};

//==============================================================================

SongStarterEditor::SongStarterEditor(SongStarterProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(460, 310);

    randomizeButton.setLookAndFeel(new AccentLookAndFeel(kAccent, kText));
    copyButton.setLookAndFeel(new AccentLookAndFeel(kCard, kMuted));

    randomizeButton.onClick = [this] { randomize(); };
    copyButton.onClick      = [this] { copyToClipboard(); };

    addAndMakeVisible(randomizeButton);
    addAndMakeVisible(copyButton);
}

SongStarterEditor::~SongStarterEditor()
{
    randomizeButton.setLookAndFeel(nullptr);
    copyButton.setLookAndFeel(nullptr);
}

void SongStarterEditor::paint(juce::Graphics& g)
{
    const auto& p = audioProcessor;
    const auto  w = getWidth();

    // Background
    g.fillAll(juce::Colour(kBg));

    // Header bar
    g.setColour(juce::Colour(kAccent).withAlpha(0.15f));
    g.fillRect(0, 0, w, 38);

    // Header text
    g.setColour(juce::Colour(kAccent));
    g.setFont(juce::Font(juce::FontOptions().withHeight(13.0f).withStyle("Bold")));
    g.drawText("SONG STARTER  v1.0", 16, 0, w - 32, 38, juce::Justification::centredLeft);

    // Accent line under header
    g.setColour(juce::Colour(kAccent).withAlpha(0.4f));
    g.fillRect(0, 37, w, 1);

    // ---- Genre card ----
    const int cardPad = 16;
    juce::Rectangle<int> genreCard(cardPad, 52, w - cardPad * 2, 90);

    g.setColour(juce::Colour(kCard));
    g.fillRoundedRectangle(genreCard.toFloat(), 8.0f);

    g.setColour(juce::Colour(kMuted));
    g.setFont(juce::Font(juce::FontOptions().withHeight(11.0f).withStyle("Bold")));
    g.drawText("GENRE", genreCard.getX() + 12, genreCard.getY() + 10,
               200, 16, juce::Justification::centredLeft);

    g.setColour(juce::Colour(kText));
    g.setFont(juce::Font(juce::FontOptions().withHeight(32.0f).withStyle("Bold")));
    g.drawText(genreNames[p.genreIndex], genreCard.getX() + 12,
               genreCard.getY() + 28, genreCard.getWidth() - 24, 40,
               juce::Justification::centredLeft);

    g.setColour(juce::Colour(kMuted));
    g.setFont(juce::Font(juce::FontOptions().withHeight(11.0f)));
    g.drawText(genreTips[p.genreIndex],
               genreCard.getX() + 12, genreCard.getY() + 68,
               genreCard.getWidth() - 24, 14,
               juce::Justification::centredLeft);

    // ---- Key card ----
    const int row2y = 158;
    const int halfW = (w - cardPad * 3) / 2;

    juce::Rectangle<int> keyCard(cardPad, row2y, halfW, 70);
    g.setColour(juce::Colour(kCard));
    g.fillRoundedRectangle(keyCard.toFloat(), 8.0f);

    g.setColour(juce::Colour(kMuted));
    g.setFont(juce::Font(juce::FontOptions().withHeight(11.0f).withStyle("Bold")));
    g.drawText("KEY", keyCard.getX() + 12, keyCard.getY() + 10,
               80, 16, juce::Justification::centredLeft);

    juce::String keyStr = noteNames[p.keyIndex];
    keyStr += p.keyIsMajor ? " Major" : " Minor";

    g.setColour(juce::Colour(kPink));
    g.setFont(juce::Font(juce::FontOptions().withHeight(26.0f).withStyle("Bold")));
    g.drawText(keyStr, keyCard.getX() + 12, keyCard.getY() + 28,
               keyCard.getWidth() - 24, 34, juce::Justification::centredLeft);

    // ---- BPM card ----
    juce::Rectangle<int> bpmCard(cardPad * 2 + halfW, row2y, halfW, 70);
    g.setColour(juce::Colour(kCard));
    g.fillRoundedRectangle(bpmCard.toFloat(), 8.0f);

    g.setColour(juce::Colour(kMuted));
    g.setFont(juce::Font(juce::FontOptions().withHeight(11.0f).withStyle("Bold")));
    g.drawText("BPM", bpmCard.getX() + 12, bpmCard.getY() + 10,
               80, 16, juce::Justification::centredLeft);

    g.setColour(juce::Colour(kAccent));
    g.setFont(juce::Font(juce::FontOptions().withHeight(26.0f).withStyle("Bold")));
    g.drawText(juce::String(p.bpm), bpmCard.getX() + 12, bpmCard.getY() + 28,
               bpmCard.getWidth() - 24, 34, juce::Justification::centredLeft);
}

void SongStarterEditor::resized()
{
    const int w = getWidth();
    randomizeButton.setBounds(16,      243, w - 32 - 80, 42);
    copyButton.setBounds(w - 16 - 74,  243, 74,          42);
}

void SongStarterEditor::randomize()
{
    auto& rng = juce::Random::getSystemRandom();
    auto& p   = audioProcessor;

    p.genreIndex  = rng.nextInt(static_cast<int>(genreNames.size()));
    p.keyIndex    = rng.nextInt(12);
    p.keyIsMajor  = rng.nextBool();

    auto [lo, hi] = bpmRanges[static_cast<size_t>(p.genreIndex)];
    p.bpm         = lo + rng.nextInt(hi - lo + 1);

    repaint();
}

void SongStarterEditor::copyToClipboard()
{
    juce::SystemClipboard::copyTextToClipboard(buildSummary());

    // Flash the copy button text briefly
    copyButton.setButtonText("COPIED!");
    juce::Timer::callAfterDelay(1200, [this] {
        if (copyButton.isVisible())
            copyButton.setButtonText("COPY");
    });
}

juce::String SongStarterEditor::buildSummary() const
{
    const auto& p = audioProcessor;
    return juce::String("Genre: ") + genreNames[p.genreIndex]
         + "  |  Key: " + noteNames[p.keyIndex] + (p.keyIsMajor ? " Major" : " Minor")
         + "  |  BPM: " + juce::String(p.bpm)
         + "\nTip: " + genreTips[p.genreIndex];
}
