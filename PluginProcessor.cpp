#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
    juce::StringArray createRootNoteChoices()
    {
        static const char* noteNames[] =
        {
            "C", "C#", "D", "D#", "E", "F",
            "F#", "G", "G#", "A", "A#", "B"
        };
        juce::StringArray choices;

        for (int midiNote = 36; midiNote <= 84; ++midiNote)
        {
            const int noteInOctave = midiNote % 12;
            const int octave = midiNote / 12;

            choices.add (juce::String (noteNames[noteInOctave])
                + juce::String (octave));
        }

        return choices;
    }

    struct AdvancedParameterDefinition
    {
        const char* id;
        const char* name;
        float minimum;
        float maximum;
        float interval;
        float defaultValue;
    };

    static const AdvancedParameterDefinition advancedParameterDefinitions[] =
    {
        { "carrierLevel", "Carrier Level", 0.0f, 1.50f, 0.001f, 1.00f },
        { "subLevel",     "Sub Level",     0.0f, 1.50f, 0.001f, 0.10f },
        { "subOctave",    "Sub Octave",   -2.0f, 0.0f,  1.0f,  -1.0f },
        { "subDecay",     "Sub Decay",     0.02f, 2.0f,  0.001f, 0.45f },
        { "subSustain",   "Sub Sustain",   0.0f, 1.0f,   0.001f, 0.38f },
        { "subShape",     "Sub Shape",     0.0f, 1.0f,   0.001f, 0.00f },

        { "fm1Decay",     "FM1 Decay",     0.005f, 1.50f, 0.001f, 0.16f },
        { "fm1Sustain",   "FM1 Sustain",   0.0f, 1.0f,   0.001f, 0.00f },
        { "fm1Shape",     "FM1 Shape",     0.0f, 1.0f,   0.001f, 0.00f },

        { "fm2Ratio",     "FM2 Ratio",     0.25f, 16.0f, 0.01f, 3.0f },
        { "fm2Amount",    "FM2 Amount",    0.0f, 2.50f,  0.001f, 0.00f },
        { "fm2Decay",     "FM2 Decay",     0.005f, 2.0f, 0.001f, 0.28f },
        { "fm2Sustain",   "FM2 Sustain",   0.0f, 1.0f,   0.001f, 0.00f },
        { "fm2Shape",     "FM2 Shape",     0.0f, 1.0f,   0.001f, 0.00f },

        { "fm3Ratio",     "FM3 Ratio",     0.25f, 24.0f, 0.01f, 8.0f },
        { "fm3Amount",    "FM3 Amount",    0.0f, 2.50f,  0.001f, 0.00f },
        { "fm3Decay",     "FM3 Decay",     0.003f, 1.0f, 0.001f, 0.035f },
        { "fm3Sustain",   "FM3 Sustain",   0.0f, 1.0f,   0.001f, 0.00f },
        { "fm3Shape",     "FM3 Shape",     0.0f, 1.0f,   0.001f, 0.00f },

        { "fm4Ratio",     "FM4 Ratio",     0.25f, 16.0f, 0.01f, 4.0f },
        { "fm4Amount",    "FM4 Amount",    0.0f, 2.50f,  0.001f, 0.00f },
        { "fm4Decay",     "FM4 Decay",     0.005f, 2.0f, 0.001f, 0.12f },
        { "fm4Sustain",   "FM4 Sustain",   0.0f, 1.0f,   0.001f, 0.00f },
        { "fm4Shape",     "FM4 Shape",     0.0f, 1.0f,   0.001f, 0.00f },

        { "ampAttack",    "Amp Attack",    0.0005f, 0.20f, 0.0005f, 0.0015f },
        { "ampDecay",     "Amp Decay",     0.02f, 2.50f, 0.001f, 0.34f },
        { "ampSustain",   "Amp Sustain",   0.0f, 1.0f,   0.001f, 0.34f },
        { "ampRelease",   "Amp Release",   0.01f, 2.0f,  0.001f, 0.09f },

        { "pitchDecay",   "Pitch Decay",   0.005f, 0.60f, 0.001f, 0.065f },
        { "distMix",      "Dist Mix",      0.0f, 1.0f,   0.001f, 1.00f },

        { "carrierTx2",   "Carrier TX Wave 2", 0.0f, 1.0f, 0.001f, 0.0f },
        { "fm1Tx2",       "FM1 TX Wave 2",     0.0f, 1.0f, 0.001f, 0.00f },
        { "fm2Tx2",       "FM2 TX Wave 2",     0.0f, 1.0f, 0.001f, 0.00f },
        { "fm3Tx2",       "FM3 TX Wave 2",     0.0f, 1.0f, 0.001f, 0.0f },
        { "fm4Tx2",       "FM4 TX Wave 2",     0.0f, 1.0f, 0.001f, 0.00f },

        { "classicBlend", "Legacy Classic Mix",     0.0f, 1.0f,  0.001f, 0.0f },
        { "classicCRatio","Legacy Classic C Ratio", 1.0f, 32.0f, 0.01f, 32.0f },
        { "classicCDecay","Legacy Classic C Decay", 0.005f, 1.0f,0.001f, 0.03f },
        { "classicCToA",  "Legacy C to A",          0.0f, 30.0f, 0.01f, 0.0f },
        { "classicCToB",  "Legacy C to B",          0.0f, 30.0f, 0.01f, 0.0f },
        { "classicCToE",  "Legacy C to E",          0.0f, 20.0f, 0.01f, 0.0f },
        { "classicBRatio","Legacy Classic B Ratio", 0.5f, 8.0f,  0.01f, 2.0f },
        { "classicERatio","Legacy Classic E Ratio", 0.5f, 4.0f,  0.01f, 1.0f },
        { "classicBLevel","Legacy Classic B Level", 0.0f, 1.5f,  0.001f, 0.0f },
        { "classicELevel","Legacy Classic E Level", 0.0f, 1.5f,  0.001f, 0.0f },

        { "filterResonance", "Filter Resonance", 0.0f, 0.92f, 0.001f, 0.00f }
    };

    static_assert (static_cast<int> (sizeof (advancedParameterDefinitions)
            / sizeof (advancedParameterDefinitions[0]))
            == NewProjectAudioProcessor::AdvancedParameterCount,
        "Advanced parameter table mismatch");

}

NewProjectAudioProcessor::NewProjectAudioProcessor()

#ifndef JucePlugin_PreferredChannelConfigurations

    : AudioProcessor (BusesProperties()

       #if ! JucePlugin_IsMidiEffect

        #if ! JucePlugin_IsSynth

            .withInput ("Input", juce::AudioChannelSet::stereo(), true)

        #endif

            .withOutput ("Output", juce::AudioChannelSet::stereo(), true)

       #endif
    )

#endif
{

    addParameter (
        rootNoteParameter = new juce::AudioParameterChoice (
                juce::ParameterID { "rootNote", 1 },
                "Root Note",
                createRootNoteChoices(),
                rootNoteNeutral - rootNoteMinimum));

    addParameter (pitchDropParameter = new juce::AudioParameterFloat (
        juce::ParameterID { "pitchDrop", 1 }, "CYKA - Pitch Drop",
        juce::NormalisableRange<float> (-48.0f, 72.0f, 0.01f), 7.0f));

    addParameter (decayParameter = new juce::AudioParameterFloat (
        juce::ParameterID { "decay", 1 }, "BLYAT - Decay",
        juce::NormalisableRange<float> (0.01f, 3.0f, 0.001f), 0.28f));

    addParameter (knockParameter = new juce::AudioParameterFloat (
        juce::ParameterID { "knock", 1 }, "DONK - Knock",
        juce::NormalisableRange<float> (-12.0f, 18.0f, 0.01f), 2.0f));

    addParameter (ratioParameter = new juce::AudioParameterFloat (
        juce::ParameterID { "ratio", 1 }, "GOPNIK - FM Ratio",
        juce::NormalisableRange<float> (-8.0f, 12.0f, 0.01f), 2.0f));

    addParameter (shapeParameter = new juce::AudioParameterFloat (
        juce::ParameterID { "shape", 1 }, "KVASS - Shape",
        juce::NormalisableRange<float> (-1.0f, 2.0f, 0.01f), 0.20f));

    addParameter (toneParameter = new juce::AudioParameterFloat (
        juce::ParameterID { "tone", 1 }, "SEMECHKI - Tone",
        juce::NormalisableRange<float> (-0.03f, 1.50f, 0.001f), 0.40f));

    addParameter (bodyParameter = new juce::AudioParameterFloat (
        juce::ParameterID { "body", 1 }, "HARD - Body",
        juce::NormalisableRange<float> (-0.25f, 1.50f, 0.001f), 0.50f));

    addParameter (driveParameter = new juce::AudioParameterFloat (
        juce::ParameterID { "drive", 1 }, "BASS - Drive",
        juce::NormalisableRange<float> (-8.0f, 16.0f, 0.01f), 2.0f));

    addParameter (basslineParameter = new juce::AudioParameterBool (
        juce::ParameterID { "bassline", 1 }, "Bassline", true));

    addParameter (reverbEnabledParameter = new juce::AudioParameterBool (
        juce::ParameterID { "reverbEnabled", 1 }, "Reverb Enabled", false));

    addParameter (reverbMixParameter = new juce::AudioParameterFloat (
        juce::ParameterID { "reverbMix", 1 }, "Reverb Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));

    addParameter (sidechainEnabledParameter = new juce::AudioParameterBool (
        juce::ParameterID { "sidechainEnabled", 1 }, "Sidechain Enabled", false));

    addParameter (sidechainAmountParameter = new juce::AudioParameterFloat (
        juce::ParameterID { "sidechainAmount", 1 }, "Sidechain Amount",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.65f));

    addParameter (sidechainPositionParameter = new juce::AudioParameterFloat (
        juce::ParameterID { "sidechainPosition", 1 }, "Sidechain Position",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.0f));

    addParameter (volumeParameter = new juce::AudioParameterFloat (
        juce::ParameterID { "volume", 1 }, "Volume",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 1.0f));

    addParameter (declickerParameter = new juce::AudioParameterBool (
        juce::ParameterID { "declicker", 1 }, "Declicker", false));

    for (int i = 0; i < AdvancedParameterCount; ++i)
    {
        const auto& definition = advancedParameterDefinitions[i];
        advancedValues[i].store (definition.defaultValue);

        addParameter (
            advancedParameters[i] = new juce::AudioParameterFloat (
                    juce::ParameterID { definition.id, 1 },
                    definition.name,
                    juce::NormalisableRange<float> (
                        definition.minimum,
                        definition.maximum,
                        definition.interval),
                    definition.defaultValue));
    }

    addParameter (
        sidechainAfterReverbParameter = new juce::AudioParameterBool (
                juce::ParameterID { "sidechainAfterReverb", 1 },
                "Sidechain After Reverb",
                false));

    addParameter (
        declickerStrengthParameter = new juce::AudioParameterFloat (
                juce::ParameterID { "declickerStrength", 1 },
                "Declicker Strength",
                juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
                0.35f));

    presets.clear();
    refreshUserPresets();
    loadGlobalSettings();
    voice.setBasslineEnabled (basslineEnabled.load());

    // loopen ska alltid va av när pluginet startar
    donkLoopEnabled.store (false);
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 3.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String
NewProjectAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index);
    juce::ignoreUnused (newName);
}

void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    voice.prepare (sampleRate);
    voice.setBasslineEnabled (basslineEnabled.load());
    voice.reset();
    reverb.setSampleRate (sampleRate);
    reverb.reset();
    sidechainSampleCounter = 0;
    sidechainSmoothedGain = 1.0f;

    sidechainGainScratch.setSize (
        1,
        juce::jmax (samplesPerBlock, 65536),
        false,
        false,
        true);

    sidechainGainScratch.clear();
    audioLoopWasEnabled = false;
    loopSampleCounter = 0;
    loopNoteCounter = 0;
    lastSynthOutput = 0.0f;
    declickStartOutput = 0.0f;
    declickSamplesRemaining = 0;
    declickTotalSamples = 0;
    donkActivity.store (0.0f);
}

void NewProjectAudioProcessor::releaseResources()
{
    voice.reset();
    reverb.reset();
    sidechainSampleCounter = 0;
    sidechainSmoothedGain = 1.0f;
    sidechainGainScratch.clear();
    audioLoopWasEnabled = false;
    loopSampleCounter = 0;
    loopNoteCounter = 0;
    lastSynthOutput = 0.0f;
    declickStartOutput = 0.0f;
    declickSamplesRemaining = 0;
    declickTotalSamples = 0;
}

#ifndef JucePlugin_PreferredChannelConfigurations

bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
   #if JucePlugin_IsMidiEffect

    juce::ignoreUnused (layouts);

    return true;

   #else

    if (layouts.getMainOutputChannelSet()
        != juce::AudioChannelSet::mono()

        &&

        layouts.getMainOutputChannelSet()
        != juce::AudioChannelSet::stereo())
    {
        return false;
    }

   #if ! JucePlugin_IsSynth

    if (layouts.getMainOutputChannelSet()
        != layouts.getMainInputChannelSet())
    {
        return false;
    }

   #endif

    return true;

   #endif
}

#endif

void NewProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    if (pitchDropParameter != nullptr) pitchDrop.store (pitchDropParameter->get());
    if (decayParameter     != nullptr) decay.store     (decayParameter->get());
    if (knockParameter     != nullptr) knock.store     (knockParameter->get());
    if (ratioParameter     != nullptr) ratio.store     (ratioParameter->get());
    if (shapeParameter     != nullptr) shape.store     (shapeParameter->get());
    if (toneParameter      != nullptr) tone.store      (toneParameter->get());
    if (bodyParameter      != nullptr) body.store      (bodyParameter->get());
    if (driveParameter     != nullptr) drive.store     (driveParameter->get());

    if (basslineParameter != nullptr)
    {
        basslineEnabled.store (basslineParameter->get());
        voice.setBasslineEnabled (basslineEnabled.load());
    }

    if (reverbEnabledParameter != nullptr) reverbEnabled.store (reverbEnabledParameter->get());
    if (reverbMixParameter     != nullptr) reverbMix.store     (reverbMixParameter->get());

    if (sidechainEnabledParameter  != nullptr) sidechainEnabled.store  (sidechainEnabledParameter->get());
    if (sidechainAmountParameter   != nullptr) sidechainAmount.store   (sidechainAmountParameter->get());
    if (sidechainPositionParameter != nullptr) sidechainPosition.store (sidechainPositionParameter->get());
    if (sidechainAfterReverbParameter != nullptr) sidechainAfterReverb.store (sidechainAfterReverbParameter->get());

    if (volumeParameter        != nullptr) volume.store        (volumeParameter->get());
    if (declickerParameter     != nullptr) declickerEnabled.store (declickerParameter->get());
    if (declickerStrengthParameter != nullptr) declickerStrength.store (declickerStrengthParameter->get());

    std::array<float, AdvancedParameterCount> advancedSnapshot {};

    for (int i = 0; i < AdvancedParameterCount; ++i)
    {
        if (advancedParameters[i] != nullptr)
            advancedValues[i].store (advancedParameters[i]->get());

        advancedSnapshot[i] = advancedValues[i].load();
    }

    const float currentDeclickStrength = juce::jlimit (
            0.0f,
            1.0f,
            declickerStrength.load());

    if (! declickerEnabled.load()
        || currentDeclickStrength <= 0.0001f)
    {
        declickSamplesRemaining = 0;
        declickTotalSamples = 0;
    }

    const auto startDeclick = [this]()
        {
            if (! declickerEnabled.load())
                return;

            const float strength = juce::jlimit (
                    0.0f,
                    1.0f,
                    declickerStrength.load());

            if (strength <= 0.0001f)
            {
                declickSamplesRemaining = 0;
                declickTotalSamples = 0;
                return;
            }

            declickStartOutput = lastSynthOutput;

            const double shapedStrength = std::pow (
                    static_cast<double> (strength),
                    1.35);

            double fadeSeconds = 0.00075
                + shapedStrength * 0.064;

            if (sidechainEnabled.load())
                fadeSeconds *= 1.20;

            if (sidechainEnabled.load()
                && reverbEnabled.load())
            {
                fadeSeconds *= 1.18;
            }

            declickTotalSamples = juce::jmax (
                1,
                static_cast<int> (
                    currentSampleRate
                    * fadeSeconds));

            declickSamplesRemaining = declickTotalSamples;
        };

    const int requestedComputerNote = computerKeyboardRequestedNote.load (
        std::memory_order_acquire);

    if (requestedComputerNote != computerKeyboardPlayingNote)
    {
        if (computerKeyboardPlayingNote >= 0)
            voice.noteOff();

        computerKeyboardPlayingNote = requestedComputerNote;

        if (computerKeyboardPlayingNote >= 0)
        {
            startDeclick();
            sidechainSampleCounter = 0;

            voice.noteOn (
                transposeIncomingMidiNote (computerKeyboardPlayingNote),
                1.0f);

            noteTriggerCount.fetch_add (
                1,
                std::memory_order_relaxed);
        }
    }

    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    if (numSamples > sidechainGainScratch.getNumSamples())
    {
        sidechainGainScratch.setSize (
            1,
            numSamples,
            false,
            false,
            true);
    }

    const bool sidechainPostReverb = sidechainEnabled.load()
        && reverbEnabled.load()
        && sidechainAfterReverb.load();

    const bool requestedLoopState = donkLoopEnabled.load()
        || funDonkLoopEnabled.load();

    if (! requestedLoopState)
    {
        if (audioLoopWasEnabled)
            voice.noteOff();

        audioLoopWasEnabled = false;
        loopSampleCounter = 0;
        loopNoteCounter = 0;
    }
    else if (! audioLoopWasEnabled)
    {
        audioLoopWasEnabled = true;
        loopSampleCounter = 0;
        loopNoteCounter = 0;
        startDeclick();
        sidechainSampleCounter = 0;

        voice.noteOn (
            transposeIncomingMidiNote (60),
            1.0f);

        noteTriggerCount.fetch_add (
            1,
            std::memory_order_relaxed);
    }

    const auto renderOneSample = [this,
         &buffer,
         numChannels,
         &startDeclick,
         &advancedSnapshot,
         sidechainPostReverb] (int sample)
        {
            if (audioLoopWasEnabled)
            {
                ++loopSampleCounter;
                ++loopNoteCounter;

                const auto noteLengthSamples = static_cast<int64_t> (
                        currentSampleRate *
                        donkLoopNoteLengthSeconds);

                if (loopNoteCounter == noteLengthSamples)
                    voice.noteOff();

                const double loopIntervalSeconds = funDonkLoopEnabled.load()
                        ? 0.30
                        : static_cast<double> (
                              juce::jlimit (
                                  0.05f,
                                  1.00f,
                                  donkLoopSpeed.load()));

                if (loopSampleCounter >= static_cast<int64_t> (currentSampleRate * loopIntervalSeconds))
                {
                    loopSampleCounter = 0;
                    loopNoteCounter = 0;
                    startDeclick();
                    sidechainSampleCounter = 0;

                    voice.noteOn (
                        transposeIncomingMidiNote (60),
                        1.0f);

                    noteTriggerCount.fetch_add (
                        1,
                        std::memory_order_relaxed);
                }
            }

            float output = voice.process (
                    pitchDrop.load(),
                    decay.load(),
                    knock.load(),
                    ratio.load(),
                    shape.load(),
                    tone.load(),
                    body.load(),
                    drive.load(),
                    advancedSnapshot,
                    advancedModeEnabled.load());

            if (declickerEnabled.load()
                && declickSamplesRemaining > 0
                && declickTotalSamples > 0)
            {
                const float progress = 1.0f
                    - static_cast<float> (declickSamplesRemaining)
                      / static_cast<float> (declickTotalSamples);

                const float p2 = progress * progress;
                const float p3 = p2 * progress;

                const float smoothProgress = p3
                    * (progress
                       * (progress * 6.0f - 15.0f)
                       + 10.0f);

                output = declickStartOutput * (1.0f - smoothProgress)
                    + output * smoothProgress;

                --declickSamplesRemaining;
            }

            float targetSidechainGain = 1.0f;

            if (sidechainEnabled.load())
            {
                const float amount = juce::jlimit (
                        0.0f,
                        1.0f,
                        sidechainAmount.load());

                if (amount > 0.0001f)
                {
                    const double windowSeconds = juce::jlimit (
                            0.10,
                            1.50,
                            static_cast<double> (
                                juce::jmax (0.05f, decay.load()))
                            * 1.10);

                    const int64_t windowSamples = juce::jmax<int64_t> (
                            1,
                            static_cast<int64_t> (
                                currentSampleRate
                                * windowSeconds));

                    if (sidechainSampleCounter < windowSamples)
                    {
                        const float progress = static_cast<float> (
                                sidechainSampleCounter)
                            /
                            static_cast<float> (windowSamples);

                        const float centre = juce::jlimit (
                                0.0f,
                                1.0f,
                                sidechainPosition.load());

                        const float width = 0.16f;

                        const float distance = (progress - centre)
                            / width;

                        const float duckShape = std::exp (
                                -0.5f
                                * distance
                                * distance);

                        targetSidechainGain = 1.0f
                            - amount
                            * 0.98f
                            * duckShape;
                    }
                }
            }

            float sidechainClickProtection = 0.0f;

            if (declickerEnabled.load())
            {
                const float strength = juce::jlimit (
                        0.0f,
                        1.0f,
                        declickerStrength.load());

                sidechainClickProtection = std::pow (
                        strength,
                        1.25f);
            }

            const float duckDownTime = 0.0025f
                + sidechainClickProtection * 0.0275f;

            const float duckUpTime = 0.0100f
                + sidechainClickProtection * 0.0400f;

            const float smoothingTime = targetSidechainGain < sidechainSmoothedGain
                    ? duckDownTime
                    : duckUpTime;

            const float smoothing = 1.0f
                -
                std::exp (
                    -1.0f
                    /
                    static_cast<float> (
                        currentSampleRate
                        * smoothingTime));

            sidechainSmoothedGain += (targetSidechainGain
                 - sidechainSmoothedGain)
                * smoothing;

            sidechainGainScratch.setSample (
                0,
                sample,
                sidechainSmoothedGain);

            if (! sidechainPostReverb)
                output *= sidechainSmoothedGain;

            ++sidechainSampleCounter;
            lastSynthOutput = output;

            for (int channel = 0;
                 channel < numChannels;
                 ++channel)
            {
                buffer.setSample (
                    channel,
                    sample,
                    output);
            }

            const float level = juce::jlimit (
                    0.0f,
                    1.0f,
                    std::abs (output) * 3.0f);

            const float oldActivity = donkActivity.load();

            donkActivity.store (
                oldActivity * 0.94f
                + level * 0.06f);
        };

    int renderedUntil = 0;

    const auto renderUntil = [&renderOneSample, &renderedUntil, numSamples] (int endSample)
        {
            const int safeEnd = juce::jlimit (
                    renderedUntil,
                    numSamples,
                    endSample);

            while (renderedUntil < safeEnd)
            {
                renderOneSample (renderedUntil);
                ++renderedUntil;
            }
        };

    for (const auto metadata : midiMessages)
    {
        const int eventSample = juce::jlimit (
                0,
                numSamples,
                metadata.samplePosition);

        renderUntil (eventSample);
        const auto message = metadata.getMessage();

        if (message.isNoteOn())
        {
            startDeclick();
            sidechainSampleCounter = 0;

            voice.noteOn (
                transposeIncomingMidiNote (
                    message.getNoteNumber()),
                message.getFloatVelocity());

            noteTriggerCount.fetch_add (
                1,
                std::memory_order_relaxed);
        }
        else if (message.isNoteOff())
        {
            voice.noteOff();
        }
        else if (message.isAllNotesOff()
                 || message.isAllSoundOff())
        {
            voice.noteOff();
        }
    }

    renderUntil (numSamples);
    const bool useReverb = reverbEnabled.load();

    const float mix = juce::jlimit (
            0.0f,
            1.0f,
            reverbMix.load());

    if (useReverb
        && mix > 0.0001f
        && numSamples > 0
        && numChannels > 0)
    {
        juce::Reverb::Parameters parameters;

        parameters.roomSize = 0.45f
            +
            mix * 0.45f;

        parameters.damping = 0.35f;
        parameters.wetLevel = mix * 0.85f;

        parameters.dryLevel = 1.0f
            -
            mix * 0.30f;

        parameters.width = 1.0f;
        parameters.freezeMode = 0.0f;
        reverb.setParameters (parameters);

        if (numChannels >= 2)
        {
            reverb.processStereo (
                buffer.getWritePointer (0),
                buffer.getWritePointer (1),
                numSamples);
        }
        else
        {
            reverb.processMono (
                buffer.getWritePointer (0),
                numSamples);
        }
    }

    if (sidechainPostReverb)
    {
        const auto* sidechainGains = sidechainGainScratch.getReadPointer (0);

        for (int channel = 0;
             channel < numChannels;
             ++channel)
        {
            auto* samples = buffer.getWritePointer (channel);

            for (int sample = 0;
                 sample < numSamples;
                 ++sample)
            {
                samples[sample] *= sidechainGains[sample];
            }
        }
    }

    const float finalVolume = juce::jlimit (
            0.0f,
            1.0f,
            volume.load());

    if (finalVolume != 1.0f)
        buffer.applyGain (finalVolume);

    if (babushkaSafeMode.load())
    {
        constexpr float babushkaCeiling = 0.055f;

        for (int channel = 0; channel < numChannels; ++channel)
        {
            auto* samples = buffer.getWritePointer (channel);

            for (int sample = 0; sample < numSamples; ++sample)
            {
                const float x = samples[sample];
                samples[sample] = std::tanh (x / babushkaCeiling) * babushkaCeiling;
            }
        }
    }

    // mata klagomätaren med lite smoothad output nivå
    float blockLevel = 0.0f;

    if (numChannels > 0 && numSamples > 0)
        blockLevel = buffer.getRMSLevel (0, 0, numSamples);

    const float oldLevel = outputLevel.load();
    outputLevel.store (oldLevel * 0.82f + blockLevel * 0.18f);
}

bool NewProjectAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor*
NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

float NewProjectAudioProcessor::getPitchDrop() const
{
    return pitchDropParameter != nullptr ? pitchDropParameter->get() : pitchDrop.load();
}

float NewProjectAudioProcessor::getDecay() const
{
    return decayParameter != nullptr ? decayParameter->get() : decay.load();
}

float NewProjectAudioProcessor::getKnock() const
{
    return knockParameter != nullptr ? knockParameter->get() : knock.load();
}

float NewProjectAudioProcessor::getRatio() const
{
    return ratioParameter != nullptr ? ratioParameter->get() : ratio.load();
}

float NewProjectAudioProcessor::getShape() const
{
    return shapeParameter != nullptr ? shapeParameter->get() : shape.load();
}

float NewProjectAudioProcessor::getTone() const
{
    return toneParameter != nullptr ? toneParameter->get() : tone.load();
}

float NewProjectAudioProcessor::getBody() const
{
    return bodyParameter != nullptr ? bodyParameter->get() : body.load();
}

float NewProjectAudioProcessor::getDrive() const
{
    return driveParameter != nullptr ? driveParameter->get() : drive.load();
}

bool NewProjectAudioProcessor::getBasslineEnabled() const
{
    return basslineParameter != nullptr ? basslineParameter->get() : basslineEnabled.load();
}

bool NewProjectAudioProcessor::getReverbEnabled() const
{
    return reverbEnabledParameter != nullptr ? reverbEnabledParameter->get() : reverbEnabled.load();
}

float NewProjectAudioProcessor::getReverbMix() const
{
    return reverbMixParameter != nullptr ? reverbMixParameter->get() : reverbMix.load();
}

bool NewProjectAudioProcessor::getSidechainEnabled() const
{
    return sidechainEnabledParameter != nullptr
        ? sidechainEnabledParameter->get()
        : sidechainEnabled.load();
}

float NewProjectAudioProcessor::getSidechainAmount() const
{
    return sidechainAmountParameter != nullptr
        ? sidechainAmountParameter->get()
        : sidechainAmount.load();
}

float NewProjectAudioProcessor::getSidechainPosition() const
{
    return sidechainPositionParameter != nullptr
        ? sidechainPositionParameter->get()
        : sidechainPosition.load();
}

bool NewProjectAudioProcessor::getSidechainAfterReverb() const
{
    return sidechainAfterReverbParameter != nullptr
        ? sidechainAfterReverbParameter->get()
        : sidechainAfterReverb.load();
}

bool NewProjectAudioProcessor::getDeclickerEnabled() const
{
    return declickerParameter != nullptr ? declickerParameter->get() : declickerEnabled.load();
}

float NewProjectAudioProcessor::getDeclickerStrength() const
{
    return declickerStrengthParameter != nullptr
        ? declickerStrengthParameter->get()
        : declickerStrength.load();
}

bool NewProjectAudioProcessor::getAnimationsDisabled() const
{
    return animationsDisabled.load();
}

bool NewProjectAudioProcessor::getMinimalisticEnabled() const
{
    return minimalisticEnabled.load();
}

bool NewProjectAudioProcessor::getAdvancedModeEnabled() const
{
    return advancedModeEnabled.load();
}

int NewProjectAudioProcessor::getNumAdvancedParameters() const
{
    return AdvancedParameterCount;
}

juce::String NewProjectAudioProcessor::getAdvancedParameterName (int index) const
{
    if (! juce::isPositiveAndBelow (index, AdvancedParameterCount))
        return {};

    return advancedParameterDefinitions[index].name;
}

float NewProjectAudioProcessor::getAdvancedParameterMinimum (int index) const
{
    if (! juce::isPositiveAndBelow (index, AdvancedParameterCount))
        return 0.0f;

    return advancedParameterDefinitions[index].minimum;
}

float NewProjectAudioProcessor::getAdvancedParameterMaximum (int index) const
{
    if (! juce::isPositiveAndBelow (index, AdvancedParameterCount))
        return 1.0f;

    return advancedParameterDefinitions[index].maximum;
}

float NewProjectAudioProcessor::getAdvancedParameterInterval (int index) const
{
    if (! juce::isPositiveAndBelow (index, AdvancedParameterCount))
        return 0.01f;

    return advancedParameterDefinitions[index].interval;
}

float NewProjectAudioProcessor::getAdvancedParameterDefault (int index) const
{
    if (! juce::isPositiveAndBelow (index, AdvancedParameterCount))
        return 0.0f;

    return advancedParameterDefinitions[index].defaultValue;
}

float NewProjectAudioProcessor::getAdvancedParameterValue (int index) const
{
    if (! juce::isPositiveAndBelow (index, AdvancedParameterCount))
        return 0.0f;

    if (advancedParameters[index] != nullptr)
        return advancedParameters[index]->get();

    return advancedValues[index].load();
}

bool NewProjectAudioProcessor::getBabushkaSafeMode() const
{
    return babushkaSafeMode.load();
}

bool NewProjectAudioProcessor::getFunDonkLoopEnabled() const
{
    return funDonkLoopEnabled.load();
}

float NewProjectAudioProcessor::getOutputLevel() const
{
    return outputLevel.load();
}

float NewProjectAudioProcessor::getVolume() const
{
    return volumeParameter != nullptr ? volumeParameter->get() : volume.load();
}

bool NewProjectAudioProcessor::getDonkLoopEnabled() const
{
    return donkLoopEnabled.load();
}

float NewProjectAudioProcessor::getDonkLoopSpeed() const
{
    return donkLoopSpeed.load();
}

bool NewProjectAudioProcessor::getExperimentalControlEnabled() const
{
    return experimentalControlEnabled.load();
}

void NewProjectAudioProcessor::setExperimentalControlEnabled (bool enabled)
{
    experimentalControlEnabled.store (enabled);

    if (! enabled)
        experimentalRandomEnabled.store (false);
}

bool NewProjectAudioProcessor::getExperimentalRandomEnabled() const
{
    return experimentalRandomEnabled.load();
}

void NewProjectAudioProcessor::setExperimentalRandomEnabled (bool enabled)
{
    experimentalRandomEnabled.store (
        enabled
        &&
        experimentalControlEnabled.load());
}

void NewProjectAudioProcessor::setPitchDrop (float value)
{
    const float v = juce::jlimit (-48.0f, 72.0f, value);
    pitchDrop.store (v);

    if (pitchDropParameter != nullptr)
        pitchDropParameter->setValueNotifyingHost (pitchDropParameter->convertTo0to1 (v));
}

void NewProjectAudioProcessor::setDecay (float value)
{
    const float v = juce::jlimit (0.01f, 3.0f, value);
    decay.store (v);

    if (decayParameter != nullptr)
        decayParameter->setValueNotifyingHost (decayParameter->convertTo0to1 (v));
}

void NewProjectAudioProcessor::setKnock (float value)
{
    const float v = juce::jlimit (-12.0f, 18.0f, value);
    knock.store (v);

    if (knockParameter != nullptr)
        knockParameter->setValueNotifyingHost (knockParameter->convertTo0to1 (v));
}

