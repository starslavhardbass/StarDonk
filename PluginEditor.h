#pragma once
#include <array>

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <memory>
#include <functional>

class FocusSafeSlider : public juce::Slider
{
public:
    FocusSafeSlider() = default;
    ~FocusSafeSlider() override = default;

    void mouseDown (const juce::MouseEvent& event) override
    {
        juce::Slider::mouseDown (event);

        if (auto* editor = findParentComponentOfClass<juce::AudioProcessorEditor>())
            editor->grabKeyboardFocus();
    }

    void mouseUp (const juce::MouseEvent& event) override
    {
        juce::Slider::mouseUp (event);

        if (auto* editor = findParentComponentOfClass<juce::AudioProcessorEditor>())
            editor->grabKeyboardFocus();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FocusSafeSlider)
};

class DonkSlider : public FocusSafeSlider
{
public:
    DonkSlider() = default;
    ~DonkSlider() override = default;

    void setNormalDonkRange (double newNormalMinimum, double newNormalMaximum)
    {
        normalMinimum = newNormalMinimum;
        normalMaximum = newNormalMaximum;
    }

    void setRawValueDisplay (bool shouldShowRaw)
    {
        rawValueDisplay = shouldShowRaw;
        updateText();
    }

    juce::String getTextFromValue (double value) override
    {
        if (rawValueDisplay)
        {
            const double interval = getInterval();

            if (interval >= 1.0)
                return juce::String (value, 0);

            if (interval >= 0.1)
                return juce::String (value, 1);

            return juce::String (value, 2);
        }

        if (normalMaximum <= normalMinimum)
            return "[]";

        if (value < normalMinimum)
        {
            const double experimentalMinimum = getMinimum();

            const double amount = juce::jlimit (
                    0.0,
                    1.0,
                    (normalMinimum - value)
                    /
                    juce::jmax (
                        0.000001,
                        normalMinimum - experimentalMinimum));

            if (amount < 0.07)
                return "[K]";

            if (amount < 0.14)
                return "[KN]";

            if (amount < 0.21)
                return "[KNO]";

            if (amount < 0.28)
                return "[KNOD]";

            // efter [knod] dra ut o:et mer när de blir riktigt långt åt minus
            const double stretchAmount = juce::jlimit (
                    0.0,
                    1.0,
                    (amount - 0.28) / 0.72);

            const int totalOs = 1
                + static_cast<int> (
                    std::round (stretchAmount * 8.0));

            juce::String os;

            for (int i = 0; i < totalOs; ++i)
                os << "O";

            return "[KN" + os + "D]";
        }

        if (value > normalMaximum)
        {
            const double experimentalMaximum = getMaximum();

            const double amount = juce::jlimit (
                    0.0,
                    1.0,
                    (value - normalMaximum)
                    /
                    juce::jmax (
                        0.000001,
                        experimentalMaximum - normalMaximum));

            const int extraOs = 2
                + static_cast<int> (
                    std::round (amount * 8.0));

            juce::String os;

            for (int i = 0; i < extraOs; ++i)
                os << "O";

            return "[D" + os + "NK]";
        }

        const double normalized = juce::jlimit (
                0.0,
                1.0,
                (value - normalMinimum)
                /
                (normalMaximum - normalMinimum));

        if (normalized < 0.20)
            return "[]";

        if (normalized < 0.40)
            return "[D]";

        if (normalized < 0.60)
            return "[DO]";

        if (normalized < 0.80)
            return "[DON]";

        return "[DONK]";
    }

private:
    double normalMinimum = 0.0;
    double normalMaximum = 1.0;
    bool rawValueDisplay = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DonkSlider)
};

class PercentSlider : public FocusSafeSlider
{
public:
    PercentSlider() = default;
    ~PercentSlider() override = default;

    juce::String getTextFromValue (double value) override
    {
        return juce::String (
            static_cast<int> (
                std::round (value)))
            + "%";
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PercentSlider)
};

class PresetGridComponent : public juce::Component
{
public:
    PresetGridComponent()
    {
        setOpaque (false);
    }

    ~PresetGridComponent() override = default;
    std::function<void(int)> onPresetSelected;

