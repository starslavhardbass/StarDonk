#pragma once

#include <JuceHeader.h>
#include <atomic>
#include <array>

class NewProjectAudioProcessor : public juce::AudioProcessor
{
public:
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    void prepareToPlay (
        double sampleRate,
        int samplesPerBlock) override;

    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (
        const BusesLayout& layouts) const override;
   #endif

    void processBlock (
        juce::AudioBuffer<float>&,
        juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;

    void setCurrentProgram (
        int index) override;

    const juce::String getProgramName (
        int index) override;

    void changeProgramName (
        int index,
        const juce::String& newName) override;

    void getStateInformation (
        juce::MemoryBlock& destData) override;

    void setStateInformation (
        const void* data,
        int sizeInBytes) override;

    float getPitchDrop() const;
    void setPitchDrop (float value);
    float getDecay() const;
    void setDecay (float value);
    float getKnock() const;
    void setKnock (float value);
    float getRatio() const;
    void setRatio (float value);
    float getShape() const;
    void setShape (float value);
    float getTone() const;
    void setTone (float value);
    float getBody() const;
    void setBody (float value);
    float getDrive() const;
    void setDrive (float value);
    bool getBasslineEnabled() const;
    void setBasslineEnabled (bool enabled);
    bool getReverbEnabled() const;
    void setReverbEnabled (bool enabled);
    float getReverbMix() const;
    void setReverbMix (float value);
    bool getSidechainEnabled() const;
    void setSidechainEnabled (bool enabled);
    float getSidechainAmount() const;
    void setSidechainAmount (float value);
    float getSidechainPosition() const;
    void setSidechainPosition (float value);
    bool getSidechainAfterReverb() const;
    void setSidechainAfterReverb (bool enabled);
    bool getDeclickerEnabled() const;
    void setDeclickerEnabled (bool enabled);
    float getDeclickerStrength() const;
    void setDeclickerStrength (float value);
    bool getAnimationsDisabled() const;
    void setAnimationsDisabled (bool disabled);
    bool getMinimalisticEnabled() const;
    void setMinimalisticEnabled (bool enabled);
    bool getAdvancedModeEnabled() const;
    void setAdvancedModeEnabled (bool enabled);

    enum AdvancedParameter
    {
        CarrierLevel = 0,
        SubLevel,
        SubOctave,
        SubDecay,
        SubSustain,
        SubShape,

        Fm1Decay,
        Fm1Sustain,
        Fm1Shape,

        Fm2Ratio,
        Fm2Amount,
        Fm2Decay,
        Fm2Sustain,
        Fm2Shape,

        Fm3Ratio,
        Fm3Amount,
        Fm3Decay,
        Fm3Sustain,
        Fm3Shape,

        Fm4Ratio,
        Fm4Amount,
        Fm4Decay,
        Fm4Sustain,
        Fm4Shape,

        AmpAttack,
        AmpDecay,
        AmpSustain,
        AmpRelease,

        PitchDecay,
        DistortionMix,

        CarrierTx2,
        Fm1Tx2,
        Fm2Tx2,
        Fm3Tx2,
        Fm4Tx2,

        ClassicBlend,
        ClassicCRatio,
        ClassicCDecay,
        ClassicCToA,
        ClassicCToB,
        ClassicCToE,
        ClassicBRatio,
        ClassicERatio,
        ClassicBLevel,
        ClassicELevel,

        FilterResonance,

        AdvancedParameterCount
    };
    int getNumAdvancedParameters() const;
    juce::String getAdvancedParameterName (int index) const;
    float getAdvancedParameterMinimum (int index) const;
    float getAdvancedParameterMaximum (int index) const;
    float getAdvancedParameterInterval (int index) const;
    float getAdvancedParameterDefault (int index) const;
    float getAdvancedParameterValue (int index) const;
    void setAdvancedParameterValue (int index, float value);
    bool getBabushkaSafeMode() const;
    void setBabushkaSafeMode (bool enabled);
    bool getFunDonkLoopEnabled() const;
    void setFunDonkLoopEnabled (bool enabled);
    float getOutputLevel() const;
    float getVolume() const;
    void setVolume (float value);
    bool getDonkLoopEnabled() const;
    void setDonkLoopEnabled (bool enabled);
    float getDonkLoopSpeed() const;
    void setDonkLoopSpeed (float seconds);
    bool getExperimentalControlEnabled() const;
    void setExperimentalControlEnabled (bool enabled);
    bool getExperimentalRandomEnabled() const;
    void setExperimentalRandomEnabled (bool enabled);
    int getRootNote() const;
    void setRootNote (int midiNote);
    void setComputerKeyboardNote (int midiNote);

    enum class FaceMode
    {
        StarSlav = 1,
        Custom = 2
    };
    FaceMode getFaceMode() const;
    void setFaceMode (FaceMode mode);
    juce::String getCustomFacePath() const;
    void setCustomFacePath (const juce::String& path);
    void randomizeSound();

    struct Preset
    {
        juce::String name;
        juce::String relativePath;
        float pitchDrop = 7.0f;
        float decay     = 0.28f;
        float knock     = 2.0f;
        float ratio     = 2.0f;
        float shape     = 0.20f;
        float tone      = 0.40f;
        float body      = 0.50f;
        float drive     = 2.0f;
        bool bassline = true;

        // reverb
        bool  reverbEnabled = false;
        float reverbMix     = 0.0f;

        // intern sidechain duck
        bool  sidechainEnabled     = false;
        float sidechainAmount      = 0.65f;
        float sidechainPosition    = 0.0f;
        bool  sidechainAfterReverb = false;

        // declicker grejen
        bool  declickerEnabled  = false;
        float declickerStrength = 0.35f;

        // volym
        float volume = 1.0f;

        // experiment mode state som va när preseten sparades
        bool experimentalControl = false;
        bool experimentalRandom  = false;

        // djupa synth motor värden, gamla presets kör bara default
        std::array<float, AdvancedParameterCount> advancedValues {};
    };
    int getNumPresets() const;

    juce::String getPresetName (
        int index) const;

    juce::String getPresetRelativePath (
        int index) const;

    int getCurrentPresetIndex() const;
    void loadPreset (int index);

    bool presetNameExists (
        const juce::String& name,
        const juce::String& relativeFolder = {}) const;

    void saveCurrentAsPreset (
        const juce::String& name,
        const juce::String& relativeFolder = {});

    juce::File getUserPresetDirectory() const;
    void refreshUserPresets();
    float getDonkActivity() const;
    uint64_t getNoteTriggerCount() const;

private:
    class DonkVoice
    {
    public:
        DonkVoice() = default;
        ~DonkVoice() = default;
        void prepare (double newSampleRate);
        void reset();

        void noteOn (
            int midiNote,
            float velocity);

        void noteOff();
        void setBasslineEnabled (bool enabled);

        float process (
            float pitchDrop,
            float decay,
            float knock,
            float ratio,
            float shape,
            float tone,
            float body,
            float drive,
            const std::array<float, AdvancedParameterCount>& advanced,
            bool advancedMode);

    private:
        double sampleRate = 44100.0;
        bool active = false;
        bool noteHeld = false;
        int currentMidiNote = -1;
        float velocity = 0.0f;
        float carrierPhase = 0.0f;
        float subPhase = 0.0f;
        std::array<float, 4> modPhases {};
        float legacyModPhase = 0.0f;
        float legacyAmpEnvelope = 0.0f;
        float legacyModEnvelope = 0.0f;
        float legacyPitchEnvelope = 0.0f;
        float legacyReleaseLevel = 0.0f;
        float currentFrequency = 261.6256f;
        std::atomic<bool> basslineEnabled { true };

        enum class AmpStage
        {
            Off,
            Attack,
            Decay,
            Sustain,
            Release
        };
        AmpStage ampStage = AmpStage::Off;
        float ampEnvelope = 0.0f;
        float pitchEnvelope = 0.0f;
        float subEnvelope = 0.0f;
        std::array<float, 4> modEnvelopes {};

        // tpt state variable lowpass states som advanced mode använder
        float filterState = 0.0f;
        float filterBandState = 0.0f;
        double voiceAgeSeconds = 0.0;
        static constexpr double maxVoiceLifetimeSeconds = 6.00;

        float oscillator (
            float phase,
            float shape) const;

        float txWave2 (
            float phase) const;

        float oscillatorWithTx2 (
            float phase,
            float shape,
            float tx2Mix) const;

        float wrapPhase (
            float phase) const;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DonkVoice)
    };
    juce::AudioParameterFloat* pitchDropParameter = nullptr;
    juce::AudioParameterFloat* decayParameter     = nullptr;
    juce::AudioParameterFloat* knockParameter     = nullptr;
    juce::AudioParameterFloat* ratioParameter     = nullptr;
    juce::AudioParameterFloat* shapeParameter     = nullptr;
    juce::AudioParameterFloat* toneParameter      = nullptr;
    juce::AudioParameterFloat* bodyParameter      = nullptr;
    juce::AudioParameterFloat* driveParameter     = nullptr;
    juce::AudioParameterBool* basslineParameter = nullptr;
    juce::AudioParameterBool* reverbEnabledParameter = nullptr;
    juce::AudioParameterFloat* reverbMixParameter = nullptr;
    juce::AudioParameterBool* sidechainEnabledParameter = nullptr;
    juce::AudioParameterFloat* sidechainAmountParameter = nullptr;
    juce::AudioParameterFloat* sidechainPositionParameter = nullptr;
    juce::AudioParameterBool* sidechainAfterReverbParameter = nullptr;
    juce::AudioParameterFloat* volumeParameter = nullptr;
    juce::AudioParameterBool* declickerParameter = nullptr;
    juce::AudioParameterFloat* declickerStrengthParameter = nullptr;
    std::array<juce::AudioParameterFloat*, AdvancedParameterCount> advancedParameters {};
    std::array<std::atomic<float>, AdvancedParameterCount> advancedValues;