void NewProjectAudioProcessor::setRatio (float value)
{
    const float v = juce::jlimit (-8.0f, 12.0f, value);
    ratio.store (v);

    if (ratioParameter != nullptr)
        ratioParameter->setValueNotifyingHost (ratioParameter->convertTo0to1 (v));
}

void NewProjectAudioProcessor::setShape (float value)
{
    const float v = juce::jlimit (-1.0f, 2.0f, value);
    shape.store (v);

    if (shapeParameter != nullptr)
        shapeParameter->setValueNotifyingHost (shapeParameter->convertTo0to1 (v));
}

void NewProjectAudioProcessor::setTone (float value)
{
    const float v = juce::jlimit (-0.03f, 1.50f, value);
    tone.store (v);

    if (toneParameter != nullptr)
        toneParameter->setValueNotifyingHost (toneParameter->convertTo0to1 (v));
}

void NewProjectAudioProcessor::setBody (float value)
{
    const float v = juce::jlimit (-0.25f, 1.50f, value);
    body.store (v);

    if (bodyParameter != nullptr)
        bodyParameter->setValueNotifyingHost (bodyParameter->convertTo0to1 (v));
}

void NewProjectAudioProcessor::setDrive (float value)
{
    const float v = juce::jlimit (-8.0f, 16.0f, value);
    drive.store (v);

    if (driveParameter != nullptr)
        driveParameter->setValueNotifyingHost (driveParameter->convertTo0to1 (v));
}

void NewProjectAudioProcessor::setBasslineEnabled (bool enabled)
{
    basslineEnabled.store (enabled);

    if (basslineParameter != nullptr)
        basslineParameter->setValueNotifyingHost (enabled ? 1.0f : 0.0f);

    voice.setBasslineEnabled (enabled);
}

void NewProjectAudioProcessor::setReverbEnabled (bool enabled)
{
    reverbEnabled.store (enabled);

    if (reverbEnabledParameter != nullptr)
        reverbEnabledParameter->setValueNotifyingHost (enabled ? 1.0f : 0.0f);
}

void NewProjectAudioProcessor::setReverbMix (float value)
{
    const float v = juce::jlimit (0.0f, 1.0f, value);
    reverbMix.store (v);

    if (reverbMixParameter != nullptr)
        reverbMixParameter->setValueNotifyingHost (reverbMixParameter->convertTo0to1 (v));
}

void NewProjectAudioProcessor::setSidechainEnabled (bool enabled)
{
    sidechainEnabled.store (enabled);

    if (sidechainEnabledParameter != nullptr)
        sidechainEnabledParameter->setValueNotifyingHost (enabled ? 1.0f : 0.0f);
}

void NewProjectAudioProcessor::setSidechainAmount (float value)
{
    const float v = juce::jlimit (
            0.0f,
            1.0f,
            value);

    sidechainAmount.store (v);

    if (sidechainAmountParameter != nullptr)
        sidechainAmountParameter->setValueNotifyingHost (sidechainAmountParameter->convertTo0to1 (v));
}

void NewProjectAudioProcessor::setSidechainPosition (float value)
{
    const float v = juce::jlimit (
            0.0f,
            1.0f,
            value);

    sidechainPosition.store (v);

    if (sidechainPositionParameter != nullptr)
        sidechainPositionParameter->setValueNotifyingHost (sidechainPositionParameter->convertTo0to1 (v));
}

void NewProjectAudioProcessor::setSidechainAfterReverb (bool enabled)
{
    sidechainAfterReverb.store (enabled);

    if (sidechainAfterReverbParameter != nullptr)
        sidechainAfterReverbParameter->setValueNotifyingHost (enabled ? 1.0f : 0.0f);
}

void NewProjectAudioProcessor::setDeclickerEnabled (bool enabled)
{
    declickerEnabled.store (enabled);

    if (declickerParameter != nullptr)
        declickerParameter->setValueNotifyingHost (enabled ? 1.0f : 0.0f);

    if (! enabled)
    {
        declickSamplesRemaining = 0;
        declickTotalSamples = 0;
    }
}

void NewProjectAudioProcessor::setDeclickerStrength (float value)
{
    const float v = juce::jlimit (
            0.0f,
            1.0f,
            value);

    declickerStrength.store (v);

    if (declickerStrengthParameter != nullptr)
        declickerStrengthParameter->setValueNotifyingHost (declickerStrengthParameter->convertTo0to1 (v));
}

void NewProjectAudioProcessor::setAnimationsDisabled (bool disabled)
{
    animationsDisabled.store (disabled);
}

void NewProjectAudioProcessor::setMinimalisticEnabled (bool enabled)
{
    minimalisticEnabled.store (enabled);
}

void NewProjectAudioProcessor::setAdvancedModeEnabled (bool enabled)
{
    advancedModeEnabled.store (enabled);
}

void NewProjectAudioProcessor::setAdvancedParameterValue (int index, float value)
{
    if (! juce::isPositiveAndBelow (index, AdvancedParameterCount))
        return;

    const auto& definition = advancedParameterDefinitions[index];

    const float clamped = juce::jlimit (
            definition.minimum,
            definition.maximum,
            value);

    advancedValues[index].store (clamped);

    if (advancedParameters[index] != nullptr)
    {
        advancedParameters[index]->setValueNotifyingHost (advancedParameters[index]->convertTo0to1 (clamped));
    }
}

void NewProjectAudioProcessor::setBabushkaSafeMode (bool enabled)
{
    babushkaSafeMode.store (enabled);
}

void NewProjectAudioProcessor::setFunDonkLoopEnabled (bool enabled)
{
    funDonkLoopEnabled.store (enabled);
}

void NewProjectAudioProcessor::setVolume (float value)
{
    const float v = juce::jlimit (0.0f, 1.0f, value);
    volume.store (v);

    if (volumeParameter != nullptr)
        volumeParameter->setValueNotifyingHost (volumeParameter->convertTo0to1 (v));
}

void NewProjectAudioProcessor::setDonkLoopEnabled (bool enabled)
{
    donkLoopEnabled.store (enabled);
}

void NewProjectAudioProcessor::setDonkLoopSpeed (float seconds)
{
    donkLoopSpeed.store (
        juce::jlimit (
            0.05f,
            1.00f,
            seconds));
}

int NewProjectAudioProcessor::getRootNote() const
{
    if (rootNoteParameter == nullptr)
        return rootNoteNeutral;

    return juce::jlimit (
        rootNoteMinimum,
        rootNoteMaximum,
        rootNoteMinimum + rootNoteParameter->getIndex());
}

void NewProjectAudioProcessor::setRootNote (int midiNote)
{
    if (rootNoteParameter == nullptr)
        return;

    const int clampedNote = juce::jlimit (
            rootNoteMinimum,
            rootNoteMaximum,
            midiNote);

    const int choiceIndex = clampedNote - rootNoteMinimum;

    rootNoteParameter->setValueNotifyingHost (
        rootNoteParameter->convertTo0to1 (static_cast<float> (choiceIndex)));
}

void NewProjectAudioProcessor::setComputerKeyboardNote (int midiNote)
{
    computerKeyboardRequestedNote.store (
        midiNote < 0 ? -1 : juce::jlimit (0, 127, midiNote),
        std::memory_order_release);
}

int NewProjectAudioProcessor::transposeIncomingMidiNote (int midiNote) const
{
    const int semitoneOffset = rootNoteNeutral - getRootNote();

    return juce::jlimit (
        0,
        127,
        midiNote + semitoneOffset);
}

NewProjectAudioProcessor::FaceMode
NewProjectAudioProcessor::getFaceMode() const
{
    // gamla projekt kan ha facemode=0 från gamla smileyn som e borta
    if (faceMode.load() == static_cast<int> (FaceMode::Custom))
    {
        return FaceMode::Custom;
    }

    return FaceMode::StarSlav;
}

void NewProjectAudioProcessor::setFaceMode (FaceMode mode)
{
    faceMode.store (
        static_cast<int> (mode));

    saveGlobalSettings();
}

juce::String NewProjectAudioProcessor::getCustomFacePath() const
{
    const juce::ScopedLock lock (faceLock);

    return customFacePath;
}

void NewProjectAudioProcessor::setCustomFacePath (const juce::String& path)
{
    {
        const juce::ScopedLock lock (faceLock);
        customFacePath = path;
    }

    saveGlobalSettings();
}

juce::File NewProjectAudioProcessor::getGlobalSettingsFile() const
{
    auto directory = juce::File::getSpecialLocation (
            juce::File::userDocumentsDirectory)
            .getChildFile ("StarDonk");

    if (! directory.exists())
        directory.createDirectory();

    return directory.getChildFile ("StarDonkSettings.xml");
}

void NewProjectAudioProcessor::loadGlobalSettings()
{
    const auto settingsFile = getGlobalSettingsFile();

    if (! settingsFile.existsAsFile())
        return;

    std::unique_ptr<juce::XmlElement> xml (
        juce::XmlDocument::parse (settingsFile));

    if (xml == nullptr
        || ! xml->hasTagName ("STAR_DONK_GLOBAL_SETTINGS"))
    {
        return;
    }

    const int savedFaceMode = xml->getIntAttribute (
            "faceMode",
            static_cast<int> (FaceMode::StarSlav));

    faceMode.store (savedFaceMode == static_cast<int> (FaceMode::Custom)
            ? static_cast<int> (FaceMode::Custom)
            : static_cast<int> (FaceMode::StarSlav));

    {
        const juce::ScopedLock lock (faceLock);

        customFacePath = xml->getStringAttribute (
                "customFacePath",
                {});
    }
}

void NewProjectAudioProcessor::saveGlobalSettings() const
{
    juce::String pathToSave;

    {
        const juce::ScopedLock lock (faceLock);
        pathToSave = customFacePath;
    }

    auto xml = std::make_unique<juce::XmlElement> (
            "STAR_DONK_GLOBAL_SETTINGS");

    xml->setAttribute (
        "faceMode",
        faceMode.load());

    xml->setAttribute (
        "customFacePath",
        pathToSave);

    xml->writeTo (getGlobalSettingsFile());
}