    void setPresets (const juce::StringArray& names)
    {
        presetNames = names;
        presetButtons.clear();

        for (int i = 0;
             i < presetNames.size();
             ++i)
        {
            auto* button = new juce::TextButton (
                    presetNames[i]);

            button->setWantsKeyboardFocus (false);
            button->setMouseClickGrabsKeyboardFocus (false);

            button->setColour (
                juce::TextButton::buttonColourId,
                juce::Colour (0xff211b2c));

            button->setColour (
                juce::TextButton::buttonOnColourId,
                juce::Colour (0xffffc933));

            button->setColour (
                juce::TextButton::textColourOffId,
                juce::Colour (0xffffd34e));

            button->setColour (
                juce::TextButton::textColourOnId,
                juce::Colour (0xff08070d));

            const int index = i;

            button->onClick = [this, index]()
                {
                    if (onPresetSelected)
                        onPresetSelected (index);
                };

            presetButtons.add (button);
            addAndMakeVisible (button);
        }

        updateSize();
        resized();
        repaint();
    }

    int getPreferredWidth() const
    {
        const int columns = 3;
        const int cellWidth = 154;
        const int gap = 6;
        const int padding = 10;

        return
            padding * 2
            +
            columns * cellWidth
            +
            (columns - 1) * gap;
    }

    int getPreferredHeight() const
    {
        const int columns = 3;
        const int cellHeight = 30;
        const int gap = 6;
        const int padding = 10;

        const int rows = juce::jmax (
                1,
                (presetButtons.size()
                 + columns - 1)
                / columns);

        return
            padding * 2
            +
            rows * cellHeight
            +
            (rows - 1) * gap;
    }

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds()
                .toFloat();

        // skugga
        g.setColour (
            juce::Colours::black
                .withAlpha (0.45f));

        g.fillRoundedRectangle (
            bounds.translated (
                3.0f,
                4.0f),
            12.0f);

        // bakgrund
        g.setColour (
            juce::Colour (0xff100d17));

        g.fillRoundedRectangle (
            bounds,
            12.0f);

        // kant typ
        g.setColour (
            juce::Colour (0xff4a3c57));

        g.drawRoundedRectangle (
            bounds.reduced (0.5f),
            12.0f,
            1.0f);

        // guld linje där uppe
        g.setColour (
            juce::Colour (0xffffcc33));

        g.drawLine (
            12.0f,
            8.0f,
            static_cast<float> (
                getWidth() - 12),
            8.0f,
            1.0f);
    }

    void resized() override
    {
        const int columns = 3;
        const int cellWidth = 154;
        const int cellHeight = 30;
        const int gap = 6;
        const int padding = 10;

        for (int i = 0;
             i < presetButtons.size();
             ++i)
        {
            const int row = i / columns;
            const int column = i % columns;

            const int x = padding
                +
                column * (cellWidth + gap);

            const int y = padding
                +
                row * (cellHeight + gap);

            presetButtons[i]->setBounds (
                x,
                y,
                cellWidth,
                cellHeight);
        }
    }

private:
    juce::StringArray presetNames;
    juce::OwnedArray<juce::TextButton> presetButtons;

    void updateSize()
    {
        setSize (
            getPreferredWidth(),
            getPreferredHeight());
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetGridComponent)
};

class StarDonkFunPanel : public juce::Component
{
public:
    void paint (juce::Graphics& g) override
    {
        auto b = getLocalBounds().toFloat();
        g.setColour (juce::Colours::black.withAlpha (0.88f));
        g.fillRoundedRectangle (b, 10.0f);
        g.setColour (juce::Colour (0xffffcc33));
        g.drawRoundedRectangle (b.reduced (0.5f), 10.0f, 1.0f);
    }
};

class StarDonkHelpComponent : public juce::Component
{
public:
    class HelpBodyComponent : public juce::Component
    {
    public:
        HelpBodyComponent()
        {
            setSize (540, 940);
        }

