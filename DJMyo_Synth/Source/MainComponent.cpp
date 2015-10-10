/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include <vector>
#include <time.h>
#include <string>
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../DJMyo/djmyo_imu.h"

using namespace std;
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

#define A_4 440.00
#define B_4 493.88
#define C_4 261.63
#define D_4 293.66
#define E_4 329.63
#define F_4 349.23
#define G_4 392.00
#define AMPLITUDE 0.9

class myoThread : public Thread
{
public:
	DataCollector collector;
	myoThread() : Thread("myo"){
		collector = DataCollector();
	};
	myoThread(char* currentNote) : Thread("myo"){
		collector = DataCollector(currentNote);

	};
	void run(){
		myo::Hub hub("com.tamuhack.djmyo");
		hub.setLockingPolicy(myo::Hub::lockingPolicyNone);
		Logger::getCurrentLogger()->writeToLog("Attempting to find a Myo...\n");
		myo::Myo* myo = hub.waitForMyo(10000);
		if (!myo) {
			throw std::runtime_error("Unable to find a Myo!");
		}
		Logger::getCurrentLogger()->writeToLog("Connected to a Myo!\n\n");
		hub.addListener(&collector);
		Logger::getCurrentLogger()->writeToLog("Setting origin. Hold Still\n");
		hub.run(2000);
		collector.setOrigin();
		myo->vibrate(myo::Myo::vibrationMedium);
		while (!threadShouldExit()){
			hub.run(1000 / 120);
			//Logger::getCurrentLogger()->writeToLog(collector.print());
		}
		Logger::getCurrentLogger()->writeToLog("Done with Myo Thread, exiting\n");
		signalThreadShouldExit();
	};
	void startMyo(){
		
	};
	myoThread& operator=(const myoThread& rhs){
		this->collector = rhs.collector;
		return *this;
	}
};

class MainContentComponent : public AudioAppComponent
{
public:
    //==============================================================================

	MainContentComponent()
	:	currentSampleRate(0.0),
		currentAngle(0.0),
		angleDelta(0.0)
	{
        setSize (800, 600);
		currentNote = new char('A');
        // specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
		myoData = myoThread(currentNote);
		myoData.run();
		
    }
		
    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //=======================================================================
	void updateAngleDelta()
	{
		double cyclesPerSample;
		switch (*currentNote){
		case 'A':
			cyclesPerSample = A_4 / currentSampleRate;
			break;
		case 'B':
			cyclesPerSample = B_4 / currentSampleRate;
			break;
		case 'C':
			cyclesPerSample = C_4 / currentSampleRate;
			break;
		case 'D':
			cyclesPerSample = D_4 / currentSampleRate;
			break;
		case 'E':
			cyclesPerSample = E_4 / currentSampleRate;
			break;
		case 'F':
			cyclesPerSample = F_4 / currentSampleRate;
			break;
		case 'G':
			cyclesPerSample = G_4 / currentSampleRate;
			break;
		default:
			cyclesPerSample = 0 / currentSampleRate;
			break;
		}

		angleDelta = cyclesPerSample * 2.0 * double_Pi;                                // [3]
	}
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
	{
		// This function will be called when the audio device is started, or when
		// its settings (i.e. sample rate, block size, etc) are changed.
		currentSampleRate = sampleRate;
		updateAngleDelta();
	}
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()
		const float level = AMPLITUDE;
		float* const buffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
		char note[2] = { *currentNote, '\0' };
		Logger::getCurrentLogger()->writeToLog("Playing Audio "+String(note));
		for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
		{
			const float currentSample = (float)std::sin(currentAngle);
			currentAngle += angleDelta;
			buffer[sample] = currentSample * level;
		}
		updateAngleDelta();
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
		Logger::getCurrentLogger()->writeToLog("Releasing audio resources");
		myoData.signalThreadShouldExit();
    }

    //=======================================================================
    void paint (Graphics& g) override
    {
		//Logger::getCurrentLogger()->writeToLog("Entered paint()");
		repaint();
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::black);
        // You can add your drawing code here!
		g.setColour(Colours::lightsteelblue);

		g.setFont(40.0f);

		g.drawLine(getWidth() / 3.0f, 0, getWidth() / 3.0f, getHeight(), 4.0f);
		g.drawLine(getWidth() - getWidth() / 3.0f, 0, getWidth() - getWidth() / 3.0f, getHeight(), 4.0f);
		g.drawLine(0, getHeight() / 3.0f, getWidth() - getWidth() / 3.0f, getHeight() / 3.0f, 4.0f);
		g.drawLine(0, getHeight() - getHeight() / 3.0f, getWidth(), getHeight() - getHeight() / 3.0f, 4.0f);

		g.drawText("A", getWidth() / 6.0f - 20.0f, getHeight() - getHeight() / 6.0f - 20.0f, 40, 40, true);
		g.drawText("B", getWidth() - getWidth() / 2.0f - 20.0f, getHeight() - getHeight() / 6.0f - 20.0f, 40, 40, true);
		g.drawText("C", getWidth() - getWidth() / 6.0f - 20.0f, getHeight() - getHeight() / 6.0f - 20.0f, 40, 40, true);
		g.drawText("D", getWidth() / 6.0f - 20.0f, getHeight() / 2.0f - 20.0f, 40, 40, true);
		g.drawText("E", getWidth() - getWidth() / 2.0f - 20.0f, getHeight() / 2.0f - 20.0f, 40, 40, true);
		g.drawText("F", getWidth() / 6.0f - 20.0f, getHeight() / 6.0f - 20.0f, 40, 40, true);
		g.drawText("G", getWidth() - getWidth() / 2.0f - 20.0f, getHeight() / 6.0f - 20.0f, 40, 40, true);
		
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }


private:
    //==============================================================================
	double sampleRate;
	int expectedSamplesPerBlock;
	double currentSampleRate, currentAngle, angleDelta;
	char *currentNote;
	myoThread myoData;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