void NewProjectAudioProcessor::randomizeSound()
{
    if (! advancedModeEnabled.load())
    {

            const auto randomBetween = [this] (float minimum, float maximum)
                {
                    return minimum
                        + random.nextFloat()
                        * (maximum - minimum);
                };

            const auto chooseRatio = [this] (std::initializer_list<float> values)
                {
                    const int index = random.nextInt (
                            static_cast<int> (values.size()));

                    auto it = values.begin();
                    std::advance (it, index);
                    return *it;
                };

            // lite galet men funkar ändå
            if (experimentalControlEnabled.load()
                && experimentalRandomEnabled.load())
            {
                setPitchDrop (randomBetween (-6.0f, 20.0f));
                setDecay     (randomBetween (0.06f, 1.60f));
                setKnock     (randomBetween (0.3f, 7.5f));
                setRatio     (chooseRatio ({ 0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 4.0f, 5.0f, 6.0f }));
                setShape     (randomBetween (0.0f, 1.0f));
                setTone      (randomBetween (0.05f, 1.0f));
                setBody      (randomBetween (0.0f, 1.0f));
                setDrive     (randomBetween (0.8f, 8.0f));
                currentPreset = -1;
                return;
            }

            float newPitchDrop = randomBetween (5.0f, 13.0f);
            float newDecay     = randomBetween (0.24f, 0.52f);
            float newKnock     = randomBetween (1.8f, 3.7f);
            float newRatio     = chooseRatio ({ 1.5f, 2.0f, 2.0f, 2.5f, 3.0f, 3.0f, 4.0f });
            float newShape     = randomBetween (0.00f, 0.28f);
            float newTone      = randomBetween (0.38f, 0.78f);
            float newBody      = randomBetween (0.18f, 0.48f);
            float newDrive     = randomBetween (1.4f, 3.6f);

            switch (random.nextInt (18))
            {
                case 0: // klassisk hollow
                    newPitchDrop = randomBetween (6.0f, 12.0f);
                    newDecay     = randomBetween (0.26f, 0.48f);
                    newKnock     = randomBetween (2.0f, 3.6f);
                    newRatio     = chooseRatio ({ 2.0f, 2.0f, 3.0f });
                    newShape     = randomBetween (0.00f, 0.16f);
                    newTone      = randomBetween (0.44f, 0.70f);
                    newBody      = randomBetween (0.18f, 0.40f);
                    newDrive     = randomBetween (1.5f, 2.8f);
                    break;

                case 1: // gummi typ
                    newPitchDrop = randomBetween (10.0f, 18.0f);
                    newDecay     = randomBetween (0.36f, 0.72f);
                    newKnock     = randomBetween (1.5f, 2.8f);
                    newRatio     = chooseRatio ({ 1.5f, 2.0f, 2.0f });
                    newShape     = randomBetween (0.04f, 0.22f);
                    newTone      = randomBetween (0.30f, 0.58f);
                    newBody      = randomBetween (0.38f, 0.68f);
                    newDrive     = randomBetween (1.2f, 2.6f);
                    break;

                case 2: // metall rör
                    newPitchDrop = randomBetween (2.0f, 8.0f);
                    newDecay     = randomBetween (0.22f, 0.46f);
                    newKnock     = randomBetween (3.0f, 4.7f);
                    newRatio     = chooseRatio ({ 2.5f, 3.0f, 3.0f, 4.0f });
                    newShape     = randomBetween (0.00f, 0.16f);
                    newTone      = randomBetween (0.68f, 0.94f);
                    newBody      = randomBetween (0.10f, 0.28f);
                    newDrive     = randomBetween (1.5f, 3.2f);
                    break;

                case 3: // träig
                    newPitchDrop = randomBetween (3.0f, 9.0f);
                    newDecay     = randomBetween (0.30f, 0.62f);
                    newKnock     = randomBetween (1.8f, 3.2f);
                    newRatio     = chooseRatio ({ 2.0f, 2.0f, 3.0f });
                    newShape     = randomBetween (0.14f, 0.34f);
                    newTone      = randomBetween (0.28f, 0.54f);
                    newBody      = randomBetween (0.26f, 0.52f);
                    newDrive     = randomBetween (1.2f, 2.5f);
                    break;

                case 4: // hård club
                    newPitchDrop = randomBetween (6.0f, 14.0f);
                    newDecay     = randomBetween (0.18f, 0.38f);
                    newKnock     = randomBetween (2.7f, 4.4f);
                    newRatio     = chooseRatio ({ 2.0f, 3.0f, 3.0f });
                    newShape     = randomBetween (0.16f, 0.42f);
                    newTone      = randomBetween (0.54f, 0.84f);
                    newBody      = randomBetween (0.10f, 0.30f);
                    newDrive     = randomBetween (3.0f, 5.8f);
                    break;

                case 5: // studsig
                    newPitchDrop = randomBetween (11.0f, 20.0f);
                    newDecay     = randomBetween (0.44f, 0.84f);
                    newKnock     = randomBetween (1.5f, 3.0f);
                    newRatio     = chooseRatio ({ 1.5f, 2.0f, 2.0f, 3.0f });
                    newShape     = randomBetween (0.04f, 0.28f);
                    newTone      = randomBetween (0.38f, 0.68f);
                    newBody      = randomBetween (0.42f, 0.74f);
                    newDrive     = randomBetween (1.2f, 2.8f);
                    break;

                case 6: // ljus slap
                    newPitchDrop = randomBetween (4.0f, 10.0f);
                    newDecay     = randomBetween (0.16f, 0.30f);
                    newKnock     = randomBetween (3.0f, 4.6f);
                    newRatio     = chooseRatio ({ 2.0f, 3.0f, 4.0f });
                    newShape     = randomBetween (0.08f, 0.30f);
                    newTone      = randomBetween (0.76f, 0.98f);
                    newBody      = randomBetween (0.08f, 0.22f);
                    newDrive     = randomBetween (1.8f, 4.0f);
                    break;

                case 7: // mörk hollow
                    newPitchDrop = randomBetween (5.0f, 12.0f);
                    newDecay     = randomBetween (0.30f, 0.60f);
                    newKnock     = randomBetween (2.1f, 3.6f);
                    newRatio     = chooseRatio ({ 1.5f, 2.0f, 3.0f });
                    newShape     = randomBetween (0.06f, 0.24f);
                    newTone      = randomBetween (0.18f, 0.40f);
                    newBody      = randomBetween (0.30f, 0.56f);
                    newDrive     = randomBetween (2.0f, 4.2f);
                    break;

                case 8: // oldschool edge typ
                    newPitchDrop = randomBetween (5.0f, 12.0f);
                    newDecay     = randomBetween (0.26f, 0.54f);
                    newKnock     = randomBetween (1.6f, 3.0f);
                    newRatio     = chooseRatio ({ 2.0f, 2.0f, 3.0f });
                    newShape     = randomBetween (0.42f, 0.66f);
                    newTone      = randomBetween (0.42f, 0.70f);
                    newBody      = randomBetween (0.20f, 0.44f);
                    newDrive     = randomBetween (1.6f, 3.2f);
                    break;

                case 9: // glas
                    newPitchDrop = randomBetween (2.0f, 7.0f);
                    newDecay     = randomBetween (0.20f, 0.40f);
                    newKnock     = randomBetween (2.8f, 4.2f);
                    newRatio     = chooseRatio ({ 3.0f, 3.0f, 4.0f });
                    newShape     = randomBetween (0.00f, 0.08f);
                    newTone      = randomBetween (0.66f, 0.90f);
                    newBody      = randomBetween (0.10f, 0.26f);
                    newDrive     = randomBetween (1.2f, 2.4f);
                    break;

                case 10: // djup
                    newPitchDrop = randomBetween (7.0f, 16.0f);
                    newDecay     = randomBetween (0.32f, 0.66f);
                    newKnock     = randomBetween (1.6f, 3.1f);
                    newRatio     = chooseRatio ({ 1.5f, 2.0f, 2.0f });
                    newShape     = randomBetween (0.00f, 0.18f);
                    newTone      = randomBetween (0.24f, 0.50f);
                    newBody      = randomBetween (0.42f, 0.70f);
                    newDrive     = randomBetween (1.3f, 3.0f);
                    break;

                case 11: // tight club donk
                    newPitchDrop = randomBetween (6.0f, 13.0f);
                    newDecay     = randomBetween (0.15f, 0.27f);
                    newKnock     = randomBetween (2.5f, 4.0f);
                    newRatio     = chooseRatio ({ 2.0f, 3.0f, 3.0f });
                    newShape     = randomBetween (0.06f, 0.24f);
                    newTone      = randomBetween (0.54f, 0.82f);
                    newBody      = randomBetween (0.10f, 0.24f);
                    newDrive     = randomBetween (2.0f, 4.2f);
                    break;

                case 12: // lång resonant
                    newPitchDrop = randomBetween (4.0f, 10.0f);
                    newDecay     = randomBetween (0.58f, 0.94f);
                    newKnock     = randomBetween (1.7f, 3.1f);
                    newRatio     = chooseRatio ({ 2.0f, 2.5f, 3.0f });
                    newShape     = randomBetween (0.00f, 0.20f);
                    newTone      = randomBetween (0.38f, 0.68f);
                    newBody      = randomBetween (0.48f, 0.78f);
                    newDrive     = randomBetween (1.2f, 2.6f);
                    break;

                case 13: // gummi metall hybrid
                    newPitchDrop = randomBetween (9.0f, 17.0f);
                    newDecay     = randomBetween (0.34f, 0.62f);
                    newKnock     = randomBetween (2.5f, 4.0f);
                    newRatio     = chooseRatio ({ 2.5f, 3.0f, 3.0f });
                    newShape     = randomBetween (0.04f, 0.20f);
                    newTone      = randomBetween (0.50f, 0.78f);
                    newBody      = randomBetween (0.30f, 0.56f);
                    newDrive     = randomBetween (1.6f, 3.2f);
                    break;

                case 14: // smutsig hollow
                    newPitchDrop = randomBetween (5.0f, 13.0f);
                    newDecay     = randomBetween (0.25f, 0.52f);
                    newKnock     = randomBetween (2.0f, 3.6f);
                    newRatio     = chooseRatio ({ 2.0f, 3.0f });
                    newShape     = randomBetween (0.12f, 0.32f);
                    newTone      = randomBetween (0.34f, 0.64f);
                    newBody      = randomBetween (0.22f, 0.48f);
                    newDrive     = randomBetween (4.0f, 6.6f);
                    break;

                case 15: // ren pipe
                    newPitchDrop = randomBetween (3.0f, 9.0f);
                    newDecay     = randomBetween (0.30f, 0.56f);
                    newKnock     = randomBetween (1.8f, 3.0f);
                    newRatio     = chooseRatio ({ 2.0f, 3.0f });
                    newShape     = randomBetween (0.00f, 0.10f);
                    newTone      = randomBetween (0.46f, 0.72f);
                    newBody      = randomBetween (0.24f, 0.48f);
                    newDrive     = randomBetween (1.0f, 1.7f);
                    break;

                case 16: // mycke energi donk
                    newPitchDrop = randomBetween (10.0f, 18.0f);
                    newDecay     = randomBetween (0.20f, 0.40f);
                    newKnock     = randomBetween (3.0f, 4.4f);
                    newRatio     = chooseRatio ({ 2.0f, 3.0f, 4.0f });
                    newShape     = randomBetween (0.12f, 0.34f);
                    newTone      = randomBetween (0.64f, 0.92f);
                    newBody      = randomBetween (0.14f, 0.32f);
                    newDrive     = randomBetween (2.6f, 5.2f);
                    break;

                default: // varm chunky
                    newPitchDrop = randomBetween (6.0f, 14.0f);
                    newDecay     = randomBetween (0.36f, 0.68f);
                    newKnock     = randomBetween (1.8f, 3.2f);
                    newRatio     = chooseRatio ({ 1.5f, 2.0f, 2.0f, 3.0f });
                    newShape     = randomBetween (0.10f, 0.30f);
                    newTone      = randomBetween (0.30f, 0.58f);
                    newBody      = randomBetween (0.36f, 0.64f);
                    newDrive     = randomBetween (2.0f, 4.0f);
                    break;
            }

            if (newRatio >= 3.5f)
                newKnock = juce::jmin (newKnock, 4.0f);

            // vassare waves har redan massa harmonics så dränk dom inte i fm
            if (newShape > 0.45f)
                newKnock = juce::jmin (newKnock, 3.2f);

            if (newPitchDrop > 15.0f)
            {
                newDecay = juce::jmax (newDecay, 0.32f);
                newBody  = juce::jmax (newBody, 0.28f);
            }

            // mörka patches behöver lite mer fm för o höras rätt
            if (newTone < 0.28f)
                newKnock = juce::jmax (newKnock, 2.2f);

            setPitchDrop (juce::jlimit (0.0f, 24.0f, newPitchDrop));
            setDecay     (juce::jlimit (0.08f, 1.0f, newDecay));
            setKnock     (juce::jlimit (0.0f, 6.0f, newKnock));
            setRatio     (juce::jlimit (0.5f, 4.0f, newRatio));
            setShape     (juce::jlimit (0.0f, 1.0f, newShape));
            setTone      (juce::jlimit (0.0f, 1.0f, newTone));
            setBody      (juce::jlimit (0.0f, 1.0f, newBody));
            setDrive     (juce::jlimit (1.0f, 8.0f, newDrive));
            currentPreset = -1;
        return;
    }

    const auto randomBetween = [this] (float minimum, float maximum)
        {
            return minimum
                + random.nextFloat()
                * (maximum - minimum);
        };

    const auto chooseRatio = [this] (std::initializer_list<float> values)
        {
            const int index = random.nextInt (
                    static_cast<int> (values.size()));

            auto it = values.begin();
            std::advance (it, index);
            return *it;
        };

    const auto decayForRatio = [&randomBetween] (float ratioValue)
        {
            if (ratioValue >= 8.0f)
                return randomBetween (0.030f, 0.095f);

            if (ratioValue >= 5.0f)
                return randomBetween (0.045f, 0.130f);

            if (ratioValue >= 3.0f)
                return randomBetween (0.070f, 0.180f);

            return randomBetween (0.100f, 0.240f);
        };

    const bool wild = experimentalControlEnabled.load()
        && experimentalRandomEnabled.load();

    setPitchDrop (randomBetween (1.0f, 18.0f));
    setDecay     (randomBetween (0.20f, 0.82f));
    setKnock     (randomBetween (1.35f, 3.85f));
    setRatio     (chooseRatio ({ 1.0f, 1.5f, 2.0f, 2.0f, 2.5f, 3.0f, 4.0f, 4.0f }));
    setShape     (randomBetween (0.0f, 0.42f));
    setTone      (randomBetween (0.30f, 0.90f));
    setBody      (randomBetween (0.10f, 0.68f));
    setDrive     (randomBetween (1.05f, 4.80f));
    setAdvancedParameterValue (CarrierLevel, randomBetween (0.88f, 1.12f));
    setAdvancedParameterValue (CarrierTx2, randomBetween (0.0f, 0.32f));
    setAdvancedParameterValue (Fm1Decay, randomBetween (0.085f, 0.28f));
    setAdvancedParameterValue (Fm1Sustain, 0.0f);
    setAdvancedParameterValue (Fm1Shape, randomBetween (0.0f, 0.10f));
    setAdvancedParameterValue (Fm1Tx2, random.nextFloat() < 0.62f ? randomBetween (0.0f, 0.24f)
            : randomBetween (0.24f, 0.78f));

    setAdvancedParameterValue (Fm2Ratio, 2.0f);
    setAdvancedParameterValue (Fm2Amount, 0.0f);
    setAdvancedParameterValue (Fm2Decay, 0.12f);
    setAdvancedParameterValue (Fm2Sustain, 0.0f);
    setAdvancedParameterValue (Fm2Shape, 0.0f);
    setAdvancedParameterValue (Fm2Tx2, 0.0f);
    setAdvancedParameterValue (Fm3Ratio, 8.0f);
    setAdvancedParameterValue (Fm3Amount, 0.0f);
    setAdvancedParameterValue (Fm3Decay, 0.055f);
    setAdvancedParameterValue (Fm3Sustain, 0.0f);
    setAdvancedParameterValue (Fm3Shape, 0.0f);
    setAdvancedParameterValue (Fm3Tx2, 0.0f);
    setAdvancedParameterValue (Fm4Ratio, 4.0f);
    setAdvancedParameterValue (Fm4Amount, 0.0f);
    setAdvancedParameterValue (Fm4Decay, 0.10f);
    setAdvancedParameterValue (Fm4Sustain, 0.0f);
    setAdvancedParameterValue (Fm4Shape, 0.0f);
    setAdvancedParameterValue (Fm4Tx2, 0.0f);
    setAdvancedParameterValue (AmpAttack, randomBetween (0.0005f, 0.0060f));
    setAdvancedParameterValue (AmpDecay, randomBetween (0.22f, 0.72f));
    setAdvancedParameterValue (AmpSustain, randomBetween (0.16f, 0.58f));
    setAdvancedParameterValue (AmpRelease, randomBetween (0.045f, 0.22f));
    setAdvancedParameterValue (PitchDecay, randomBetween (0.025f, 0.11f));
    setAdvancedParameterValue (DistortionMix, randomBetween (0.42f, 1.0f));
    setAdvancedParameterValue (FilterResonance, randomBetween (0.0f, 0.52f));

    // sub finns bara när bassline e på, den ska inte bestämma själva donken
    setAdvancedParameterValue (SubLevel, randomBetween (0.06f, 0.18f));
    setAdvancedParameterValue (SubOctave, -1.0f);
    setAdvancedParameterValue (SubDecay, randomBetween (0.24f, 0.58f));
    setAdvancedParameterValue (SubSustain, randomBetween (0.10f, 0.30f));
    setAdvancedParameterValue (SubShape, randomBetween (0.0f, 0.18f));

    switch (random.nextInt (30))
    {
        case 0: // klassisk hollow
            setRatio (2.0f);
            setPitchDrop (randomBetween (6.0f, 13.0f));
            setDecay (randomBetween (0.28f, 0.54f));
            setKnock (randomBetween (2.0f, 3.4f));
            setShape (randomBetween (0.0f, 0.10f));
            setTone (randomBetween (0.48f, 0.72f));
            setDrive (randomBetween (1.35f, 2.6f));
            setAdvancedParameterValue (CarrierTx2, 0.0f);
            setAdvancedParameterValue (Fm1Tx2, randomBetween (0.0f, 0.14f));
            setAdvancedParameterValue (Fm1Decay, randomBetween (0.13f, 0.23f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.02f, 0.18f));
            break;

        case 1: // studs donk
            setRatio (chooseRatio ({ 1.5f, 2.0f, 2.0f }));
            setPitchDrop (randomBetween (12.0f, 22.0f));
            setDecay (randomBetween (0.52f, 0.92f));
            setKnock (randomBetween (1.45f, 2.65f));
            setBody (randomBetween (0.42f, 0.74f));
            setTone (randomBetween (0.38f, 0.66f));
            setAdvancedParameterValue (Fm1Decay, randomBetween (0.18f, 0.32f));
            setAdvancedParameterValue (AmpDecay, randomBetween (0.48f, 0.82f));
            setAdvancedParameterValue (PitchDecay, randomBetween (0.065f, 0.12f));
            setAdvancedParameterValue (Fm2Ratio, 2.0f);
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.025f, 0.10f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.14f, 0.28f));
            break;

        case 2: // metall shade typ
            setRatio (chooseRatio ({ 3.0f, 4.0f, 5.0f }));
            setPitchDrop (randomBetween (1.0f, 7.0f));
            setDecay (randomBetween (0.26f, 0.52f));
            setKnock (randomBetween (2.0f, 3.5f));
            setShape (randomBetween (0.0f, 0.14f));
            setTone (randomBetween (0.68f, 0.94f));
            setDrive (randomBetween (1.2f, 2.8f));
            setAdvancedParameterValue (CarrierTx2, randomBetween (0.18f, 0.52f));
            setAdvancedParameterValue (Fm1Tx2, randomBetween (0.34f, 0.82f));
            setAdvancedParameterValue (Fm1Decay, randomBetween (0.10f, 0.20f));
            setAdvancedParameterValue (Fm2Ratio, chooseRatio ({ 5.0f, 6.0f, 8.0f }));
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.07f, 0.22f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.055f, 0.14f));
            setAdvancedParameterValue (Fm3Ratio, chooseRatio ({ 8.0f, 10.0f, 12.0f }));
            setAdvancedParameterValue (Fm3Amount, randomBetween (0.025f, 0.09f));
            setAdvancedParameterValue (Fm3Decay, randomBetween (0.035f, 0.09f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.20f, 0.48f));
            break;

        case 3: // gummi donk
            setRatio (chooseRatio ({ 1.0f, 1.5f, 2.0f }));
            setPitchDrop (randomBetween (10.0f, 20.0f));
            setDecay (randomBetween (0.38f, 0.76f));
            setKnock (randomBetween (1.35f, 2.55f));
            setShape (randomBetween (0.04f, 0.24f));
            setTone (randomBetween (0.28f, 0.58f));
            setBody (randomBetween (0.40f, 0.72f));
            setDrive (randomBetween (1.1f, 2.5f));
            setAdvancedParameterValue (Fm1Decay, randomBetween (0.18f, 0.34f));
            setAdvancedParameterValue (Fm2Ratio, chooseRatio ({ 0.5f, 1.0f, 2.0f }));
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.05f, 0.16f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.14f, 0.30f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.16f, 0.42f));
            break;

        case 4: // träig knock
            setRatio (chooseRatio ({ 2.0f, 2.5f, 3.0f }));
            setPitchDrop (randomBetween (3.0f, 10.0f));
            setDecay (randomBetween (0.30f, 0.62f));
            setKnock (randomBetween (1.8f, 3.1f));
            setShape (randomBetween (0.18f, 0.42f));
            setTone (randomBetween (0.26f, 0.54f));
            setDrive (randomBetween (1.0f, 2.2f));
            setAdvancedParameterValue (CarrierTx2, randomBetween (0.0f, 0.14f));
            setAdvancedParameterValue (Fm2Ratio, chooseRatio ({ 2.0f, 3.0f, 4.0f }));
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.04f, 0.14f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.08f, 0.18f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.22f, 0.52f));
            break;

        case 5: // glas donk
            setRatio (chooseRatio ({ 3.0f, 4.0f }));
            setPitchDrop (randomBetween (0.0f, 6.0f));
            setDecay (randomBetween (0.24f, 0.48f));
            setKnock (randomBetween (2.2f, 3.7f));
            setShape (randomBetween (0.0f, 0.06f));
            setTone (randomBetween (0.76f, 0.98f));
            setDrive (randomBetween (1.0f, 1.8f));
            setAdvancedParameterValue (CarrierTx2, randomBetween (0.28f, 0.68f));
            setAdvancedParameterValue (Fm1Tx2, randomBetween (0.50f, 0.92f));
            setAdvancedParameterValue (Fm2Ratio, chooseRatio ({ 6.0f, 8.0f }));
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.04f, 0.13f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.05f, 0.12f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.28f, 0.58f));
            setAdvancedParameterValue (DistortionMix, randomBetween (0.30f, 0.62f));
            break;

        case 6: // smutsig plast
            setRatio (chooseRatio ({ 2.0f, 3.0f, 4.0f }));
            setPitchDrop (randomBetween (5.0f, 14.0f));
            setDecay (randomBetween (0.24f, 0.50f));
            setKnock (randomBetween (2.2f, 3.8f));
            setShape (randomBetween (0.30f, 0.62f));
            setTone (randomBetween (0.46f, 0.76f));
            setDrive (randomBetween (4.0f, 7.2f));
            setAdvancedParameterValue (CarrierTx2, randomBetween (0.10f, 0.38f));
            setAdvancedParameterValue (Fm1Tx2, randomBetween (0.18f, 0.55f));
            setAdvancedParameterValue (DistortionMix, randomBetween (0.82f, 1.0f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.06f, 0.28f));
            break;

        case 7: // rör donk
            setRatio (chooseRatio ({ 2.0f, 3.0f, 4.0f }));
            setPitchDrop (randomBetween (2.0f, 9.0f));
            setDecay (randomBetween (0.34f, 0.66f));
            setKnock (randomBetween (1.7f, 3.0f));
            setShape (randomBetween (0.0f, 0.12f));
            setTone (randomBetween (0.50f, 0.76f));
            setDrive (randomBetween (1.0f, 2.0f));
            setAdvancedParameterValue (Fm1Decay, randomBetween (0.16f, 0.28f));
            setAdvancedParameterValue (Fm2Ratio, chooseRatio ({ 3.0f, 4.0f, 6.0f }));
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.06f, 0.18f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.10f, 0.22f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.30f, 0.62f));
            break;

        case 8: // mörk hollow
            setRatio (chooseRatio ({ 1.0f, 1.5f, 2.0f }));
            setPitchDrop (randomBetween (4.0f, 13.0f));
            setDecay (randomBetween (0.34f, 0.68f));
            setKnock (randomBetween (2.0f, 3.4f));
            setShape (randomBetween (0.0f, 0.18f));
            setTone (randomBetween (0.18f, 0.40f));
            setBody (randomBetween (0.32f, 0.62f));
            setDrive (randomBetween (1.5f, 3.8f));
            setAdvancedParameterValue (Fm1Decay, randomBetween (0.16f, 0.29f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.35f, 0.68f));
            break;

        case 9: // ljus slap
            setRatio (chooseRatio ({ 2.0f, 3.0f, 4.0f }));
            setPitchDrop (randomBetween (3.0f, 10.0f));
            setDecay (randomBetween (0.18f, 0.34f));
            setKnock (randomBetween (2.8f, 4.2f));
            setShape (randomBetween (0.08f, 0.30f));
            setTone (randomBetween (0.76f, 0.98f));
            setBody (randomBetween (0.08f, 0.26f));
            setDrive (randomBetween (2.0f, 4.6f));
            setAdvancedParameterValue (Fm1Decay, randomBetween (0.075f, 0.15f));
            setAdvancedParameterValue (Fm3Ratio, 8.0f);
            setAdvancedParameterValue (Fm3Amount, randomBetween (0.02f, 0.07f));
            setAdvancedParameterValue (Fm3Decay, randomBetween (0.03f, 0.075f));
            break;

        case 10: // lång resonant
            setRatio (chooseRatio ({ 1.5f, 2.0f, 2.5f }));
            setPitchDrop (randomBetween (4.0f, 11.0f));
            setDecay (randomBetween (0.68f, 1.0f));
            setKnock (randomBetween (1.45f, 2.75f));
            setBody (randomBetween (0.48f, 0.78f));
            setTone (randomBetween (0.34f, 0.66f));
            setDrive (randomBetween (1.0f, 2.4f));
            setAdvancedParameterValue (Fm1Decay, randomBetween (0.22f, 0.38f));
            setAdvancedParameterValue (AmpDecay, randomBetween (0.62f, 0.95f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.30f, 0.66f));
            break;

        case 11: // hård club
            setRatio (chooseRatio ({ 2.0f, 3.0f, 4.0f }));
            setPitchDrop (randomBetween (6.0f, 15.0f));
            setDecay (randomBetween (0.20f, 0.42f));
            setKnock (randomBetween (2.8f, 4.4f));
            setShape (randomBetween (0.18f, 0.46f));
            setTone (randomBetween (0.58f, 0.86f));
            setBody (randomBetween (0.10f, 0.34f));
            setDrive (randomBetween (3.2f, 6.8f));
            setAdvancedParameterValue (DistortionMix, randomBetween (0.88f, 1.0f));
            setAdvancedParameterValue (Fm2Ratio, 4.0f);
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.035f, 0.12f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.06f, 0.13f));
            break;

        case 12: // mjuk rund donk
            setRatio (chooseRatio ({ 1.0f, 1.5f, 2.0f }));
            setPitchDrop (randomBetween (4.0f, 10.0f));
            setDecay (randomBetween (0.44f, 0.78f));
            setKnock (randomBetween (1.15f, 2.05f));
            setShape (randomBetween (0.0f, 0.10f));
            setTone (randomBetween (0.34f, 0.56f));
            setBody (randomBetween (0.44f, 0.70f));
            setDrive (randomBetween (1.0f, 1.8f));
            setAdvancedParameterValue (DistortionMix, randomBetween (0.20f, 0.48f));
            setAdvancedParameterValue (Fm1Decay, randomBetween (0.20f, 0.34f));
            break;

        case 13: // typ tx81z buzzig
            setRatio (chooseRatio ({ 2.0f, 3.0f, 4.0f }));
            setPitchDrop (randomBetween (2.0f, 10.0f));
            setDecay (randomBetween (0.28f, 0.56f));
            setKnock (randomBetween (1.8f, 3.1f));
            setShape (randomBetween (0.0f, 0.12f));
            setTone (randomBetween (0.56f, 0.84f));
            setAdvancedParameterValue (CarrierTx2, randomBetween (0.40f, 0.86f));
            setAdvancedParameterValue (Fm1Tx2, randomBetween (0.52f, 1.0f));
            setAdvancedParameterValue (Fm2Tx2, randomBetween (0.30f, 0.78f));
            setAdvancedParameterValue (Fm2Ratio, chooseRatio ({ 2.0f, 4.0f, 6.0f }));
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.05f, 0.17f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.08f, 0.17f));
            break;

        case 14: // typ fresh bacon 1 / 4 / 8 / 4
            setRatio (1.0f);
            setKnock (randomBetween (2.0f, 3.2f));
            setAdvancedParameterValue (Fm2Ratio, 4.0f);
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.10f, 0.28f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.08f, 0.18f));
            setAdvancedParameterValue (Fm3Ratio, 8.0f);
            setAdvancedParameterValue (Fm3Amount, randomBetween (0.025f, 0.085f));
            setAdvancedParameterValue (Fm3Decay, randomBetween (0.035f, 0.09f));
            setAdvancedParameterValue (Fm4Ratio, 4.0f);
            setAdvancedParameterValue (Fm4Amount, randomBetween (0.04f, 0.14f));
            setAdvancedParameterValue (Fm4Decay, randomBetween (0.07f, 0.16f));
            break;

        case 15: // typ frogger 1 / 2 / 8 / 2
            setRatio (1.0f);
            setPitchDrop (randomBetween (8.0f, 17.0f));
            setAdvancedParameterValue (Fm2Ratio, 2.0f);
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.10f, 0.27f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.10f, 0.22f));
            setAdvancedParameterValue (Fm3Ratio, 8.0f);
            setAdvancedParameterValue (Fm3Amount, randomBetween (0.02f, 0.075f));
            setAdvancedParameterValue (Fm3Decay, randomBetween (0.035f, 0.085f));
            setAdvancedParameterValue (Fm4Ratio, 2.0f);
            setAdvancedParameterValue (Fm4Amount, randomBetween (0.04f, 0.16f));
            setAdvancedParameterValue (Fm4Decay, randomBetween (0.10f, 0.22f));
            break;

        case 16: // 1 / 8 / 8 / 4 metall hybrid
            setRatio (1.0f);
            setTone (randomBetween (0.58f, 0.88f));
            setAdvancedParameterValue (Fm2Ratio, 8.0f);
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.04f, 0.13f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.04f, 0.10f));
            setAdvancedParameterValue (Fm3Ratio, 8.0f);
            setAdvancedParameterValue (Fm3Amount, randomBetween (0.02f, 0.08f));
            setAdvancedParameterValue (Fm3Decay, randomBetween (0.035f, 0.09f));
            setAdvancedParameterValue (Fm4Ratio, 4.0f);
            setAdvancedParameterValue (Fm4Amount, randomBetween (0.05f, 0.16f));
            setAdvancedParameterValue (Fm4Decay, randomBetween (0.07f, 0.16f));
            setAdvancedParameterValue (Fm1Tx2, randomBetween (0.20f, 0.62f));
            break;

        case 17: // lite orgel typ men fortfarande donk
            setRatio (1.0f);
            setPitchDrop (randomBetween (2.0f, 8.0f));
            setDecay (randomBetween (0.38f, 0.72f));
            setKnock (randomBetween (1.3f, 2.3f));
            setShape (randomBetween (0.02f, 0.20f));
            setAdvancedParameterValue (Fm2Ratio, 2.0f);
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.10f, 0.22f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.14f, 0.28f));
            setAdvancedParameterValue (Fm4Ratio, 4.0f);
            setAdvancedParameterValue (Fm4Amount, randomBetween (0.025f, 0.10f));
            setAdvancedParameterValue (Fm4Decay, randomBetween (0.11f, 0.24f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.08f, 0.28f));
            break;

        case 18: // nasal honky typ
            setRatio (chooseRatio ({ 2.5f, 3.0f, 3.0f }));
            setPitchDrop (randomBetween (3.0f, 11.0f));
            setDecay (randomBetween (0.30f, 0.58f));
            setShape (randomBetween (0.34f, 0.60f));
            setTone (randomBetween (0.38f, 0.62f));
            setDrive (randomBetween (1.5f, 3.2f));
            setAdvancedParameterValue (Fm2Ratio, 3.0f);
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.07f, 0.20f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.10f, 0.21f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.38f, 0.72f));
            break;

        case 19: // tunn laser metall donk, inte full laser
            setRatio (chooseRatio ({ 4.0f, 5.0f, 6.0f }));
            setPitchDrop (randomBetween (0.0f, 5.0f));
            setDecay (randomBetween (0.20f, 0.42f));
            setKnock (randomBetween (1.5f, 2.6f));
            setShape (randomBetween (0.0f, 0.10f));
            setTone (randomBetween (0.74f, 0.96f));
            setBody (randomBetween (0.06f, 0.22f));
            setAdvancedParameterValue (Fm1Decay, randomBetween (0.07f, 0.14f));
            setAdvancedParameterValue (Fm2Ratio, chooseRatio ({ 7.0f, 8.0f, 10.0f }));
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.04f, 0.12f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.035f, 0.085f));
            setAdvancedParameterValue (CarrierTx2, randomBetween (0.20f, 0.60f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.12f, 0.36f));
            break;

        case 20: // fet hardbass studs
            setRatio (chooseRatio ({ 1.5f, 2.0f, 3.0f }));
            setPitchDrop (randomBetween (14.0f, 24.0f));
            setDecay (randomBetween (0.48f, 0.88f));
            setKnock (randomBetween (2.0f, 3.5f));
            setBody (randomBetween (0.52f, 0.82f));
            setTone (randomBetween (0.42f, 0.70f));
            setDrive (randomBetween (2.4f, 5.4f));
            setAdvancedParameterValue (Fm1Decay, randomBetween (0.17f, 0.30f));
            setAdvancedParameterValue (AmpDecay, randomBetween (0.50f, 0.86f));
            setAdvancedParameterValue (Fm2Ratio, 4.0f);
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.04f, 0.13f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.09f, 0.20f));
            break;

        case 21: // konstig men ändå valid mutant donk
        {
            const float r2 =
                chooseRatio ({ 0.5f, 1.0f, 1.5f, 2.0f, 3.0f, 4.0f, 6.0f });

            const float r3 =
                chooseRatio ({ 2.0f, 3.0f, 4.0f, 6.0f, 8.0f, 10.0f });

            const float r4 =
                chooseRatio ({ 0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 6.0f, 8.0f });

            setRatio (chooseRatio ({ 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 4.0f }));
            setPitchDrop (randomBetween (0.0f, 20.0f));
            setDecay (randomBetween (0.24f, 0.84f));
            setKnock (randomBetween (1.3f, 3.8f));
            setShape (randomBetween (0.0f, 0.55f));
            setTone (randomBetween (0.26f, 0.92f));
            setDrive (randomBetween (1.0f, 5.8f));
            setAdvancedParameterValue (Fm2Ratio, r2);
            setAdvancedParameterValue (Fm3Ratio, r3);
            setAdvancedParameterValue (Fm4Ratio, r4);
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.02f, 0.22f));
            setAdvancedParameterValue (Fm3Amount, randomBetween (0.0f, 0.10f));
            setAdvancedParameterValue (Fm4Amount, randomBetween (0.0f, 0.16f));
            setAdvancedParameterValue (Fm2Decay, decayForRatio (r2));
            setAdvancedParameterValue (Fm3Decay, decayForRatio (r3));
            setAdvancedParameterValue (Fm4Decay, decayForRatio (r4));
            setAdvancedParameterValue (CarrierTx2, randomBetween (0.0f, 0.62f));
            setAdvancedParameterValue (Fm1Tx2, randomBetween (0.0f, 0.78f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.0f, 0.60f));
            break;
        }
        case 22: // fjäder studs typ
            setRatio (chooseRatio ({ 1.0f, 1.5f, 2.0f }));
            setPitchDrop (randomBetween (15.0f, 24.0f));
            setDecay (randomBetween (0.50f, 0.95f));
            setKnock (randomBetween (1.3f, 2.6f));
            setBody (randomBetween (0.45f, 0.82f));
            setTone (randomBetween (0.36f, 0.68f));
            setAdvancedParameterValue (Fm1Decay, randomBetween (0.20f, 0.36f));
            setAdvancedParameterValue (Fm2Ratio, 0.5f);
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.18f, 0.42f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.18f, 0.34f));
            setAdvancedParameterValue (PitchDecay, randomBetween (0.08f, 0.15f));
            break;

        case 23: // metall shade typ
            setRatio (chooseRatio ({ 3.0f, 4.0f, 5.0f }));
            setPitchDrop (randomBetween (0.0f, 7.0f));
            setDecay (randomBetween (0.28f, 0.62f));
            setKnock (randomBetween (1.7f, 3.2f));
            setTone (randomBetween (0.68f, 0.96f));
            setDrive (randomBetween (1.0f, 2.4f));
            setAdvancedParameterValue (CarrierTx2, randomBetween (0.40f, 0.90f));
            setAdvancedParameterValue (Fm1Tx2, randomBetween (0.55f, 1.0f));
            setAdvancedParameterValue (Fm2Ratio, chooseRatio ({ 5.0f, 6.0f, 8.0f }));
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.20f, 0.48f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.07f, 0.17f));
            setAdvancedParameterValue (Fm3Ratio, chooseRatio ({ 8.0f, 10.0f, 12.0f }));
            setAdvancedParameterValue (Fm3Amount, randomBetween (0.08f, 0.20f));
            setAdvancedParameterValue (Fm3Decay, randomBetween (0.035f, 0.10f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.28f, 0.62f));
            setAdvancedParameterValue (DistortionMix, randomBetween (0.18f, 0.55f));
            break;

        case 24: // ihålig hink
            setRatio (chooseRatio ({ 1.0f, 2.0f, 2.5f }));
            setPitchDrop (randomBetween (5.0f, 14.0f));
            setDecay (randomBetween (0.38f, 0.76f));
            setKnock (randomBetween (2.2f, 4.0f));
            setTone (randomBetween (0.20f, 0.48f));
            setAdvancedParameterValue (Fm2Ratio, 3.0f);
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.16f, 0.38f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.11f, 0.25f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.46f, 0.76f));
            break;

        case 25: // arcade plast
            setRatio (chooseRatio ({ 2.0f, 3.0f, 4.0f }));
            setPitchDrop (randomBetween (7.0f, 18.0f));
            setDecay (randomBetween (0.20f, 0.48f));
            setKnock (randomBetween (2.0f, 3.8f));
            setShape (randomBetween (0.50f, 0.88f));
            setTone (randomBetween (0.58f, 0.94f));
            setDrive (randomBetween (4.5f, 8.0f));
            setAdvancedParameterValue (Fm2Ratio, chooseRatio ({ 3.0f, 4.0f, 6.0f }));
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.16f, 0.38f));
            setAdvancedParameterValue (Fm2Shape, randomBetween (0.45f, 0.86f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.055f, 0.14f));
            setAdvancedParameterValue (DistortionMix, randomBetween (0.90f, 1.0f));
            break;

        case 26: // resonant rör
            setRatio (chooseRatio ({ 1.5f, 2.0f, 3.0f }));
            setPitchDrop (randomBetween (2.0f, 11.0f));
            setDecay (randomBetween (0.42f, 0.82f));
            setKnock (randomBetween (1.5f, 3.0f));
            setShape (randomBetween (0.20f, 0.50f));
            setTone (randomBetween (0.32f, 0.64f));
            setAdvancedParameterValue (Fm1Tx2, randomBetween (0.18f, 0.62f));
            setAdvancedParameterValue (Fm4Ratio, chooseRatio ({ 0.5f, 1.0f, 2.0f }));
            setAdvancedParameterValue (Fm4Amount, randomBetween (0.16f, 0.36f));
            setAdvancedParameterValue (Fm4Decay, randomBetween (0.14f, 0.30f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.52f, 0.80f));
            break;

        case 27: // ljus metall studs
            setRatio (chooseRatio ({ 1.5f, 2.0f, 4.0f }));
            setPitchDrop (randomBetween (11.0f, 23.0f));
            setDecay (randomBetween (0.44f, 0.86f));
            setKnock (randomBetween (1.7f, 3.0f));
            setTone (randomBetween (0.68f, 0.98f));
            setAdvancedParameterValue (CarrierTx2, randomBetween (0.22f, 0.60f));
            setAdvancedParameterValue (Fm2Ratio, 4.0f);
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.14f, 0.34f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.10f, 0.24f));
            setAdvancedParameterValue (Fm3Ratio, 8.0f);
            setAdvancedParameterValue (Fm3Amount, randomBetween (0.05f, 0.14f));
            setAdvancedParameterValue (Fm3Decay, randomBetween (0.04f, 0.10f));
            break;

        case 28: // gummi metall hybrid
            setRatio (chooseRatio ({ 1.0f, 1.5f, 2.0f }));
            setPitchDrop (randomBetween (8.0f, 20.0f));
            setDecay (randomBetween (0.44f, 0.90f));
            setKnock (randomBetween (1.5f, 2.9f));
            setShape (randomBetween (0.08f, 0.30f));
            setTone (randomBetween (0.34f, 0.70f));
            setBody (randomBetween (0.46f, 0.80f));
            setAdvancedParameterValue (Fm2Ratio, 0.5f);
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.16f, 0.36f));
            setAdvancedParameterValue (Fm2Decay, randomBetween (0.18f, 0.34f));
            setAdvancedParameterValue (Fm3Ratio, 6.0f);
            setAdvancedParameterValue (Fm3Amount, randomBetween (0.04f, 0.12f));
            setAdvancedParameterValue (Fm3Decay, randomBetween (0.05f, 0.12f));
            setAdvancedParameterValue (Fm1Tx2, randomBetween (0.12f, 0.50f));
            break;

        default: // kaos men fortfarande valid donk
        {
            const float r2 = chooseRatio ({ 0.5f, 1.0f, 1.5f, 2.0f, 3.0f, 4.0f, 6.0f, 8.0f });
            const float r3 = chooseRatio ({ 2.0f, 3.0f, 4.0f, 6.0f, 8.0f, 10.0f, 12.0f });
            const float r4 = chooseRatio ({ 0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 6.0f, 8.0f });
            setRatio (chooseRatio ({ 0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 4.0f, 5.0f }));
            setPitchDrop (randomBetween (0.0f, 24.0f));
            setDecay (randomBetween (0.18f, 0.95f));
            setKnock (randomBetween (1.1f, 4.2f));
            setShape (randomBetween (0.0f, 0.82f));
            setTone (randomBetween (0.20f, 0.98f));
            setBody (randomBetween (0.05f, 0.84f));
            setDrive (randomBetween (1.0f, 7.5f));
            setAdvancedParameterValue (Fm2Ratio, r2);
            setAdvancedParameterValue (Fm3Ratio, r3);
            setAdvancedParameterValue (Fm4Ratio, r4);
            setAdvancedParameterValue (Fm2Amount, randomBetween (0.06f, 0.44f));
            setAdvancedParameterValue (Fm3Amount, randomBetween (0.0f, 0.20f));
            setAdvancedParameterValue (Fm4Amount, randomBetween (0.0f, 0.34f));
            setAdvancedParameterValue (Fm2Decay, decayForRatio (r2));
            setAdvancedParameterValue (Fm3Decay, decayForRatio (r3));
            setAdvancedParameterValue (Fm4Decay, decayForRatio (r4));
            setAdvancedParameterValue (CarrierTx2, randomBetween (0.0f, 0.88f));
            setAdvancedParameterValue (Fm1Tx2, randomBetween (0.0f, 1.0f));
            setAdvancedParameterValue (FilterResonance, randomBetween (0.0f, 0.78f));
            break;
        }
    }

    if (getRatio() >= 5.0f)
        setKnock (juce::jmin (getKnock(), 2.8f));

    if (getAdvancedParameterValue (Fm3Ratio) >= 8.0f)
        setAdvancedParameterValue (
            Fm3Amount,
            juce::jmin (
                getAdvancedParameterValue (Fm3Amount),
                0.10f));

    if (getAdvancedParameterValue (Fm2Ratio) >= 8.0f)
        setAdvancedParameterValue (
            Fm2Amount,
            juce::jmin (
                getAdvancedParameterValue (Fm2Amount),
                0.16f));

    if (getTone() < 0.24f)
        setKnock (juce::jmax (getKnock(), 1.8f));

    if (wild)
    {
        // experiment random går bredare men börjar ändå från nån donk
        setPitchDrop (randomBetween (-1.0f, 16.0f));
        setDecay (randomBetween (0.16f, 0.95f));
        setKnock (randomBetween (1.0f, 4.2f));
        setRatio (chooseRatio ({ 0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 4.0f, 5.0f, 6.0f, 8.0f }));
        setShape (randomBetween (0.0f, 0.62f));
        setTone (randomBetween (0.28f, 0.92f));
        setBody (randomBetween (0.05f, 0.78f));
        setDrive (randomBetween (1.0f, 6.5f));
        const float r2 = chooseRatio ({ 0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 6.0f, 8.0f });
        const float r3 = chooseRatio ({ 2.0f, 4.0f, 6.0f, 8.0f, 10.0f, 12.0f });
        const float r4 = chooseRatio ({ 0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 6.0f, 8.0f });
        setAdvancedParameterValue (Fm2Ratio, r2);
        setAdvancedParameterValue (Fm3Ratio, r3);
        setAdvancedParameterValue (Fm4Ratio, r4);
        setAdvancedParameterValue (CarrierLevel, randomBetween (0.72f, 1.24f));
        setAdvancedParameterValue (CarrierTx2, randomBetween (0.0f, 1.0f));
        setAdvancedParameterValue (Fm1Decay, randomBetween (0.045f, 0.42f));
        setAdvancedParameterValue (Fm1Shape, randomBetween (0.0f, 0.88f));
        setAdvancedParameterValue (Fm1Tx2, randomBetween (0.0f, 1.0f));
        setAdvancedParameterValue (Fm2Amount, randomBetween (0.0f, 0.66f));
        setAdvancedParameterValue (Fm3Amount, randomBetween (0.0f, 0.32f));
        setAdvancedParameterValue (Fm4Amount, randomBetween (0.0f, 0.52f));
        setAdvancedParameterValue (Fm2Decay, decayForRatio (r2));
        setAdvancedParameterValue (Fm3Decay, decayForRatio (r3));
        setAdvancedParameterValue (Fm4Decay, decayForRatio (r4));
        setAdvancedParameterValue (Fm2Shape, randomBetween (0.0f, 0.88f));
        setAdvancedParameterValue (Fm3Shape, randomBetween (0.0f, 0.72f));
        setAdvancedParameterValue (Fm4Shape, randomBetween (0.0f, 0.84f));
        setAdvancedParameterValue (Fm2Tx2, randomBetween (0.0f, 0.86f));
        setAdvancedParameterValue (Fm3Tx2, randomBetween (0.0f, 0.66f));
        setAdvancedParameterValue (Fm4Tx2, randomBetween (0.0f, 0.80f));
        setAdvancedParameterValue (FilterResonance, randomBetween (0.0f, 0.82f));
        setAdvancedParameterValue (DistortionMix, randomBetween (0.22f, 1.0f));
        setAdvancedParameterValue (AmpAttack, randomBetween (0.0005f, 0.014f));
        setAdvancedParameterValue (AmpDecay, randomBetween (0.16f, 0.95f));
        setAdvancedParameterValue (AmpRelease, randomBetween (0.025f, 0.30f));
        setAdvancedParameterValue (PitchDecay, randomBetween (0.015f, 0.16f));
    }

    // nolla gamla c a b e systemet som vi inte använder mer
    setAdvancedParameterValue (ClassicBlend, 0.0f);
    setAdvancedParameterValue (ClassicCToA, 0.0f);
    setAdvancedParameterValue (ClassicCToB, 0.0f);
    setAdvancedParameterValue (ClassicCToE, 0.0f);
    setAdvancedParameterValue (ClassicBLevel, 0.0f);
    setAdvancedParameterValue (ClassicELevel, 0.0f);
    currentPreset = -1;
}
juce::File NewProjectAudioProcessor::getUserPresetDirectory() const
{
    auto directory = juce::File::getSpecialLocation (
            juce::File::userDocumentsDirectory)
            .getChildFile ("StarDonk")
            .getChildFile ("Presets");

    if (! directory.exists())
        directory.createDirectory();

    return directory;
}

