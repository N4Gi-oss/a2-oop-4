

#include "PlayerGui.h"
PlayerGUI::PlayerGUI() {
    for (auto* btn : { &loadButton, &restartButton , &stopButton, &playButton, &pauseButton , &startButton, &endButton, &muteButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    addAndMakeVisible(loopButton);
    loopButton.addListener(this);


    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

   

}

PlayerGUI::~PlayerGUI() {
}

void PlayerGUI::resized()
{
    int buttonWidth = 100;
    int buttonHeight = 40;
    int gap = 15;
    int x = 20;
    int y1 = 20;     
    int y2 = 80;    

    
    loadButton.setBounds(x, y1, buttonWidth, buttonHeight); x += buttonWidth + gap;
    restartButton.setBounds(x, y1, buttonWidth, buttonHeight); x += buttonWidth + gap;
    stopButton.setBounds(x, y1, buttonWidth, buttonHeight); x += buttonWidth + gap;
    playButton.setBounds(x, y1, buttonWidth, buttonHeight); x += buttonWidth + gap;
    pauseButton.setBounds(x, y1, buttonWidth, buttonHeight); x += buttonWidth + gap;
    muteButton.setBounds(x, y1, buttonWidth, buttonHeight); x += buttonWidth + gap;

   
    x = 20;
    startButton.setBounds(x, y2, buttonWidth, buttonHeight); x += buttonWidth + gap;
    endButton.setBounds(x, y2, buttonWidth, buttonHeight); x += buttonWidth + gap;
    loopButton.setBounds(x, y2, buttonWidth, buttonHeight); x += buttonWidth + gap;


   
    volumeSlider.setBounds(20, 150, getWidth() - 40, 30);
}


void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select audio files...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    playerAudio.loadFile(file);
                }
            });
    }

    if (button == &restartButton)
    {
        playerAudio.play();
    }

    if (button == &stopButton)
    {
        playerAudio.stop();
        playerAudio.setPosition(0.0);
    }

    // mute 

    if (button == &muteButton)
    {
        if (!isMuted)
        {
            previousVolume = (float)volumeSlider.getValue();
            playerAudio.setGain(0.0f);
            isMuted = true;
            muteButton.setButtonText("Unmute");
        }
        else
        {
            playerAudio.setGain(previousVolume);
            isMuted = false;
            muteButton.setButtonText("Mute");
        }
    }

    // play and pause
    if (button == &playButton)
    {
        playerAudio.play();
    }

    if (button == &pauseButton)
    {
        playerAudio.pause();  
    }

    // go to start/end
    if (button == &startButton)
    {
        playerAudio.setPosition(0.0);
    }

    if (button == &endButton)
    {
        playerAudio.setPosition(playerAudio.getLength());
    }
    if (button == &loopButton)
    {
        bool loopOn = loopButton.getToggleState();
        playerAudio.setLooping(loopOn);
        
        if (loopOn)
            DBG("Loop ON");
        else
            DBG("Loop OFF");

    }



}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
}