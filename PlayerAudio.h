#pragma once       // PlayerAudio.h 
#include <JuceHeader.h> 


class PlayerAudio
{
public:
    PlayerAudio();
    ~PlayerAudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    bool loadFile(const juce::File& file);
    void play();
    void pause();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;
    void setLooping(bool shouldLoop);

    // Feature 6: Speed Control
    void setSpeed(double ratio);
    double getSpeed() const { return speed; }

    // Feature 5: Metadata access
    juce::String getCurrentFileName() const { return currentFileName; }
    juce::String getMetadataInfo() const;

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    // keep looping flag (Feature 4)
    bool looping = false;

    // Feature 6: Resampler sits *on top* of transportSource
    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
    double speed = 1.0;

    // Feature 5: metadata storage
    juce::String currentFileName;
    juce::String currentMetadata;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};