        void paint (juce::Graphics& g) override
        {
            int y = 8;
            const int x = 8;
            const int w = getWidth() - 22;

            auto drawSection = [&] (const juce::String& title,
                     const juce::String& body,
                     int bodyHeight)
                {
                    g.setFont (juce::Font (juce::FontOptions()
                                .withName ("Arial")
                                .withHeight (12.0f)
                                .withStyle ("Bold")));

                    g.setColour (juce::Colour (0xffffd34e));
                    g.drawText (
                        title,
                        x,
                        y,
                        w,
                        18,
                        juce::Justification::centredLeft);

                    y += 18;

                    g.setFont (juce::Font (juce::FontOptions()
                                .withName ("Arial")
                                .withHeight (11.5f)));

                    g.setColour (juce::Colour (0xffb9b3c6));
                    g.drawFittedText (
                        body,
                        juce::Rectangle<int> (x, y, w, bodyHeight),
                        juce::Justification::topLeft,
                        10,
                        1.0f);

                    y += bodyHeight + 12;
                };

            drawSection (
                "KNOBS",
                "CYKA = pitch drop   |   BLYAT = decay   |   DONK = FM knock   |   GOPNIK = FM ratio\n"
                "KVASS = waveform   |   SEMECHKI = tone   |   HARD = body/sustain   |   BASS = drive",
                34);

            drawSection (
                "EXTRA",
                "BASSLINE changes the donk character. REVERB adds reverb lol. DECLICKER removes click from short notes or click problems; when enabled, STRENGTH changes how aggressive the smoothing is. "
                "VOLUME should be obvious ;-)\nTURN OFF ANIMATIONS stops the fancy movement (incase you dont like fancy animations and/or get dizzy :-D).",
                56);

            drawSection (
                "SIDECHAIN",
                "It should be obvious what it does :) AMOUNT changes how much it ducks and POSITION moves where the deepest dip happens inside the donk. "
                "If REVERB is also on, AFTER REVERB chooses whether the duck happens before or after the reverb.",
                48);

            drawSection (
                "MINIMALISTIC",
                "Makes StarDonk black and white, removes most fancy stuff and shows the real values instead of the DONK text. "
                "It can be used together with ADVANCED MODE.",
                34);

            drawSection (
                "ADVANCED MODE",
                "Keeps the normal StarDonk donk core, then opens FM2, FM3 and FM4, FM envelopes, TX Wave 2 blends, optional sub, amp envelope, resonant filter and distortion mix. "
                "FM1 is still the main donk. The extra operators colour the same carrier instead of playing separate layered notes. Everything can be changed manually and automated.",
                50);

            drawSection (
                "EXPERIMENTAL FUN",
                "try it out, its self explenatory i think",
                24);

            drawSection (
                "RANDOM",
                "RANDOM DONK generates a random donk. In ADVANCED MODE it can make a much wider range of donk families like hollow, bounce, metal shade, rubber, woody, glass, dirty, arcade, resonant and weird hybrids. EXPERIMENTAL CONTROL and EXPERIMENTAL RANDOM also work in ADVANCED MODE. Turning EXPERIMENTAL CONTROL off does not change the donk you already made. GIVE BACK DONK restores the sound from before the last random. "
                "DONK LOOP SPEED changes how fast DONK LOOP repeats (1.00 sec is standard and the slowest setting). "
                "EXPERIMENTAL CONTROL unlocks extreme knob ranges. EXPERIMENTAL RANDOM lets RANDOM DONK use those crazy ranges.",
                42);

            drawSection (
                "PRESETS",
                "SAVE writes .donk files. RANDOM NAME makes a blyatiful two-word name. Folders inside Documents\\StarDonk\\Presets can be browsed. "
                "R rescans the preset folder.",
                36);

            drawSection (
                "AUTOMATION",
                "Move a control, then in FL Studio use Tools > Last tweaked > Create automation clip. "
                "Advanced Mode controls are host-visible parameters too.",
                34);

            drawSection (
                "FL STUDIO",
                "To keep multiple StarDonk windows open at the same time: press F10 > General Settings and turn OFF Auto select linked modules. "
                "You can also hold ALT while opening another StarDonk instance.",
                42);

            drawSection (
                "FACE / FILES",
                "Right-click the picture to use StarSlav or a custom PNG.\n"
                "Presets: Documents\\StarDonk\\Presets\n"
                "Pictures: Documents\\StarDonk\\Pictures\n"
                "If you select a custom .png it will create a xml file in Documents\\StarDonk so you see your blyatiful picture everytime you load STARDONK :-)",
                70);
        }
    };