void NewProjectAudioProcessor::refreshUserPresets()
{
    const auto directory = getUserPresetDirectory();

    const auto files = directory.findChildFiles (
            juce::File::findFiles,
            true,
            "*.donk");

    juce::Array<Preset> foundPresets;

    for (const auto& file : files)
    {
        Preset preset;

        if (readDonkFile (
                file,
                preset))
        {
            preset.name = file.getFileNameWithoutExtension();

            preset.relativePath = file.getRelativePathFrom (directory)
                    .replaceCharacter ('\\', '/');

            foundPresets.add (preset);
        }
    }

    // sortera alfabet typ

    for (int i = 0;
         i < foundPresets.size();
         ++i)
    {
        for (int j = i + 1;
             j < foundPresets.size();
             ++j)
        {
            if (foundPresets[j].relativePath.compareIgnoreCase (
                    foundPresets[i].relativePath) < 0)
            {
                foundPresets.swap (
                    i,
                    j);
            }
        }
    }

    // byt ut nuvarande listan

    {
        const juce::ScopedLock lock (presetLock);
        presets = foundPresets;

        if (presets.isEmpty())
        {
            currentPreset = -1;
        }
        else if (currentPreset >= presets.size())
        {
            currentPreset = -1;
        }

    }
}

bool NewProjectAudioProcessor::readDonkFile (const juce::File& file, Preset& preset) const
{
    if (! file.existsAsFile())
        return false;

    std::unique_ptr<juce::XmlElement> xml (
        juce::XmlDocument::parse (file));

    if (xml == nullptr)
        return false;

    if (! xml->hasTagName (
            "STAR_DONK"))
    {
        return false;
    }

    preset.name = xml->getStringAttribute (
            "name",
            file.getFileNameWithoutExtension());

    preset.pitchDrop = static_cast<float> (
            xml->getDoubleAttribute (
                "pitchDrop",
                7.0));

    preset.decay = static_cast<float> (
            xml->getDoubleAttribute (
                "decay",
                0.28));

    preset.knock = static_cast<float> (
            xml->getDoubleAttribute (
                "knock",
                2.0));

    preset.ratio = static_cast<float> (
            xml->getDoubleAttribute (
                "ratio",
                2.0));

    preset.shape = static_cast<float> (
            xml->getDoubleAttribute (
                "shape",
                0.2));

    preset.tone = static_cast<float> (
            xml->getDoubleAttribute (
                "tone",
                0.4));

    preset.body = static_cast<float> (
            xml->getDoubleAttribute (
                "body",
                0.5));

    preset.drive = static_cast<float> (
            xml->getDoubleAttribute (
                "drive",
                2.0));

    preset.bassline = xml->getBoolAttribute (
            "bassline",
            true);

    preset.reverbEnabled = xml->getBoolAttribute (
            "reverbEnabled",
            false);

    preset.reverbMix = static_cast<float> (
            xml->getDoubleAttribute (
                "reverbMix",
                0.0));

    preset.reverbMix = juce::jlimit (
            0.0f,
            1.0f,
            preset.reverbMix);

    preset.sidechainEnabled = xml->getBoolAttribute (
            "sidechainEnabled",
            false);

    preset.sidechainAmount = juce::jlimit (
            0.0f,
            1.0f,
            static_cast<float> (
                xml->getDoubleAttribute (
                    "sidechainAmount",
                    0.65)));

    preset.sidechainPosition = juce::jlimit (
            0.0f,
            1.0f,
            static_cast<float> (
                xml->getDoubleAttribute (
                    "sidechainPosition",
                    0.0)));

    preset.sidechainAfterReverb = xml->getBoolAttribute (
            "sidechainAfterReverb",
            false);

    preset.declickerEnabled = xml->getBoolAttribute (
            "declickerEnabled",
            false);

    preset.declickerStrength = juce::jlimit (
            0.0f,
            1.0f,
            static_cast<float> (
                xml->getDoubleAttribute (
                    "declickerStrength",
                    0.35)));

    preset.volume = static_cast<float> (
            xml->getDoubleAttribute (
                "volume",
                1.0));

    preset.volume = juce::jlimit (
            0.0f,
            1.0f,
            preset.volume);

    preset.experimentalControl = xml->getBoolAttribute (
            "experimentalControl",
            false);

    preset.experimentalRandom = xml->getBoolAttribute (
            "experimentalRandom",
            false);

    if (! preset.experimentalControl)
        preset.experimentalRandom = false;

    for (int i = 0; i < AdvancedParameterCount; ++i)
    {
        preset.advancedValues[i] = static_cast<float> (
                xml->getDoubleAttribute (
                    "advanced" + juce::String (i),
                    getAdvancedParameterDefault (i)));
    }

    return true;
}

