#include "PlayerGui.h"
using namespace juce;

PlayerGUI::PlayerGUI()
{
   
    for (auto* btn : { &loadButton, &restartButton , &stopButton, &playButton, &pauseButton , &startButton, &endButton, &muteButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    
    addAndMakeVisible(setAButton);
    setAButton.addListener(this);

    addAndMakeVisible(setBButton);
    setBButton.addListener(this);

    addAndMakeVisible(abLoopButton);
    abLoopButton.addListener(this);

    addAndMakeVisible(loopButton);
    loopButton.addListener(this);

   
    addAndMakeVisible(positionSlider);
    positionSlider.setRange(0.0, 1.0, 0.0001);
    positionSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    positionSlider.addListener(this);

    addAndMakeVisible(positionLabel);
    positionLabel.setJustificationType(juce::Justification::centredLeft);
    positionLabel.setMinimumHorizontalScale(1.0f);


    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(previousVolume);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

  
    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);
    

    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed: 1.0x", juce::dontSendNotification);

    // Feature 9: progress bar
    addAndMakeVisible(progressBar);

    // Feature 5: metadata label
    addAndMakeVisible(metadataLabel);
    metadataLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    metadataLabel.setJustificationType(juce::Justification::topLeft);
    metadataLabel.setColour(juce::Label::backgroundColourId, juce::Colours::transparentBlack);

    // Feature 8: playlist UI
    addAndMakeVisible(addToPlaylistButton);
    addToPlaylistButton.addListener(this);
    addAndMakeVisible(playlistBox);
    playlistBox.setModel(this);

    startTimerHz(10);
    
    auto styleButton = [](juce::TextButton& btn, juce::Colour base)
        {
            btn.setColour(juce::TextButton::buttonColourId, base);
            btn.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
            btn.setColour(juce::TextButton::buttonOnColourId, base.brighter(0.2f));
            btn.setAlpha(0.9f);
            btn.setWantsKeyboardFocus(false);
            btn.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            btn.setClickingTogglesState(false);
        };

    styleButton(playButton, juce::Colour(0xff2ecc71));
    styleButton(pauseButton, juce::Colour(0xfff39c12)); 
    styleButton(stopButton, juce::Colour(0xffe74c3c));  
    styleButton(restartButton, juce::Colour(0xff3498db)); 
    styleButton(muteButton, juce::Colour(0xff95a5a6));   
    styleButton(setAButton, juce::Colour(0xff1abc9c));
    styleButton(setBButton, juce::Colour(0xff16a085));
    styleButton(addToPlaylistButton, juce::Colour(0xff2c3e50));

    auto styleSlider = [](juce::Slider& s, juce::Colour c)
        {
            s.setColour(juce::Slider::thumbColourId, juce::Colours::white);
            s.setColour(juce::Slider::trackColourId, c);
            s.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
            s.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
            s.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        };

    styleSlider(volumeSlider, juce::Colour(0xff2ecc71));
    styleSlider(speedSlider, juce::Colour(0xff3498db));
    styleSlider(positionSlider, juce::Colour(0xff9b59b6)); // Purple line


}

PlayerGUI::~PlayerGUI()
{
    stopTimer();
}

void PlayerGUI::resized()
{
    int margin = 20;
    int y = 70;
    int h = 35;
    int gap = 10;

    
    int playlistWidth = 250;
    playlistBox.setBounds(margin, y, playlistWidth, getHeight() - 200);
    addToPlaylistButton.setBounds(margin, getHeight() - 110, playlistWidth, h);

    
    int controlWidth = getWidth() - playlistWidth - 380;
    int controlX = playlistWidth + margin + 20;
    int x = controlX;

  
    loadButton.setBounds(x, y, 120, h);
    y += h + gap;

   
    playButton.setBounds(x, y, 80, h);
    pauseButton.setBounds(x + 90, y, 80, h);
    stopButton.setBounds(x + 180, y, 80, h);
    restartButton.setBounds(x + 270, y, 100, h);
    muteButton.setBounds(x + 380, y, 80, h);
    loopButton.setBounds(x + 470, y, 80, h);
    y += h + gap;

   
    positionSlider.setBounds(x, y, controlWidth, 20);
    positionLabel.setBounds(x + controlWidth + 10, y - 2, 60, 24);
    y += 35;

   
    volumeSlider.setBounds(x, y, (controlWidth / 2) - 20, 20);
    speedSlider.setBounds(x + (controlWidth / 2) + 10, y, (controlWidth / 2) - 20, 20);
    y += 35;
    speedLabel.setBounds(x, y, 200, 20);
    y += 25;

   
    progressBar.setBounds(x, y, controlWidth, 20);
    y += 40;

   
    setAButton.setBounds(x, y, 90, h);
    setBButton.setBounds(x + 100, y, 90, h);
    abLoopButton.setBounds(x + 200, y, 100, h);
    startButton.setBounds(x + 310, y, 100, h);
    endButton.setBounds(x + 420, y, 100, h);
    y += h + gap;

   
    metadataLabel.setBounds(getWidth() - 340, 70, 320, getHeight() - 100);
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
   
    juce::ColourGradient backgroundGradient(
        juce::Colour(0xff1e1e1e), 0, 0,
        juce::Colour(0xff2e2e3a), 0, (float)getHeight(),
        false
    );
    g.setGradientFill(backgroundGradient);
    g.fillAll();

  
    g.setColour(juce::Colour(0xff0a84ff));
    g.fillRect(0, 0, getWidth(), 50);

    
    g.setColour(juce::Colours::white);
    g.setFont(22.0f);
    g.drawText("  JUCE Pro Audio Player", 20, 10, getWidth(), 30, juce::Justification::topLeft);

    
    g.setColour(juce::Colours::darkgrey);
    g.drawLine(0.0f, 50.0f, (float)getWidth(), 50.0f, 1.0f);
}


void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>("Select audio file...", juce::File{}, "*.mp3;*.wav");
        fileChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& chooser)
            {
                auto file = chooser.getResult();
                if (file.existsAsFile())
                {
                    playerAudio.loadFile(file);
                    metadataLabel.setText(playerAudio.getMetadataInfo(), juce::dontSendNotification);
                }
            });
    }

    if (button == &addToPlaylistButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>("Add to playlist...", juce::File{}, "*.mp3;*.wav");
        fileChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectMultipleItems,
            [this](const juce::FileChooser& chooser)
            {
                auto results = chooser.getResults();
                for (auto& f : results)
                {
                    playlist.add(f.getFileName());
                    playlistFiles.add(f);
                }
                playlistBox.updateContent();
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

    // Play / Pause
    if (button == &playButton)
        playerAudio.play();

    if (button == &pauseButton)
        playerAudio.pause();

    // Go to start / end
    if (button == &startButton)
        playerAudio.setPosition(0.0);

    if (button == &endButton)
        playerAudio.setPosition(playerAudio.getLength());

    // Loop toggle (Feature 4)
    if (button == &loopButton)
    {
        bool loopOn = loopButton.getToggleState();
        playerAudio.setLooping(loopOn);
        if (loopOn) DBG("Loop ON"); else DBG("Loop OFF");
    }

    // A-B loop set points (Feature 10)
    if (button == &setAButton)
    {
        pointA = playerAudio.getPosition();
        DBG("Set A at: " << pointA);
    }

    if (button == &setBButton)
    {
        pointB = playerAudio.getPosition();
        DBG("Set B at: " << pointB);
    }

    if (button == &abLoopButton)
    {
        isABLooping = abLoopButton.getToggleState();
        if (isABLooping) DBG("A-B Loop Activated"); else DBG("A-B Loop Deactivated");
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        playerAudio.setGain((float)slider->getValue());
    }
    else if (slider == &speedSlider)
    {
        double newSpeed = slider->getValue();
        playerAudio.setSpeed(newSpeed);
        speedLabel.setText(juce::String::formatted("Speed: %.2fx", newSpeed), juce::dontSendNotification);
    }
    else if (slider == &positionSlider)
    {
        if (playerAudio.getLength() > 0.0)
        {
            double total = playerAudio.getLength();
            double seconds = positionSlider.getValue() * total;
            int s = (int)std::round(seconds);
            int mins = s / 60;
            int secs = s % 60;
            positionLabel.setText(juce::String::formatted("%02d:%02d", mins, secs), juce::dontSendNotification);
        }
    }
}

void PlayerGUI::sliderDragStarted(juce::Slider* slider)
{
    if (slider == &positionSlider)
        isDraggingPosition = true;
}

void PlayerGUI::sliderDragEnded(juce::Slider* slider)
{
    if (slider == &positionSlider)
    {
        isDraggingPosition = false;
        double total = playerAudio.getLength();
        if (total > 0.0)
        {
            double newPos = positionSlider.getValue() * total;
            playerAudio.setPosition(newPos);
        }
    }
}

void PlayerGUI::timerCallback()
{
    if (!isDraggingPosition)
    {
        double total = playerAudio.getLength();
        if (total > 0.0)
        {
            double pos = playerAudio.getPosition();
            double norm = pos / total;
            positionSlider.setValue(norm, juce::dontSendNotification);

            int s = (int)std::round(pos);
            int mins = s / 60;
            int secs = s % 60;
            positionLabel.setText(juce::String::formatted("%02d:%02d", mins, secs), juce::dontSendNotification);

           
            progress = norm;
            progressBar.repaint();
        }
        else
        {
            positionSlider.setValue(0.0, juce::dontSendNotification);
            positionLabel.setText("00:00", juce::dontSendNotification);
            progress = 0.0;
            progressBar.repaint();
        }
    }

    // A-B loop enforcement (Feature 10)
    if (isABLooping && pointB > pointA)
    {
        double currentPos = playerAudio.getPosition();
        if (currentPos >= pointB)
        {
            playerAudio.setPosition(pointA);
            playerAudio.play();
        }
    }
}

int PlayerGUI::getNumRows()
{
    return playlist.size();
}

void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::lightblue);
    g.setColour(juce::Colours::white);
    if (isPositiveAndBelow(rowNumber, playlist.size()))
        g.drawText(playlist[rowNumber], 5, 0, width, height, juce::Justification::centredLeft);
}

void PlayerGUI::selectedRowsChanged(int lastRowSelected)
{
    if (isPositiveAndBelow(lastRowSelected, playlistFiles.size()))
    {
        auto file = playlistFiles[lastRowSelected];
        playerAudio.loadFile(file);
        metadataLabel.setText(playerAudio.getMetadataInfo(), juce::dontSendNotification);
    }
}
