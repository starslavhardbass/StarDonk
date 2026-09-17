#pragma once
#include <array>

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <memory>
#include <functional>

class DonkSlider : public juce::Slider
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

class PercentSlider : public juce::Slider
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

class StarDonkHelpComponent
    : public juce::Component,
      private juce::Thread
{
public:
    class HelpBodyComponent : public juce::Component
    {
    public:
        HelpBodyComponent()
        {
            setSize (540, 1220);
        }

        void setMinimalistic (bool shouldBeMinimal)
        {
            if (minimalisticMode == shouldBeMinimal)
                return;

            minimalisticMode = shouldBeMinimal;
            repaint();
        }

        void paint (juce::Graphics& g) override
        {
            int y = 8;
            const int x = 8;
            const int w = getWidth() - 22;

            const auto titleColour = minimalisticMode
                ? juce::Colours::white
                : juce::Colour (0xffffd34e);

            const auto bodyColour = minimalisticMode
                ? juce::Colour (0xffd8d8d8)
                : juce::Colour (0xffb9b3c6);

            auto drawSection = [&] (const juce::String& title,
                     const juce::String& body,
                     int bodyHeight)
                {
                    g.setFont (juce::Font (juce::FontOptions()
                                .withName ("Arial")
                                .withHeight (12.0f)
                                .withStyle ("Bold")));

                    g.setColour (titleColour);
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

                    g.setColour (bodyColour);
                    g.drawFittedText (
                        body,
                        juce::Rectangle<int> (x, y, w, bodyHeight),
                        juce::Justification::topLeft,
                        20,
                        1.0f);

                    y += bodyHeight + 12;
                };

            drawSection (
                "KNOBS",
                "CYKA = pitch drop   |   BLYAT = decay   |   DONK = FM knock   |   GOPNIK = FM ratio\n"
                "KVASS = waveform   |   SEMECHKI = tone   |   HARD = body/sustain   |   BASS = drive",
                44);

            drawSection (
                "EXTRA",
                "BASSLINE changes the donk character. REVERB adds reverb lol. DECLICKER removes click from short notes or click problems; when enabled, STRENGTH changes how aggressive the smoothing is. "
                "VOLUME should be obvious ;-)\nTURN OFF ANIMATIONS stops the fancy movement (incase you dont like fancy animations and/or get dizzy :-D).",
                82);

            drawSection (
                "SIDECHAIN",
                "It should be obvious what it does :) AMOUNT changes how much it ducks and POSITION moves where the deepest dip happens inside the donk. "
                "If REVERB is also on, AFTER REVERB chooses whether the duck happens before or after the reverb.",
                66);

            drawSection (
                "MINIMALISTIC",
                "Makes StarDonk black and white, removes most fancy stuff and shows the real values instead of the DONK text. "
                "It can be used together with ADVANCED MODE.",
                48);

            drawSection (
                "ADVANCED MODE",
                "Keeps the normal StarDonk donk core, then opens FM2, FM3 and FM4, FM envelopes, TX Wave 2 blends, optional sub, amp envelope, resonant filter and distortion mix. "
                "FM1 is still the main donk. The extra operators colour the same carrier instead of playing separate layered notes. Everything can be changed manually and automated.",
                74);

            drawSection (
                "EXPERIMENTAL FUN",
                "try it out, its self explenatory i think",
                30);

            drawSection (
                "RANDOM",
                "RANDOM DONK generates a random donk. In ADVANCED MODE it can make a much wider range of donk families like hollow, bounce, metal shade, rubber, woody, glass, dirty, arcade, resonant and weird hybrids. EXPERIMENTAL CONTROL and EXPERIMENTAL RANDOM also work in ADVANCED MODE. Turning EXPERIMENTAL CONTROL off does not change the donk you already made. GIVE BACK DONK restores the sound from before the last random. "
                "DONK LOOP SPEED changes how fast DONK LOOP repeats (1.00 sec is standard and the slowest setting). "
                "EXPERIMENTAL CONTROL unlocks extreme knob ranges. EXPERIMENTAL RANDOM lets RANDOM DONK use those crazy ranges.",
                112);

            drawSection (
                "PRESETS",
                "SAVE writes .donk files. RANDOM NAME makes a blyatiful two-word name. Folders inside Documents\\StarDonk\\Presets can be browsed. "
                "R rescans the preset folder.",
                50);

            drawSection (
                "AUTOMATION",
                "Move a control, then in FL Studio use Tools > Last tweaked > Create automation clip. "
                "Advanced Mode controls are host-visible parameters too.",
                46);

            drawSection (
                "FL STUDIO",
                "To keep multiple StarDonk windows open at the same time: press F10 > General Settings and turn OFF Auto select linked modules. "
                "You can also hold ALT while opening another StarDonk instance.",
                58);

            drawSection (
                "FACE / FILES",
                "Right-click the picture to use StarSlav or a custom PNG.\n"
                "Presets: Documents\\StarDonk\\Presets\n"
                "Pictures: Documents\\StarDonk\\Pictures\n"
                "Exports: Documents\\StarDonk\\Exports\n"
                "If you select a custom .png it will create a xml file in Documents\\StarDonk so you see your blyatiful picture everytime you load STARDONK :-)",
                112);

            drawSection (
                "ROOT / OCTAVE / EXPORT",
                "DETECTED ROOT follows the current donk sound. ROOT NOTE changes how the donk is transposed. MATCH ROOT instantly sets ROOT NOTE to the detected root and keeps following it while the sound changes. Turning MATCH ROOT off returns ROOT NOTE to C. "
                "OCTAVE moves the donk by whole octaves while staying on the same note name. RANDOM DONK normally resets OCTAVE to 0; FREEZE OCTAVE keeps your chosen octave. EXPORT WAV renders the current donk.",
                92);
        }

    private:
        bool minimalisticMode = false;
    };

    StarDonkHelpComponent()
        : juce::Thread ("StarDonk Update Check"),
          youtubeLink (
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
        closeButton.onClick = [this]()
            {
                if (onClose)
                    onClose();
            };

        versionLabel.setText (
            "Version " + getStarDonkVersion(),
            juce::dontSendNotification);

        versionLabel.setFont (juce::Font (juce::FontOptions()
                    .withName ("Arial")
                    .withHeight (11.5f)
                    .withStyle ("Bold")));

        versionLabel.setJustificationType (
            juce::Justification::centredLeft);

        updateStatusLabel.setText (
            "Manual update check - nothing downloads automatically.",
            juce::dontSendNotification);

        updateStatusLabel.setFont (juce::Font (juce::FontOptions()
                    .withName ("Arial")
                    .withHeight (10.5f)));

        updateStatusLabel.setJustificationType (
            juce::Justification::centredLeft);

        checkUpdateButton.setButtonText ("CHECK FOR UPDATES");
        checkUpdateButton.setWantsKeyboardFocus (false);
        checkUpdateButton.setMouseClickGrabsKeyboardFocus (false);
        checkUpdateButton.onClick = [this]()
            {
                if (updateAvailable)
                {
                    juce::URL (latestReleaseUrl).launchInDefaultBrowser();

                    updateStatusLabel.setText (
                        "GitHub opened. Close your DAW, optionally back up the old VST3, then replace StarDonk.vst3.",
                        juce::dontSendNotification);
                    return;
                }

                beginUpdateCheck();
            };

        youtubeLink.setWantsKeyboardFocus (false);
        instagramLink.setWantsKeyboardFocus (false);

        helpViewport.setViewedComponent (
            &helpBody,
            false);

        helpViewport.setScrollBarsShown (
            true,
            false);

        helpViewport.setScrollOnDragEnabled (true);

        addAndMakeVisible (closeButton);
        addAndMakeVisible (helpViewport);
        addAndMakeVisible (versionLabel);
        addAndMakeVisible (updateStatusLabel);
        addAndMakeVisible (checkUpdateButton);
        addAndMakeVisible (youtubeLink);
        addAndMakeVisible (instagramLink);

        applyColours();
    }

    ~StarDonkHelpComponent() override
    {
        signalThreadShouldExit();
        stopThread (6000);
    }

    std::function<void()> onClose;

    void setMinimalistic (bool shouldBeMinimal)
    {
        if (minimalisticMode == shouldBeMinimal)
            return;

        minimalisticMode = shouldBeMinimal;
        helpBody.setMinimalistic (minimalisticMode);
        applyColours();
        repaint();
    }

    void paint (juce::Graphics& g) override
    {
        g.setColour (juce::Colours::black.withAlpha (0.72f));
        g.fillRect (getLocalBounds());

        const auto panel = getPanelBounds();
        const auto panelF = panel.toFloat();

        const auto panelColour = minimalisticMode
            ? juce::Colours::black
            : juce::Colour (0xff100d17);

        const auto accentColour = minimalisticMode
            ? juce::Colours::white
            : juce::Colour (0xffffcc33);

        g.setColour (panelColour);
        g.fillRoundedRectangle (panelF, 14.0f);
        g.setColour (accentColour);
        g.drawRoundedRectangle (
            panelF.reduced (0.5f),
            14.0f,
            1.5f);

        g.setFont (juce::Font (juce::FontOptions()
                    .withName ("Arial")
                    .withHeight (22.0f)
                    .withStyle ("Bold")));

        g.setColour (accentColour);
        g.drawText (
            "STAR DONK - QUICK HELP",
            panel.getX() + 22,
            panel.getY() + 14,
            panel.getWidth() - 70,
            30,
            juce::Justification::centredLeft);

        const int footerTop = panel.getBottom() - 96;

        g.setColour (minimalisticMode
            ? juce::Colour (0xff5a5a5a)
            : juce::Colour (0xff30293d));

        g.drawHorizontalLine (
            footerTop,
            static_cast<float> (panel.getX() + 18),
            static_cast<float> (panel.getRight() - 18));

        g.setFont (juce::Font (juce::FontOptions()
                    .withName ("Arial")
                    .withHeight (12.0f)
                    .withStyle ("Bold")));

        g.setColour (juce::Colours::white);

        g.drawText (
            "YouTube:",
            panel.getX() + 22,
            footerTop + 43,
            70,
            22,
            juce::Justification::centredLeft);

        g.drawText (
            "Instagram:",
            panel.getX() + 22,
            footerTop + 67,
            70,
            22,
            juce::Justification::centredLeft);
    }

    void resized() override
    {
        const auto panel = getPanelBounds();
        const int footerTop = panel.getBottom() - 96;

        closeButton.setBounds (
            panel.getRight() - 38,
            panel.getY() + 12,
            26,
            24);

        helpViewport.setBounds (
            panel.getX() + 16,
            panel.getY() + 50,
            panel.getWidth() - 32,
            footerTop - (panel.getY() + 56));

        helpBody.setSize (
            juce::jmax (100, helpViewport.getWidth() - 14),
            1220);

        versionLabel.setBounds (
            panel.getX() + 22,
            footerTop + 7,
            120,
            24);

        checkUpdateButton.setBounds (
            panel.getRight() - 178,
            footerTop + 6,
            156,
            25);

        updateStatusLabel.setBounds (
            panel.getX() + 145,
            footerTop + 6,
            juce::jmax (80, panel.getWidth() - 350),
            26);

        youtubeLink.setBounds (
            panel.getX() + 92,
            footerTop + 43,
            panel.getWidth() - 120,
            22);

        instagramLink.setBounds (
            panel.getX() + 92,
            footerTop + 67,
            panel.getWidth() - 120,
            22);
    }

private:
    static juce::String getStarDonkVersion()
    {
        return "1.0.1";
    }

    static juce::String cleanVersionString (juce::String version)
    {
        version = version.trim();

        while (version.startsWithIgnoreCase ("v"))
            version = version.substring (1);

        const int dash = version.indexOfChar ('-');
        if (dash >= 0)
            version = version.substring (0, dash);

        return version;
    }

    static bool isVersionNewer (
        const juce::String& possibleNewVersion,
        const juce::String& currentVersion)
    {
        const auto newVersion = cleanVersionString (possibleNewVersion);
        const auto oldVersion = cleanVersionString (currentVersion);

        juce::StringArray newParts;
        juce::StringArray oldParts;
        newParts.addTokens (newVersion, ".", "");
        oldParts.addTokens (oldVersion, ".", "");

        const int count = juce::jmax (newParts.size(), oldParts.size());

        for (int i = 0; i < count; ++i)
        {
            const int newer = i < newParts.size()
                ? newParts[i].getIntValue()
                : 0;

            const int older = i < oldParts.size()
                ? oldParts[i].getIntValue()
                : 0;

            if (newer != older)
                return newer > older;
        }

        return false;
    }

    void beginUpdateCheck()
    {
        if (isThreadRunning())
            return;

        updateAvailable = false;
        latestVersionTag.clear();
        checkUpdateButton.setEnabled (false);
        checkUpdateButton.setButtonText ("CHECKING...");
        updateStatusLabel.setText (
            "Checking GitHub...",
            juce::dontSendNotification);

        startThread();
    }

    void run() override
    {
        juce::String foundTag;
        juce::String errorMessage;
        int statusCode = 0;

        const juce::URL apiUrl (
            "https://api.github.com/repos/starslavhardbass/StarDonk/releases/latest");

        const auto options = juce::URL::InputStreamOptions (
                juce::URL::ParameterHandling::inAddress)
            .withConnectionTimeoutMs (5000)
            .withNumRedirectsToFollow (5)
            .withStatusCode (&statusCode)
            .withExtraHeaders (
                "User-Agent: StarDonk-VST3\r\n"
                "Accept: application/vnd.github+json\r\n");

        if (auto stream = apiUrl.createInputStream (options))
        {
            if (! threadShouldExit())
            {
                const auto reply = stream->readEntireStreamAsString();
                const auto parsed = juce::JSON::parse (reply);

                if (auto* object = parsed.getDynamicObject())
                {
                    foundTag = object->getProperty ("tag_name").toString().trim();

                    if (foundTag.isEmpty())
                        errorMessage = "GitHub returned a release without a version tag.";
                }
                else
                {
                    errorMessage = "Could not read the GitHub release information.";
                }
            }
        }
        else
        {
            errorMessage = statusCode > 0
                ? "Could not check GitHub (HTTP " + juce::String (statusCode) + ")."
                : "Could not connect to GitHub.";
        }

        if (threadShouldExit())
            return;

        auto safeThis = juce::Component::SafePointer<StarDonkHelpComponent> (this);

        juce::MessageManager::callAsync (
            [safeThis, foundTag, errorMessage]() mutable
            {
                if (safeThis != nullptr)
                    safeThis->finishUpdateCheck (foundTag, errorMessage);
            });
    }

    void finishUpdateCheck (
        const juce::String& foundTag,
        const juce::String& errorMessage)
    {
        checkUpdateButton.setEnabled (true);

        if (errorMessage.isNotEmpty())
        {
            updateAvailable = false;
            checkUpdateButton.setButtonText ("CHECK AGAIN");
            updateStatusLabel.setText (
                errorMessage,
                juce::dontSendNotification);
            return;
        }

        latestVersionTag = foundTag;

        const auto currentVersion = getStarDonkVersion();
        updateAvailable = isVersionNewer (
            latestVersionTag,
            currentVersion);

        if (updateAvailable)
        {
            checkUpdateButton.setButtonText ("DOWNLOAD UPDATE");
            updateStatusLabel.setText (
                "New version: " + latestVersionTag,
                juce::dontSendNotification);

            juce::AlertWindow::showMessageBoxAsync (
                juce::MessageBoxIconType::InfoIcon,
                "StarDonk update available",
                "You have StarDonk v" + cleanVersionString (currentVersion)
                    + " and " + latestVersionTag + " is available.\n\n"
                      "There might will be some new presets ;)\n\n"
                      "Press DOWNLOAD UPDATE to open the GitHub release page.\n\n"
                      "To update: close your DAW, optionally copy your old StarDonk.vst3 somewhere outside the VST3 folder as a backup, then replace the installed StarDonk.vst3 with the new one."
            );
        }
        else
        {
            checkUpdateButton.setButtonText ("CHECK AGAIN");
            updateStatusLabel.setText (
                "You have the latest version.",
                juce::dontSendNotification);
        }
    }

    void applyColours()
    {
        const auto accent = minimalisticMode
            ? juce::Colours::white
            : juce::Colour (0xffffd34e);

        const auto buttonBackground = minimalisticMode
            ? juce::Colours::black
            : juce::Colour (0xff211b2c);

        const auto secondaryText = minimalisticMode
            ? juce::Colour (0xffd8d8d8)
            : juce::Colour (0xffb9b3c6);

        closeButton.setColour (
            juce::TextButton::buttonColourId,
            buttonBackground);
        closeButton.setColour (
            juce::TextButton::textColourOffId,
            accent);

        checkUpdateButton.setColour (
            juce::TextButton::buttonColourId,
            buttonBackground);
        checkUpdateButton.setColour (
            juce::TextButton::textColourOffId,
            accent);

        versionLabel.setColour (
            juce::Label::textColourId,
            accent);

        updateStatusLabel.setColour (
            juce::Label::textColourId,
            secondaryText);

        youtubeLink.setColour (
            juce::HyperlinkButton::textColourId,
            accent);

        instagramLink.setColour (
            juce::HyperlinkButton::textColourId,
            accent);

        helpViewport.setColour (
            juce::ScrollBar::thumbColourId,
            minimalisticMode
                ? juce::Colour (0xffbdbdbd)
                : juce::Colour (0xff5a5368));
    }

    juce::Rectangle<int> getPanelBounds() const
    {
        const int panelHeight = juce::jmin (
            590,
            juce::jmax (360, getHeight() - 56));

        return juce::Rectangle<int> (
            35,
            28,
            getWidth() - 70,
            panelHeight);
    }

    bool minimalisticMode = false;
    bool updateAvailable = false;
    juce::String latestVersionTag;

    const juce::String latestReleaseUrl =
        "https://github.com/starslavhardbass/StarDonk/releases/latest";

    juce::TextButton closeButton;
    juce::Viewport helpViewport;
    HelpBodyComponent helpBody;
    juce::Label versionLabel;
    juce::Label updateStatusLabel;
    juce::TextButton checkUpdateButton;
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

    bool keyPressed (
        const juce::KeyPress& key) override;

    bool keyStateChanged (
        bool isKeyDown) override;

    void focusLost (
        juce::Component::FocusChangeType cause) override;

    void mouseDown (
        const juce::MouseEvent& event) override;

    void mouseUp (
        const juce::MouseEvent& event) override;

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
    juce::Slider donkLoopSpeedSlider;
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

    // root grejer: detected e bara info, ROOT NOTE transponerar playback men petar inte rattar/preset värden
    juce::Label detectedRootTitleLabel;
    juce::Label detectedRootValueLabel;
    juce::Label assignedRootTitleLabel;
    juce::ComboBox assignedRootBox;
    juce::ToggleButton matchRootToggle;
    juce::Label octaveTitleLabel;
    juce::ComboBox octaveBox;
    juce::ToggleButton freezeOctaveToggle;
    juce::TextButton exportWavButton;
    std::unique_ptr<juce::FileChooser> wavFileChooser;

    // qwerty fallback när FL tappar sitt egna typing keyboard fokus
    int computerKeyboardHeldKeyCode = 0;
    int computerKeyboardHeldMidiNote = -1;
    int midiNoteForComputerKey (const juce::KeyPress& key) const;
    void releaseComputerKeyboardNote();

    // detected root ska följa soundet live men inte spamma analys varje frame
    bool rootAnalysisSignatureReady = false;
    bool rootAnalysisPending = false;
    std::size_t lastRootAnalysisSignature = 0;
    double lastRootAnalysisTimeMs = 0.0;

    // hjälp o snabb instruktion typ
    juce::TextButton helpButton;

    // avskalad svart vit mode
    juce::ToggleButton minimalisticToggle;

    // alla synth motorns kontroller, funkar i normal o minimal mode
    juce::ToggleButton advancedModeToggle;
    std::array<juce::Slider, NewProjectAudioProcessor::AdvancedParameterCount> advancedSliders;
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
    void refreshRootControls (bool analyseIfMissing);
    std::size_t getRootAnalysisSignature() const;
    void updateLiveRootDetection();
    void exportDonkAsWav();
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

    juce::Rectangle<int> getFaceClickBounds() const;
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