bool NewProjectAudioProcessor::writeDonkFile (const juce::File& file, const Preset& preset)
{
    auto xml = std::make_unique<juce::XmlElement> (
            "STAR_DONK");

    xml->setAttribute (
        "name",
        preset.name);

    xml->setAttribute (
        "pitchDrop",
        preset.pitchDrop);

    xml->setAttribute (
        "decay",
        preset.decay);

    xml->setAttribute (
        "knock",
        preset.knock);

    xml->setAttribute (
        "ratio",
        preset.ratio);

    xml->setAttribute (
        "shape",
        preset.shape);

    xml->setAttribute (
        "tone",
        preset.tone);

    xml->setAttribute (
        "body",
        preset.body);

    xml->setAttribute (
        "drive",
        preset.drive);

    xml->setAttribute (
        "bassline",
        preset.bassline);

    xml->setAttribute (
        "reverbEnabled",
        preset.reverbEnabled);

    xml->setAttribute (
        "reverbMix",
        preset.reverbMix);

    xml->setAttribute (
        "sidechainEnabled",
        preset.sidechainEnabled);

    xml->setAttribute (
        "sidechainAmount",
        preset.sidechainAmount);

    xml->setAttribute (
        "sidechainPosition",
        preset.sidechainPosition);

    xml->setAttribute (
        "sidechainAfterReverb",
        preset.sidechainAfterReverb);

    xml->setAttribute (
        "declickerEnabled",
        preset.declickerEnabled);

    xml->setAttribute (
        "declickerStrength",
        preset.declickerStrength);

    xml->setAttribute (
        "volume",
        preset.volume);

    xml->setAttribute (
        "experimentalControl",
        preset.experimentalControl);

    xml->setAttribute (
        "experimentalRandom",
        preset.experimentalRandom);

    for (int i = 0; i < AdvancedParameterCount; ++i)
    {
        xml->setAttribute (
            "advanced" + juce::String (i),
            preset.advancedValues[i]);
    }

    return xml->writeTo (file);
}