    StarDonkHelpComponent()
        : youtubeLink (
              "YouTube.com/@starslav",
              juce::URL ("https://www.youtube.com/@starslav")),
          instagramLink (
              "instagram.com/starslavhardbass",
              juce::URL ("https://instagram.com/starslavhardbass"))
    {
        setOpaque (false);
        closeButton.setButtonText ("X");
        closeButton.setWantsKeyboardFocus (false);
        closeButton.setMouseClickGrabsKeyboardFocus (false);
        closeButton.setColour (
            juce::TextButton::buttonColourId,
            juce::Colour (0xff211b2c));
        closeButton.setColour (
            juce::TextButton::textColourOffId,
            juce::Colour (0xffffd34e));
        closeButton.onClick = [this]()
            {
                if (onClose)
                    onClose();
            };

        youtubeLink.setColour (
            juce::HyperlinkButton::textColourId,
            juce::Colour (0xffffd34e));

        instagramLink.setColour (
            juce::HyperlinkButton::textColourId,
            juce::Colour (0xffffd34e));

        youtubeLink.setWantsKeyboardFocus (false);
        instagramLink.setWantsKeyboardFocus (false);

        helpViewport.setViewedComponent (
            &helpBody,
            false);

        helpViewport.setScrollBarsShown (
            true,
            false);

        helpViewport.setScrollOnDragMode (juce::Viewport::ScrollOnDragMode::all);
        helpViewport.setColour (
            juce::ScrollBar::thumbColourId,
            juce::Colour (0xff5a5368));

        addAndMakeVisible (closeButton);
        addAndMakeVisible (helpViewport);
        addAndMakeVisible (youtubeLink);
        addAndMakeVisible (instagramLink);
    }

    std::function<void()> onClose;

    void paint (juce::Graphics& g) override
    {
        g.setColour (juce::Colours::black.withAlpha (0.72f));
        g.fillRect (getLocalBounds());
        const auto panel = getPanelBounds();
        const auto panelF = panel.toFloat();
        g.setColour (juce::Colour (0xff100d17));
        g.fillRoundedRectangle (panelF, 14.0f);
        g.setColour (juce::Colour (0xffffcc33));
        g.drawRoundedRectangle (
            panelF.reduced (0.5f),
            14.0f,
            1.5f);

        g.setFont (juce::Font (juce::FontOptions()
                    .withName ("Arial")
                    .withHeight (22.0f)
                    .withStyle ("Bold")));

        g.setColour (juce::Colour (0xffffd34e));
        g.drawText (
            "STAR DONK - QUICK HELP",
            panel.getX() + 22,
            panel.getY() + 14,
            panel.getWidth() - 70,
            30,
            juce::Justification::centredLeft);

        g.setFont (juce::Font (juce::FontOptions()
                    .withName ("Arial")
                    .withHeight (12.0f)
                    .withStyle ("Bold")));

        g.setColour (juce::Colours::white);

        g.drawText (
            "YouTube:",
            panel.getX() + 22,
            panel.getBottom() - 57,
            70,
            22,
            juce::Justification::centredLeft);

        g.drawText (
            "Instagram:",
            panel.getX() + 22,
            panel.getBottom() - 31,
            70,
            22,
            juce::Justification::centredLeft);
    }

    void resized() override
    {
        const auto panel = getPanelBounds();

        closeButton.setBounds (
            panel.getRight() - 38,
            panel.getY() + 12,
            26,
            24);

        helpViewport.setBounds (
            panel.getX() + 16,
            panel.getY() + 50,
            panel.getWidth() - 32,
            panel.getHeight() - 122);

        helpBody.setSize (
            helpViewport.getWidth() - 14,
            940);

        youtubeLink.setBounds (
            panel.getX() + 92,
            panel.getBottom() - 57,
            panel.getWidth() - 120,
            22);

        instagramLink.setBounds (
            panel.getX() + 92,
            panel.getBottom() - 31,
            panel.getWidth() - 120,
            22);
    }

private:
    juce::Rectangle<int> getPanelBounds() const
    {
        return juce::Rectangle<int> (
            35,
            28,
            getWidth() - 70,
            550);
    }

    juce::TextButton closeButton;
    juce::Viewport helpViewport;
    HelpBodyComponent helpBody;
    juce::HyperlinkButton youtubeLink;
    juce::HyperlinkButton instagramLink;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StarDonkHelpComponent)
};

class NewProjectAudioProcessorEditor
    : public juce::AudioProcessorEditor,
      private juce::Timer
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;

    void paint (
        juce::Graphics&) override;

    void resized() override;

    void mouseDown (
        const juce::MouseEvent& event) override;

    void mouseUp (
        const juce::MouseEvent& event) override;

    bool keyPressed (
        const juce::KeyPress& key) override;

    bool keyStateChanged (
        bool isKeyDown) override;

