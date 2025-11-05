#pragma once      // PlayerGUI.h 
#include <JuceHeader.h> 
#include "PlayerAudio.h" 

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

private:
    PlayerAudio playerAudio;

    // GUI elements 
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton pauseButton{ "Pause" };
    juce::TextButton startButton{ "go to start" };
    juce::TextButton endButton{ "go to end" };
    juce::ToggleButton loopButton{ "Loop" };
    juce::Slider volumeSlider;

    // for mute 
    float previousVolume = 0.5f;
    bool isMuted = false;
    
    //---------------//

    std::unique_ptr<juce::FileChooser> fileChooser;

    // Event handlers 
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};