    // root note param som hosten kan se
    juce::AudioParameterChoice* rootNoteParameter = nullptr;
    static constexpr int rootNoteMinimum = 36;
    static constexpr int rootNoteMaximum = 84;
    static constexpr int rootNoteNeutral = 60;
    std::atomic<int> computerKeyboardRequestedNote { -1 };
    int computerKeyboardPlayingNote = -1;
    int transposeIncomingMidiNote (int midiNote) const;
    std::atomic<float> pitchDrop { 7.0f };
    std::atomic<float> decay     { 0.28f };
    std::atomic<float> knock     { 2.0f };
    std::atomic<float> ratio     { 2.0f };
    std::atomic<float> shape     { 0.20f };
    std::atomic<float> tone      { 0.40f };
    std::atomic<float> body      { 0.50f };
    std::atomic<float> drive     { 2.0f };
    std::atomic<bool> basslineEnabled { true };
    std::atomic<bool> reverbEnabled { false };

    // sparas som 0.0 till 1.0
    std::atomic<float> reverbMix { 0.0f };

    // position flyttar ducken genom donken
    std::atomic<bool> sidechainEnabled { false };
    std::atomic<float> sidechainAmount { 0.65f };
    std::atomic<float> sidechainPosition { 0.0f };
    std::atomic<bool> sidechainAfterReverb { false };
    int64_t sidechainSampleCounter = 0;
    float sidechainSmoothedGain = 1.0f;
    juce::AudioBuffer<float> sidechainGainScratch;