int NewProjectAudioProcessor::getNumPresets() const
{
    const juce::ScopedLock lock (presetLock);

    return presets.size();
}

juce::String NewProjectAudioProcessor::getPresetName (int index) const
{
    const juce::ScopedLock lock (presetLock);

    if (juce::isPositiveAndBelow (
            index,
            presets.size()))
    {
        return presets[index].name;
    }

    return {};
}

juce::String NewProjectAudioProcessor::getPresetRelativePath (int index) const
{
    const juce::ScopedLock lock (presetLock);

    if (juce::isPositiveAndBelow (index, presets.size()))
        return presets[index].relativePath;

    return {};
}

int NewProjectAudioProcessor::getCurrentPresetIndex() const
{
    return currentPreset;
}

void NewProjectAudioProcessor::loadPreset (int index)
{
    Preset preset;

    {
        const juce::ScopedLock lock (presetLock);

        if (! juce::isPositiveAndBelow (
                index,
                presets.size()))
        {
            return;
        }

        preset = presets[index];
    }

    setExperimentalControlEnabled (preset.experimentalControl);
    setExperimentalRandomEnabled (preset.experimentalRandom);
    setPitchDrop (preset.pitchDrop);
    setDecay (preset.decay);
    setKnock (preset.knock);
    setRatio (preset.ratio);
    setShape (preset.shape);
    setTone (preset.tone);
    setBody (preset.body);
    setDrive (preset.drive);
    setBasslineEnabled (preset.bassline);
    setReverbEnabled (preset.reverbEnabled);
    setReverbMix (preset.reverbMix);
    setSidechainEnabled (preset.sidechainEnabled);
    setSidechainAmount (preset.sidechainAmount);
    setSidechainPosition (preset.sidechainPosition);
    setSidechainAfterReverb (preset.sidechainAfterReverb);
    setDeclickerEnabled (preset.declickerEnabled);
    setDeclickerStrength (preset.declickerStrength);
    setVolume (preset.volume);

    for (int i = 0; i < AdvancedParameterCount; ++i)
        setAdvancedParameterValue (i, preset.advancedValues[i]);

    currentPreset = index;
}

bool NewProjectAudioProcessor::presetNameExists (
    const juce::String& name,
    const juce::String& relativeFolder) const
{
    auto fileName = name.trim();

    if (fileName.isEmpty())
        return false;

    const juce::String invalidCharacters = "\\/:*?\"<>|";

    for (auto character : invalidCharacters)
        fileName = fileName.replaceCharacter (character, '_');

    fileName = fileName.trim();

    if (fileName.isEmpty())
        fileName = "MyDonk";

    auto targetDirectory = getUserPresetDirectory();

    if (relativeFolder.trim().isNotEmpty())
    {
        const auto safeFolder = relativeFolder
            .replaceCharacter ('\\', '/')
            .trimCharactersAtStart ("/")
            .trimCharactersAtEnd ("/");

        targetDirectory = targetDirectory.getChildFile (safeFolder);
    }

    return targetDirectory
        .getChildFile (fileName)
        .withFileExtension (".donk")
        .existsAsFile();
}

void NewProjectAudioProcessor::saveCurrentAsPreset (
    const juce::String& name,
    const juce::String& relativeFolder)
{
    const auto cleanName = name.trim();

    if (cleanName.isEmpty())
        return;

    // skriv aldrig över en .donk preset i smyg
    if (presetNameExists (cleanName, relativeFolder))
        return;

    Preset newPreset;
    newPreset.name = cleanName;
    newPreset.pitchDrop = getPitchDrop();
    newPreset.decay = getDecay();
    newPreset.knock = getKnock();
    newPreset.ratio = getRatio();
    newPreset.shape = getShape();
    newPreset.tone = getTone();
    newPreset.body = getBody();
    newPreset.drive = getDrive();
    newPreset.bassline = getBasslineEnabled();
    newPreset.reverbEnabled = getReverbEnabled();
    newPreset.reverbMix = getReverbMix();
    newPreset.sidechainEnabled = getSidechainEnabled();
    newPreset.sidechainAmount = getSidechainAmount();
    newPreset.sidechainPosition = getSidechainPosition();
    newPreset.sidechainAfterReverb = getSidechainAfterReverb();
    newPreset.declickerEnabled = getDeclickerEnabled();
    newPreset.declickerStrength = getDeclickerStrength();
    newPreset.volume = getVolume();
    newPreset.experimentalControl = getExperimentalControlEnabled();
    newPreset.experimentalRandom = getExperimentalRandomEnabled();

    for (int i = 0; i < AdvancedParameterCount; ++i)
        newPreset.advancedValues[i] = getAdvancedParameterValue (i);

    // windows safe filnamn typ

    juce::String fileName = cleanName;
    const juce::String invalidCharacters = "\\/:*?\"<>|";

    for (auto character : invalidCharacters)
    {
        fileName = fileName.replaceCharacter (
                character,
                '_');
    }

    fileName = fileName.trim();

    if (fileName.isEmpty())
        fileName = "MyDonk";

    auto targetDirectory = getUserPresetDirectory();

    if (relativeFolder.trim().isNotEmpty())
    {
        auto safeFolder = relativeFolder
            .replaceCharacter ('\\', '/')
            .trimCharactersAtStart ("/")
            .trimCharactersAtEnd ("/");

        targetDirectory = targetDirectory.getChildFile (safeFolder);
        targetDirectory.createDirectory();
    }

    const auto file = targetDirectory
            .getChildFile (fileName)
            .withFileExtension (".donk");

    // skriv filen

    if (! writeDonkFile (
            file,
            newPreset))
    {
        return;
    }

    // läs om

    refreshUserPresets();

    // välj preseten vi nyss spara
    const auto wantedPath = getUserPresetDirectory().getRelativePathFrom (file)
            .replaceCharacter ('\\', '/');

    for (int i = 0; i < presets.size(); ++i)
    {
        if (presets[i].relativePath.equalsIgnoreCase (wantedPath))
        {
            currentPreset = i;
            break;
        }
    }
}

float NewProjectAudioProcessor::getDonkActivity() const
{
    return donkActivity.load();
}

uint64_t NewProjectAudioProcessor::getNoteTriggerCount() const
{
    return noteTriggerCount.load (std::memory_order_relaxed);
}

void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto xml = std::make_unique<juce::XmlElement> (
            "STAR_DONK_STATE");

    xml->setAttribute (
        "pitchDrop",
        getPitchDrop());

    xml->setAttribute (
        "decay",
        getDecay());

    xml->setAttribute (
        "knock",
        getKnock());

    xml->setAttribute (
        "ratio",
        getRatio());

    xml->setAttribute (
        "shape",
        getShape());

    xml->setAttribute (
        "tone",
        getTone());

    xml->setAttribute (
        "body",
        getBody());

    xml->setAttribute (
        "drive",
        getDrive());

    xml->setAttribute (
        "bassline",
        getBasslineEnabled());

    xml->setAttribute (
        "reverbEnabled",
        getReverbEnabled());

    xml->setAttribute (
        "reverbMix",
        getReverbMix());

    xml->setAttribute (
        "sidechainEnabled",
        getSidechainEnabled());

    xml->setAttribute (
        "sidechainAmount",
        getSidechainAmount());

    xml->setAttribute (
        "sidechainPosition",
        getSidechainPosition());

    xml->setAttribute (
        "sidechainAfterReverb",
        getSidechainAfterReverb());

    xml->setAttribute (
        "declickerEnabled",
        getDeclickerEnabled());

    xml->setAttribute (
        "declickerStrength",
        getDeclickerStrength());

    xml->setAttribute (
        "animationsDisabled",
        getAnimationsDisabled());

    xml->setAttribute (
        "minimalisticEnabled",
        getMinimalisticEnabled());

    xml->setAttribute (
        "advancedModeEnabled",
        getAdvancedModeEnabled());

    for (int i = 0; i < AdvancedParameterCount; ++i)
    {
        xml->setAttribute (
            "advanced" + juce::String (i),
            getAdvancedParameterValue (i));
    }

    xml->setAttribute (
        "volume",
        getVolume());

    xml->setAttribute (
        "rootNote",
        getRootNote());

    xml->setAttribute (
        "experimentalControl",
        getExperimentalControlEnabled());

    xml->setAttribute (
        "experimentalRandom",
        getExperimentalRandomEnabled());

    xml->setAttribute (
        "faceMode",
        static_cast<int> (getFaceMode()));

    xml->setAttribute (
        "customFacePath",
        getCustomFacePath());

    xml->setAttribute (
        "preset",
        getCurrentPresetIndex());

    copyXmlToBinary (
        *xml,
        destData);
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml (
        getXmlFromBinary (
            data,
            sizeInBytes));

    if (xml == nullptr
        || ! xml->hasTagName ("STAR_DONK_STATE"))
    {
        return;
    }

    setPitchDrop (
        static_cast<float> (
            xml->getDoubleAttribute (
                "pitchDrop",
                7.0)));

    setDecay (
        static_cast<float> (
            xml->getDoubleAttribute (
                "decay",
                0.28)));

    setKnock (
        static_cast<float> (
            xml->getDoubleAttribute (
                "knock",
                2.0)));

    setRatio (
        static_cast<float> (
            xml->getDoubleAttribute (
                "ratio",
                2.0)));

    setShape (
        static_cast<float> (
            xml->getDoubleAttribute (
                "shape",
                0.2)));

    setTone (
        static_cast<float> (
            xml->getDoubleAttribute (
                "tone",
                0.4)));

    setBody (
        static_cast<float> (
            xml->getDoubleAttribute (
                "body",
                0.5)));

    setDrive (
        static_cast<float> (
            xml->getDoubleAttribute (
                "drive",
                2.0)));

    setBasslineEnabled (
        xml->getBoolAttribute (
            "bassline",
            true));

    setReverbEnabled (
        xml->getBoolAttribute (
            "reverbEnabled",
            false));

    setReverbMix (
        static_cast<float> (
            xml->getDoubleAttribute (
                "reverbMix",
                0.0)));

    setSidechainEnabled (
        xml->getBoolAttribute (
            "sidechainEnabled",
            false));

    setSidechainAmount (
        static_cast<float> (
            xml->getDoubleAttribute (
                "sidechainAmount",
                0.65)));

    setSidechainPosition (
        static_cast<float> (
            xml->getDoubleAttribute (
                "sidechainPosition",
                0.0)));

    setSidechainAfterReverb (
        xml->getBoolAttribute (
            "sidechainAfterReverb",
            false));

    setDeclickerEnabled (
        xml->getBoolAttribute (
            "declickerEnabled",
            false));

    setDeclickerStrength (
        static_cast<float> (
            xml->getDoubleAttribute (
                "declickerStrength",
                0.35)));

    setAnimationsDisabled (
        xml->getBoolAttribute (
            "animationsDisabled",
            false));

    setMinimalisticEnabled (
        xml->getBoolAttribute (
            "minimalisticEnabled",
            false));

    setAdvancedModeEnabled (
        xml->getBoolAttribute (
            "advancedModeEnabled",
            false));

    for (int i = 0; i < AdvancedParameterCount; ++i)
    {
        setAdvancedParameterValue (
            i,
            static_cast<float> (
                xml->getDoubleAttribute (
                    "advanced" + juce::String (i),
                    getAdvancedParameterDefault (i))));
    }

    setBabushkaSafeMode (false);
    setFunDonkLoopEnabled (false);

    setVolume (
        static_cast<float> (
            xml->getDoubleAttribute (
                "volume",
                1.0)));

    setRootNote (
        xml->getIntAttribute (
            "rootNote",
            rootNoteNeutral));

    setExperimentalControlEnabled (
        xml->getBoolAttribute (
            "experimentalControl",
            false));

    setExperimentalRandomEnabled (
        xml->getBoolAttribute (
            "experimentalRandom",
            false));

    const int savedFaceMode = xml->getIntAttribute (
            "faceMode",
            static_cast<int> (FaceMode::StarSlav));

    setFaceMode (savedFaceMode == static_cast<int> (FaceMode::Custom)
            ? FaceMode::Custom
            : FaceMode::StarSlav);

    setCustomFacePath (
        xml->getStringAttribute (
            "customFacePath",
            {}));

    currentPreset = xml->getIntAttribute (
            "preset",
            -1);

    // ta inte tillbaks donk loop från projekt
    setDonkLoopEnabled (false);
}

void NewProjectAudioProcessor::DonkVoice::prepare (double newSampleRate)
{
    sampleRate = newSampleRate;
    reset();
}

void NewProjectAudioProcessor::DonkVoice::reset()
{
    active = false;
    noteHeld = false;
    currentMidiNote = -1;
    velocity = 0.0f;
    carrierPhase = 0.0f;
    subPhase = 0.0f;
    modPhases.fill (0.0f);
    legacyModPhase = 0.0f;
    legacyAmpEnvelope = 0.0f;
    legacyModEnvelope = 0.0f;
    legacyPitchEnvelope = 0.0f;
    legacyReleaseLevel = 0.0f;
    currentFrequency = 261.6256f;
    ampStage = AmpStage::Off;
    ampEnvelope = 0.0f;
    pitchEnvelope = 0.0f;
    subEnvelope = 0.0f;
    modEnvelopes.fill (0.0f);
    filterState = 0.0f;
    voiceAgeSeconds = 0.0;
}

void NewProjectAudioProcessor::DonkVoice::setBasslineEnabled (bool enabled)
{
    basslineEnabled.store (enabled);
}

void NewProjectAudioProcessor::DonkVoice::noteOn (int midiNote, float newVelocity)
{
    currentMidiNote = midiNote;

    velocity = juce::jlimit (
            0.0f,
            1.0f,
            newVelocity);

    const int internalMidiNote = midiNote - 24;

    const double frequency = juce::MidiMessage::getMidiNoteInHertz (
            internalMidiNote);

    currentFrequency = static_cast<float> (
            frequency);

    carrierPhase = 0.0f;
    subPhase = 0.0f;
    modPhases.fill (0.0f);
    legacyModPhase = 0.0f;
    legacyAmpEnvelope = 0.0f;
    legacyModEnvelope = 1.0f;
    legacyPitchEnvelope = 1.0f;
    legacyReleaseLevel = 1.0f;
    ampStage = AmpStage::Attack;
    ampEnvelope = 0.0f;
    modEnvelopes.fill (1.0f);
    subEnvelope = 1.0f;
    pitchEnvelope = 1.0f;
    voiceAgeSeconds = 0.0;
    filterState = 0.0f;
    filterBandState = 0.0f;
    noteHeld = true;
    active = true;
}

void NewProjectAudioProcessor::DonkVoice::noteOff()
{
    if (! active)
        return;

    noteHeld = false;
    ampStage = AmpStage::Release;
    legacyReleaseLevel = 1.0f;
}

float NewProjectAudioProcessor::DonkVoice::wrapPhase (float phase) const
{
    while (phase >= 1.0f)
        phase -= 1.0f;

    while (phase < 0.0f)
        phase += 1.0f;

    return phase;
}

float NewProjectAudioProcessor::DonkVoice::oscillator (float phase, float morph) const
{
    const float twoPi = juce::MathConstants<float>::twoPi;

    const float sine = std::sin (
            phase * twoPi);

    const float triangle = 1.0f
        -
        4.0f *
        std::abs (phase - 0.5f);

    const float saw = 2.0f * phase - 1.0f;

    const float square = phase < 0.5f
            ? 1.0f
            : -1.0f;

    if (morph < 0.3333f)
    {
        const float amount = morph * 3.0f;

        return sine * (1.0f - amount)
             +
               triangle * amount;
    }

    if (morph < 0.6666f)
    {
        const float amount = (morph - 0.3333f) * 3.0f;

        return triangle * (1.0f - amount)
             +
               saw * amount;
    }

    const float amount = (morph - 0.6666f) * 3.0f;

    return saw * (1.0f - amount)
         +
           square * amount;
}

float NewProjectAudioProcessor::DonkVoice::txWave2 (float phase) const
{
    const float p = phase
        * juce::MathConstants<float>::twoPi;

    const float value = std::sin (p)
        + 0.19f * std::sin (p * 3.0f)
        + 0.09f * std::sin (p * 4.0f)
        + 0.03f * std::sin (p * 5.0f)
        + 0.01f * std::sin (p * 7.0f);

    return value / 1.32f;
}

