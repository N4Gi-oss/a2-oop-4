#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // Prepare both transport and resampler
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Fill from resampleSource so speed control works
    resampleSource.getNextAudioBlock(bufferToFill);

    // Keep loop behavior on the underlying transport (Feature 4)
    if (looping && transportSource.hasStreamFinished())
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }
}

void PlayerAudio::releaseResources()
{
    resampleSource.releaseResources();
    transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (file.existsAsFile())
    {
        currentFileName = file.getFileName();

        if (auto* reader = formatManager.createReaderFor(file))
        {
            // Disconnect old source
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            // Create new reader source and attach
            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
            transportSource.setSource(readerSource.get(),
                0,
                nullptr,
                reader->sampleRate);

            // Ensure resampler uses current speed
            resampleSource.setResamplingRatio(speed);

            // Read metadata (Feature 5)
            currentMetadata.clear();
            // JUCE AudioFormatReader has metadataValues (StringPairArray) for many formats
            if (reader->metadataValues.size() > 0)
            {
                for (auto& key : reader->metadataValues.getAllKeys())
                {
                    currentMetadata += key + ": " + reader->metadataValues[key] + "\n";
                }
            }
            // fallback to filename
            if (currentMetadata.isEmpty())
                currentMetadata = "File: " + currentFileName + "\n";

            // duration
            double duration = 0.0;
            if (reader->sampleRate > 0)
                duration = static_cast<double>(reader->lengthInSamples) / reader->sampleRate;
            currentMetadata += "Duration: " + juce::String(duration, 2) + " sec\n";

            transportSource.start();
        }
    }
    return true;
}

void PlayerAudio::play()
{
    transportSource.start();
}

void PlayerAudio::pause()
{
    transportSource.stop();
}

void PlayerAudio::stop()
{
    transportSource.stop();
}

void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}

void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}

void PlayerAudio::setLooping(bool shouldLoop)
{
    looping = shouldLoop;
}

void PlayerAudio::setSpeed(double ratio)
{
    if (ratio > 0.05 && ratio <= 4.0)
    {
        speed = ratio;
        resampleSource.setResamplingRatio(speed);
    }
}

double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}

juce::String PlayerAudio::getMetadataInfo() const
{
    return currentMetadata;
}
