#pragma once      // PlayerGUI.h 
#include <JuceHeader.h> 
#include "PlayerAudio.h" 

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
    public juce::ListBoxModel
{
public:
    PlayerGUI();
    ~PlayerGUI() override;
    //hooh

    void paint(juce::Graphics& g) override;
    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragStarted(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;
    void timerCallback() override;

  
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g,
        int width, int height, bool rowIsSelected) override;
    void selectedRowsChanged(int lastRowSelected) override;

private:
    PlayerAudio playerAudio;

   
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton muteButton{ "Mute" };            // Feature 3
    juce::TextButton playButton{ "Play" };
    juce::TextButton pauseButton{ "Pause" };
    juce::TextButton startButton{ "go to start" };
    juce::TextButton endButton{ "go to end" };
    juce::ToggleButton loopButton{ "Loop" };          // Feature 4
    juce::Slider positionSlider;
    juce::Label positionLabel{ {}, "00:00" };
    juce::TextButton setAButton{ "Set A" };           // Feature 10
    juce::TextButton setBButton{ "Set B" };
    juce::ToggleButton abLoopButton{ "A-B Loop" };
   

    juce::Slider volumeSlider;

   
    juce::Slider speedSlider;
    juce::Label speedLabel{ {}, "Speed: 1.0x" };

    
    double progress = 0.0;
    juce::ProgressBar progressBar{ progress };

   
    juce::Label metadataLabel;

  
    juce::TextButton addToPlaylistButton{ "Add to Playlist" };
    juce::ListBox playlistBox;
    juce::StringArray playlist;
    juce::Array<juce::File> playlistFiles;

   
    float previousVolume = 0.5f;
    bool isMuted = false;
    bool isDraggingPosition = false;

   
    double pointA = 0.0;
    double pointB = 0.0;
    bool isABLooping = false;

    std::unique_ptr<juce::FileChooser> fileChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