    // extra retrigger crossfade för o ta bort klick
    std::atomic<bool> declickerEnabled { false };
    std::atomic<float> declickerStrength { 0.35f };
    std::atomic<bool> animationsDisabled { false };
    std::atomic<bool> minimalisticEnabled { false };
    std::atomic<bool> advancedModeEnabled { false };
    std::atomic<bool> babushkaSafeMode { false };
    std::atomic<bool> funDonkLoopEnabled { false };
    std::atomic<float> outputLevel { 0.0f };
    float lastSynthOutput = 0.0f;
    float declickStartOutput = 0.0f;
    int declickSamplesRemaining = 0;
    int declickTotalSamples = 0;

    // sparas som 0.0 till 1.0
    std::atomic<float> volume { 1.0f };

    // switch på ui sidan
    std::atomic<bool> donkLoopEnabled { false };

    // 1.0 sek e vanliga loop farten
    std::atomic<float> donkLoopSpeed { 1.0f };

    // loop state bara för audio tråden
    bool audioLoopWasEnabled = false;

    // samples sen senaste loop hit
    int64_t loopSampleCounter = 0;

    // samples sen nuvarande loop noten börja
    int64_t loopNoteCounter = 0;

    // hur länge varje test not hålls
    static constexpr double donkLoopNoteLengthSeconds = 0.20;
    std::atomic<bool> experimentalControlEnabled { false };
    std::atomic<bool> experimentalRandomEnabled { false };

    // starslav e default face, gamla ritade smileyn e borttagen
    std::atomic<int> faceMode
    {
        static_cast<int> (FaceMode::StarSlav)
    };
    mutable juce::CriticalSection faceLock;
    juce::String customFacePath;
    std::atomic<float> donkActivity { 0.0f };
    std::atomic<uint64_t> noteTriggerCount { 0 };
    juce::Array<Preset> presets;
    int currentPreset = -1;
    juce::CriticalSection presetLock;

    bool readDonkFile (
        const juce::File& file,
        Preset& preset) const;

    bool writeDonkFile (
        const juce::File& file,
        const Preset& preset);

    juce::File getGlobalSettingsFile() const;
    void loadGlobalSettings();
    void saveGlobalSettings() const;
    juce::Random random;
    DonkVoice voice;
    double currentSampleRate = 44100.0;
    juce::Reverb reverb;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};