private:
    NewProjectAudioProcessor& audioProcessor;
    DonkSlider pitchDropSlider;
    DonkSlider decaySlider;
    DonkSlider knockSlider;
    DonkSlider ratioSlider;
    DonkSlider shapeSlider;
    DonkSlider toneSlider;
    DonkSlider bodySlider;
    DonkSlider driveSlider;
    PercentSlider reverbSlider;
    PercentSlider declickerStrengthSlider;
    PercentSlider sidechainAmountSlider;
    PercentSlider sidechainPositionSlider;
    PercentSlider volumeSlider;
    juce::Label pitchDropLabel;
    juce::Label decayLabel;
    juce::Label knockLabel;
    juce::Label ratioLabel;
    juce::Label shapeLabel;
    juce::Label toneLabel;
    juce::Label bodyLabel;
    juce::Label driveLabel;
    juce::Label reverbLabel;
    juce::Label declickerStrengthLabel;
    juce::Label sidechainAmountLabel;
    juce::Label sidechainPositionLabel;
    juce::Label volumeLabel;
    juce::TextButton randomButton;
    juce::ToggleButton basslineToggle;

    // reverb av på
    juce::ToggleButton reverbToggle;

    // intern duck som triggas av noten
    juce::ToggleButton sidechainToggle;
    juce::ToggleButton sidechainAfterReverbToggle;

    // extra smoothing mot klick när noten triggas igen
    juce::ToggleButton declickerToggle;

    // donk loop grejen
    FocusSafeSlider donkLoopSpeedSlider;
    juce::Label donkLoopSpeedLabel;
    juce::TextButton donkLoopButton;

    // experiment kontroller
    juce::ToggleButton experimentalControlToggle;
    juce::ToggleButton experimentalRandomToggle;

    // läs om .donk filer från disken
    juce::TextButton refreshPresetButton;
    juce::TextButton presetBox;
    juce::TextButton previousPresetButton;
    juce::TextButton nextPresetButton;
    juce::TextButton savePresetButton;

    // hjälp o snabb instruktion typ
    juce::TextButton helpButton;

    // avskalad svart vit mode
    juce::ToggleButton minimalisticToggle;

    // alla synth motorns kontroller, funkar i normal o minimal mode
    juce::ToggleButton advancedModeToggle;
    std::array<FocusSafeSlider, NewProjectAudioProcessor::AdvancedParameterCount> advancedSliders;
    std::array<juce::Label, NewProjectAudioProcessor::AdvancedParameterCount> advancedLabels;
    juce::ToggleButton animationsOffToggle;

    // experiment kul grejer
    juce::ToggleButton experimentalFunToggle;
    StarDonkFunPanel funPanel;
    juce::ToggleButton semechkiRainToggle;
    juce::ToggleButton babushkaSafeToggle;
    juce::ToggleButton neighborMeterToggle;
    juce::ToggleButton redAlertToggle;
    juce::ToggleButton impactCracksToggle;
    juce::TextButton randomExplosionButton;
    juce::TextButton doNotPressButton;
    juce::TextButton memeButton;
    juce::TextButton closeFunPanelButton;
    juce::ToggleButton bassEarthquakeToggle;
    juce::ToggleButton screenShakeToggle;
    juce::ToggleButton faceSquashToggle;
    juce::ToggleButton ladaModeToggle;
    juce::ToggleButton discoModeToggle;
    juce::ToggleButton flyingPresetToggle;
    juce::TextButton giveBackDonkButton;
    std::unique_ptr<PresetGridComponent> presetGrid;
    std::unique_ptr<StarDonkHelpComponent> helpPanel;
    juce::Image starSlavImage;
    juce::Image customFaceImage;
    juce::Image semechkiImage;
    std::unique_ptr<juce::FileChooser> faceFileChooser;
    float starEnergy = 0.0f;
    float starX[45] {};
    float starY[45] {};
    float starSpeed[45] {};
    float starSize[45] {};
    float faceBounce = 0.0f;
    uint64_t lastNoteTrigger = 0;
    uint64_t funSeenTrigger = 0;

    struct DonkSnapshot
    {
        float pitchDrop = 0.0f;
        float decay = 0.0f;
        float knock = 0.0f;
        float ratio = 0.0f;
        float shape = 0.0f;
        float tone = 0.0f;
        float body = 0.0f;
        float drive = 0.0f;
        bool experimentalControl = false;
        bool experimentalRandom = false;
        std::array<float, NewProjectAudioProcessor::AdvancedParameterCount> advanced {};
    };
    DonkSnapshot previousRandomSnapshot;
    DonkSnapshot earthquakeSnapshot;
    DonkSnapshot doNotPressSnapshot;
    bool hasPreviousRandom = false;
    bool earthquakeHasSnapshot = false;
    bool doNotPressRunning = false;
    int doNotPressFrames = 0;
    int doNotPressRandomFrames = 0;
    int earthquakeReturnFrames = 0;
    bool memePanelOpen = false;
    float discoLife = 0.0f;
    float faceSquashLife = 0.0f;
    float wideBoiWidth = 1.0f;
    float wideBoiHeight = 1.0f;
    float wideBoiTargetWidth = 1.0f;
    float wideBoiTargetHeight = 1.0f;
    float wideBoiPhase = 0.0f;
    float uiShakeX = 0.0f;
    float uiShakeY = 0.0f;
    float discoHue = 0.0f;
    int flyingPresetSpawnFrames = 0;

    struct ImpactCrack
    {
        float x = 0.0f;
        float y = 0.0f;
        float size = 100.0f;
        float rotation = 0.0f;
        float life = 1.0f;
        int branches = 7;
        float jitter = 0.0f;
    };

    struct FlyingPreset
    {
        juce::String text;
        float x = 0.0f;
        float y = 0.0f;
        float vx = 0.0f;
        float vy = 0.0f;
        float life = 1.0f;
        float angle = 0.0f;
        float fontSize = 24.0f;
    };
    juce::Array<ImpactCrack> impactCracks;
    juce::Array<FlyingPreset> flyingPresets;

    struct FunParticle
    {
        float x = 0.0f;
        float y = 0.0f;
        float vx = 0.0f;
        float vy = 0.0f;
        float life = 0.0f;
        bool text = false;
    };
    juce::Array<FunParticle> funParticles;
    juce::Random funRandom;
    juce::String currentPresetFolder;
    juce::Array<int> presetGridTargetIndices;
    juce::StringArray presetGridTargetFolders;

    void setupSlider (
        juce::Slider& slider,
        juce::Label& label,
        const juce::String& labelText);

    void setupButton (
        juce::TextButton& button,
        const juce::String& text);

    void refreshPresetBox();
    void updateSlidersFromProcessor();
    void randomize();
    void previousPreset();
    void nextPreset();
    void savePreset();
    void refreshPresetsFromDisk();
    void showPresetGrid();
    void hidePresetGrid();
    void showHelpPanel();
    void hideHelpPanel();
    void updateLoopButton();
    void updateExperimentalControls();
    void applyExperimentalSliderRanges();
    void applyMinimalisticMode();
    void setupAdvancedControls();
    void updateAdvancedMode();
    void updateAdvancedControlsFromProcessor();
    DonkSnapshot captureDonkSnapshot() const;
    void restoreDonkSnapshot (const DonkSnapshot& snapshot);
    void giveBackDonk();
    void updateExperimentalFunVisibility();
    void disableAllExperimentalFun();
    void updateMemeVisibility();
    void triggerRandomExplosion();
    void startDoNotPress();
    void stopDoNotPress (bool restoreSound);
    void triggerBassEarthquake();
    void restoreBassEarthquake();
    void triggerFlyingPresetName();
    void spawnImpactCrack();
    void closeExperimentalFunPanel();
    void applyChildShakeTransform();
    juce::String makeRandomPresetName();
    void savePresetWithSuggestedName (const juce::String& suggestedName);
    void loadFaceImages();
    void showFaceMenu();
    void chooseCustomFace();
    void selectFaceMode (NewProjectAudioProcessor::FaceMode mode);

    juce::Image loadFaceImageFromFile (
        const juce::File& file) const;

    void updateComputerKeyboardState();
    int getComputerKeyboardNoteForIndex (int index) const;
    std::array<bool, 24> computerKeyboardHeld {};
    std::array<uint64_t, 24> computerKeyboardPressOrder {};
    uint64_t computerKeyboardOrderCounter = 0;

    juce::Rectangle<int> getFaceClickBounds() const;
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