float NewProjectAudioProcessor::DonkVoice::oscillatorWithTx2 (float phase, float shape, float tx2Mix) const
{
    const float normal = oscillator (
            phase,
            shape);

    const float tx2 = txWave2 (
            phase);

    const float mix = juce::jlimit (
            0.0f,
            1.0f,
            tx2Mix);

    return normal * (1.0f - mix)
         + tx2 * mix;
}

float NewProjectAudioProcessor::DonkVoice::process (
    float pitchDrop,
    float decay,
    float knock,
    float ratio,
    float shape,
    float tone,
    float body,
    float drive,
    const std::array<float, AdvancedParameterCount>& advanced,
    bool advancedMode)
{
    if (! active)
        return 0.0f;

    if (! advancedMode)
    {
        voiceAgeSeconds += 1.0 / sampleRate;

        if (voiceAgeSeconds >= maxVoiceLifetimeSeconds)
        {
            noteHeld = false;
            ampStage = AmpStage::Release;
        }

        const float safeDecay = juce::jmax (
                0.01f,
                decay);

        const float attackTime = 0.0015f;
        const float releaseTime = 0.10f;
        const bool basslineOn = basslineEnabled.load();

        const float targetBody = basslineOn
                ? juce::jlimit (0.0f, 1.0f, body)
                : 0.0f;

        // med bassline av ändrar blyat fortfarande känslan på hitten men den
        const float ampDecayTime = basslineOn
                ? safeDecay
                : juce::jlimit (
                      0.065f,
                      0.260f,
                      safeDecay * 0.38f);

        // gamla legacy koden kolla "envelope < 1" för o bestämma om
        switch (ampStage)
        {
            case AmpStage::Attack:
            {
                legacyAmpEnvelope += 1.0f
                    /
                    static_cast<float> (sampleRate * attackTime);

                if (legacyAmpEnvelope >= 1.0f)
                {
                    legacyAmpEnvelope = 1.0f;
                    ampStage = AmpStage::Decay;
                }

                break;
            }

            case AmpStage::Decay:
            {
                const float decayAmount = (1.0f - targetBody)
                    /
                    static_cast<float> (sampleRate * ampDecayTime);

                legacyAmpEnvelope -= decayAmount;

                if (legacyAmpEnvelope <= targetBody)
                {
                    legacyAmpEnvelope = targetBody;
                    ampStage = AmpStage::Sustain;
                }

                break;
            }

            case AmpStage::Sustain:
            {
                if (! noteHeld)
                {
                    ampStage = AmpStage::Release;
                }
                else if (! basslineOn)
                {
                    ampStage = AmpStage::Decay;
                }
                else
                {
                    legacyAmpEnvelope = targetBody;
                }

                break;
            }

            case AmpStage::Release:
            {
                legacyAmpEnvelope -= 1.0f
                    /
                    static_cast<float> (sampleRate * releaseTime);

                if (legacyAmpEnvelope <= 0.0f)
                {
                    legacyAmpEnvelope = 0.0f;
                    ampStage = AmpStage::Off;
                }

                break;
            }

            case AmpStage::Off:
            default:
                legacyAmpEnvelope = 0.0f;
                break;
        }

        const float modDecayTime = 0.10f
            + safeDecay * 0.20f;

        legacyModEnvelope -= 1.0f
            /
            static_cast<float> (sampleRate * modDecayTime);

        legacyModEnvelope = juce::jmax (
                0.0f,
                legacyModEnvelope);

        legacyPitchEnvelope -= 1.0f
            /
            static_cast<float> (sampleRate * 0.065f);

        legacyPitchEnvelope = juce::jmax (
                0.0f,
                legacyPitchEnvelope);

        const float pitchMultiplier = std::pow (
                2.0f,
                (pitchDrop * legacyPitchEnvelope)
                /
                12.0f);

        const float frequency = currentFrequency
            * pitchMultiplier;

        const float carrier = oscillator (
                carrierPhase,
                shape);

        const float modulator = std::sin (
                legacyModPhase
                * juce::MathConstants<float>::twoPi);

        const float phaseMod = modulator
            * legacyModEnvelope
            * knock
            * 0.18f;

        const float modulatedCarrier = oscillator (
                wrapPhase (
                    carrierPhase
                    + phaseMod),
                shape);

        const float donkDifference = (modulatedCarrier - carrier)
            * 2.10f;

        const float transientBite = juce::jlimit (
                0.20f,
                0.50f,
                0.20f
                + std::abs (knock) * 0.045f
                + juce::jlimit (0.0f, 1.0f, tone) * 0.06f);

        float signal = 0.0f;

        if (basslineOn)
        {
            signal = carrier * 0.36f
                + modulatedCarrier * 0.68f
                + donkDifference * (transientBite * 0.52f);
        }
        else
        {
            signal = modulatedCarrier * 0.72f
                + donkDifference * (transientBite * 1.08f);
        }

        carrierPhase = wrapPhase (
                carrierPhase
                + frequency
                / static_cast<float> (sampleRate));

        legacyModPhase = wrapPhase (
                legacyModPhase
                + (frequency * ratio)
                / static_cast<float> (sampleRate));

        const float normalizedDrive = std::tanh (
                drive);

        signal = std::tanh (
                signal * drive)
            /
            juce::jmax (
                0.1f,
                normalizedDrive);

        const float cutoff = 500.0f
            + tone * 14500.0f;

        const float filterAmount = 1.0f
            -
            std::exp (
                -2.0f
                * juce::MathConstants<float>::pi
                * cutoff
                / static_cast<float> (sampleRate));

        filterState += filterAmount
            * (signal - filterState);

        signal = filterState
            * legacyAmpEnvelope
            * velocity;

        if ((! basslineOn
             && legacyAmpEnvelope <= 0.0010f)
            || legacyAmpEnvelope <= 0.0001f
            || ampStage == AmpStage::Off)
        {
            active = false;
            legacyAmpEnvelope = 0.0f;
            legacyReleaseLevel = 0.0f;
            filterState = 0.0f;
            filterBandState = 0.0f;
            return 0.0f;
        }

        return signal * 0.72f;
    }

    voiceAgeSeconds += 1.0 / sampleRate;

    if (voiceAgeSeconds >= maxVoiceLifetimeSeconds)
    {
        noteHeld = false;
        ampStage = AmpStage::Release;
    }

    const float safeDecay = juce::jmax (
            0.02f,
            decay);

    const bool basslineOn = basslineEnabled.load();

    const float attackTime = juce::jmax (
            0.0005f,
            advanced[AmpAttack]);

    const float simpleAmpDecayTime = basslineOn
            ? safeDecay
            : juce::jlimit (
                  0.065f,
                  0.260f,
                  safeDecay * 0.38f);

    const float ampDecayScale = juce::jlimit (
            0.20f,
            5.0f,
            advanced[AmpDecay] / 0.34f);

    const float ampDecayTime = juce::jlimit (
            0.015f,
            2.50f,
            simpleAmpDecayTime * ampDecayScale);

    const float sustainLevel = basslineOn
            ? juce::jlimit (0.0f, 1.0f, body + (advanced[AmpSustain] - 0.34f) * 0.75f)
            : 0.0f;

    const float releaseTime = juce::jlimit (
            0.005f,
            2.0f,
            0.10f * (advanced[AmpRelease] / 0.09f));

    switch (ampStage)
    {
        case AmpStage::Attack:
        {
            ampEnvelope += 1.0f /
                static_cast<float> (sampleRate * attackTime);

            if (ampEnvelope >= 1.0f)
            {
                ampEnvelope = 1.0f;
                ampStage = AmpStage::Decay;
            }

            break;
        }

        case AmpStage::Decay:
        {
            const float amount = (1.0f - sustainLevel)
                /
                static_cast<float> (sampleRate * ampDecayTime);

            ampEnvelope -= amount;

            if (ampEnvelope <= sustainLevel)
            {
                ampEnvelope = sustainLevel;
                ampStage = AmpStage::Sustain;
            }

            break;
        }

        case AmpStage::Sustain:
            ampEnvelope = sustainLevel;
            break;

        case AmpStage::Release:
            ampEnvelope -= 1.0f /
                static_cast<float> (sampleRate * releaseTime);

            if (ampEnvelope <= 0.0f)
            {
                ampEnvelope = 0.0f;
                ampStage = AmpStage::Off;
                active = false;
                filterState = 0.0f;
                filterBandState = 0.0f;
                return 0.0f;
            }
            break;

        case AmpStage::Off:
        default:
            active = false;
            return 0.0f;
    }

    const float pitchDecayTime = juce::jmax (
            0.005f,
            advanced[PitchDecay]);

    pitchEnvelope -= 1.0f /
        static_cast<float> (sampleRate * pitchDecayTime);

    pitchEnvelope = juce::jmax (
            0.0f,
            pitchEnvelope);

    const float pitchMultiplier = std::pow (
            2.0f,
            (pitchDrop * pitchEnvelope)
            / 12.0f);

    // noteon() flyttar redan midi noten ner två oktaver
    const float frequency = currentFrequency
        * pitchMultiplier;

    const float carrierShape = juce::jlimit (
            0.0f,
            1.0f,
            shape);

    const float carrier = oscillatorWithTx2 (
            carrierPhase,
            carrierShape,
            advanced[CarrierTx2]);

    const float modRatios[4] =
    {
        juce::jmax (0.05f, ratio),
        juce::jmax (0.05f, advanced[Fm2Ratio]),
        juce::jmax (0.05f, advanced[Fm3Ratio]),
        juce::jmax (0.05f, advanced[Fm4Ratio])
    };

    const float normalFm1Decay = 0.10f
        + safeDecay * 0.20f;

    const float fm1DecayScale = juce::jlimit (
            0.10f,
            6.0f,
            advanced[Fm1Decay] / 0.16f);

    const float modDecayTimes[4] =
    {
        juce::jmax (0.003f, normalFm1Decay * fm1DecayScale),
        juce::jmax (0.003f, advanced[Fm2Decay]),
        juce::jmax (0.003f, advanced[Fm3Decay]),
        juce::jmax (0.003f, advanced[Fm4Decay])
    };

    const float modSustains[4] =
    {
        juce::jlimit (0.0f, 1.0f, advanced[Fm1Sustain]),
        juce::jlimit (0.0f, 1.0f, advanced[Fm2Sustain]),
        juce::jlimit (0.0f, 1.0f, advanced[Fm3Sustain]),
        juce::jlimit (0.0f, 1.0f, advanced[Fm4Sustain])
    };

    const float modShapes[4] =
    {
        juce::jlimit (0.0f, 1.0f, advanced[Fm1Shape]),
        juce::jlimit (0.0f, 1.0f, advanced[Fm2Shape]),
        juce::jlimit (0.0f, 1.0f, advanced[Fm3Shape]),
        juce::jlimit (0.0f, 1.0f, advanced[Fm4Shape])
    };

    const float modTx2Mix[4] =
    {
        juce::jlimit (0.0f, 1.0f, advanced[Fm1Tx2]),
        juce::jlimit (0.0f, 1.0f, advanced[Fm2Tx2]),
        juce::jlimit (0.0f, 1.0f, advanced[Fm3Tx2]),
        juce::jlimit (0.0f, 1.0f, advanced[Fm4Tx2])
    };

    const float modAmounts[4] =
    {
        juce::jmax (0.0f, knock) * 0.180f,
        juce::jmax (0.0f, advanced[Fm2Amount]) * 0.120f,
        juce::jmax (0.0f, advanced[Fm3Amount]) * 0.082f,
        juce::jmax (0.0f, advanced[Fm4Amount]) * 0.100f
    };
    float phaseMod = 0.0f;

    for (int i = 0; i < 4; ++i)
    {
        const float target = noteHeld
                ? modSustains[i]
                : 0.0f;

        const float envelopeTime = noteHeld
                ? modDecayTimes[i]
                : releaseTime;

        if (modEnvelopes[i] > target)
        {
            modEnvelopes[i] -= (1.0f - target)
                /
                static_cast<float> (sampleRate * envelopeTime);

            modEnvelopes[i] = juce::jmax (
                    target,
                    modEnvelopes[i]);
        }

        const float modSignal = oscillatorWithTx2 (
                modPhases[i],
                modShapes[i],
                modTx2Mix[i]);

        phaseMod += modSignal
            * modEnvelopes[i]
            * modAmounts[i];

        modPhases[i] = wrapPhase (
                modPhases[i]
                + (frequency * modRatios[i])
                / static_cast<float> (sampleRate));
    }

    const float modulatedCarrier = oscillatorWithTx2 (
            wrapPhase (
                carrierPhase
                + phaseMod),
            carrierShape,
            advanced[CarrierTx2]);

    const float carrierLevel = juce::jlimit (
            0.0f,
            1.50f,
            advanced[CarrierLevel]);

    const float donkDifference = (modulatedCarrier - carrier)
        * 2.10f;

    const float transientBite = juce::jlimit (
            0.20f,
            0.50f,
            0.20f
            + std::abs (knock) * 0.045f
            + juce::jlimit (0.0f, 1.0f, tone) * 0.060f);

    float signal = 0.0f;

    if (basslineOn)
    {
        signal = carrier * 0.36f
            + modulatedCarrier * 0.68f
            + donkDifference * (transientBite * 0.52f);
    }
    else
    {
        signal = modulatedCarrier * 0.72f
            + donkDifference * (transientBite * 1.08f);
    }

    signal *= carrierLevel;

    // sub spelar bara när bassline e på
    if (basslineOn)
    {
        const float subDecayTime = juce::jmax (
                0.02f,
                advanced[SubDecay]);

        const float subSustain = juce::jlimit (
                0.0f,
                1.0f,
                advanced[SubSustain]);

        if (subEnvelope > subSustain)
        {
            subEnvelope -= (1.0f - subSustain)
                /
                static_cast<float> (sampleRate * subDecayTime);

            subEnvelope = juce::jmax (
                    subSustain,
                    subEnvelope);
        }

        const float subFrequency = frequency
            * std::pow (
                2.0f,
                advanced[SubOctave]);

        const float subSignal = oscillator (
                subPhase,
                juce::jlimit (0.0f, 1.0f, advanced[SubShape]))
            * subEnvelope
            * juce::jlimit (
                0.0f,
                1.50f,
                advanced[SubLevel]);

        signal += subSignal * 0.62f;

        subPhase = wrapPhase (
                subPhase
                + subFrequency
                / static_cast<float> (sampleRate));
    }

    carrierPhase = wrapPhase (
            carrierPhase
            + frequency
            / static_cast<float> (sampleRate));

    const float drySignal = signal;

    const float safeDrive = juce::jmax (
            0.05f,
            std::abs (drive));

    const float drivenSignal = std::tanh (
            signal * safeDrive)
        /
        juce::jmax (
            0.1f,
            std::tanh (safeDrive));

    const float distMix = juce::jlimit (
            0.0f,
            1.0f,
            advanced[DistortionMix]);

    signal = drySignal * (1.0f - distMix)
        + drivenSignal * distMix;

    const float safeTone = juce::jlimit (
            0.0f,
            1.0f,
            tone);

    const float cutoff = 500.0f
        + safeTone * 14500.0f;

    const float resonance = juce::jlimit (
            0.0f,
            0.92f,
            advanced[FilterResonance]);

    if (resonance <= 0.0005f)
    {
        const float filterAmount = 1.0f
            - std::exp (
                -2.0f
                * juce::MathConstants<float>::pi
                * cutoff
                / static_cast<float> (sampleRate));

        filterState += filterAmount
            * (signal - filterState);

        signal = filterState;
    }
    else
    {
        const float g = std::tan (
                juce::MathConstants<float>::pi
                * cutoff
                / static_cast<float> (sampleRate));

        const float k = juce::jmax (
                0.16f,
                2.0f * (1.0f - resonance));

        const float a1 = 1.0f
            /
            (1.0f
             + g * (g + k));

        const float a2 = g * a1;
        const float a3 = g * a2;
        const float v3 = signal - filterState;
        const float v1 = a1 * filterBandState + a2 * v3;
        const float v2 = filterState + a2 * filterBandState + a3 * v3;

        filterBandState = 2.0f * v1
            - filterBandState;

        filterState = 2.0f * v2
            - filterState;

        const float resonantMix = juce::jlimit (
                0.20f,
                0.78f,
                0.20f + resonance * 0.63f);

        signal = signal * (1.0f - resonantMix)
            + v2 * resonantMix;
    }

    signal *= ampEnvelope
        * velocity;

    if (! active
        || ampEnvelope <= 0.000001f)
    {
        active = false;
        ampStage = AmpStage::Off;
        ampEnvelope = 0.0f;
        filterState = 0.0f;
        filterBandState = 0.0f;
        return 0.0f;
    }

    return signal * 0.72f;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
