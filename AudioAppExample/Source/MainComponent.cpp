/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include <time.h>
#include "djmyo_imu.h"
#include <string>
using namespace std;

//==============================================================================
class MainContentComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainContentComponent()
		: phase({ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
		phaseDelta({ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
		frequency({ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
		amplitude({ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
		xcoor({ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
		ycoor({ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }),
          sampleRate (0.0),
          expectedSamplesPerBlock (0)
    {
        setSize (600, 600);
        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
		initializeMyo();
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //=======================================================================
    void prepareToPlay (int samplesPerBlockExpected, double newSampleRate) override
    {
        sampleRate = newSampleRate;
        expectedSamplesPerBlock = samplesPerBlockExpected;
    }

    /*  This method generates the actual audio samples.
        In this example the buffer is filled with a sine wave whose frequency and
        amplitude are controlled by the mouse position.
     */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();
		
		getMyoAudio();

		time_t timer;
		struct tm y2k = { 0 };
		int seconds;

		y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
		y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

		time(&timer);  /* get current time; same as: timer = time(NULL)  */

		seconds = (int)difftime(timer, mktime(&y2k)) %10 ;

		int next;
		if (seconds == 9)
		{
			next = 0;
		}
		else
		{
			next = seconds + 1;
		}
		phase[seconds] = std::fmod(phase[seconds] + phaseDelta[seconds], float_Pi * 2.0f);
		const float originalPhase = phase[seconds];

        for (int chan = 0; chan < bufferToFill.buffer->getNumChannels(); ++chan)
        {
			phase[seconds] = originalPhase;

            float* const channelData = bufferToFill.buffer->getWritePointer (chan, bufferToFill.startSample);

            for (int i = 0; i < bufferToFill.numSamples ; ++i)
            {
				channelData[i] = amplitude[seconds] * std::sin(phase[seconds]) * 50;

                // increment the phase step for the next sample
				phase[seconds] += phaseDelta[seconds];
				//phase[seconds] = std::fmod(phase[seconds] + phaseDelta[seconds], float_Pi * 2.0f);
            }
        }
    }

    void releaseResources() override
    {
        // This gets automatically called when audio device parameters change
        // or device is restarted.
    }


    //=======================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)

		repaint();

		time_t timer;
		struct tm y2k = { 0 };
		int seconds;

		y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
		y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

		time(&timer);  /* get current time; same as: timer = time(NULL)  */

		seconds = (int)difftime(timer, mktime(&y2k)) % 10;


        g.fillAll (Colours::maroon);

        const float centreY = getHeight() - getHeight() / 3.0f;
		const float centerX = getWidth() - getWidth() / 6.0f;
        const float radius = 40.0f;

        // Draw an ellipse based on the mouse position and audio volume
        g.setColour (Colours::white);
        g.drawEllipse  (xcoor[seconds] - radius / 2.0f,
                        ycoor[seconds] - radius / 2.0f,
                        radius, radius, 2.0f);

        // draw a representative sinewave
		

        Path wavePath;
        wavePath.startNewSubPath (centerX, centreY);

        for (float y = 0.0f; y < centreY; ++y)
			wavePath.lineTo(centerX + (amplitude[seconds] - 0.85f) * getWidth() * 2.0f * std::sin(y * frequency[seconds] * 0.0001f), y);

        g.setColour (Colours::lightsteelblue);
        g.strokePath (wavePath, PathStrokeType (5.0f));

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

    // Mouse handling..
    void mouseDown (const MouseEvent& e) override
    {
		lastMousePosition = e.position;
		//All 4th octave
		if (e.x < getWidth()/3.0f && e.y > getHeight() - getHeight()/3.0f)	//A
		{
			frequency[count] = 440.00;
			amplitude[count] = 0.9f;
			phaseDelta[count] = (float)(2.0 * double_Pi * frequency[count] / sampleRate);
			xcoor[count] = getWidth() / 6.0f - 10.0f;
			ycoor[count] = getHeight() - getHeight() / 6.0f;
		}
		else if (e.x < getWidth() - getWidth() / 3.0f && e.y > getHeight() - getHeight() / 3.0f)	//B
		{
			frequency[count] = 493.88f;
			amplitude[count] = 0.9f;
			phaseDelta[count] = (float)(2.0 * double_Pi * frequency[count] / sampleRate);
			xcoor[count] = getWidth() / 2.0f - 5.0f;
			ycoor[count] = getHeight() - getHeight() / 6.0f;
		}
		else if (e.x <= getWidth() && e.y > getHeight() - getHeight() / 3.0f)	//C
		{
			frequency[count] = 261.63f;
			amplitude[count] = 0.9f;
			phaseDelta[count] = (float)(2.0 * double_Pi * frequency[count] / sampleRate);
			xcoor[count] = getWidth() - getWidth() / 6.0f - 10.0f;
			ycoor[count] = getHeight() - getHeight() / 6.0f;
		}
		else if (e.x < getWidth() / 3.0f && e.y > getHeight() / 3.0f)	//D
		{
			frequency[count] = 293.66f;
			amplitude[count] = 0.9f;
			phaseDelta[count] = (float)(2.0 * double_Pi * frequency[count] / sampleRate);
			xcoor[count] = getWidth() / 6.0f - 10.0f;
			ycoor[count] = getHeight() / 2.0f;
		}
		else if (e.x < getWidth() - getWidth() / 3.0f && e.y > getHeight() / 3.0f)	//E
		{
			frequency[count] = 329.63f;
			amplitude[count] = 0.9f;
			phaseDelta[count] = (float)(2.0 * double_Pi * frequency[count] / sampleRate);
			xcoor[count] = getWidth() / 2.0f - 5.0f;
			ycoor[count] = getHeight() / 2.0f;
		}
		else if (e.x < getWidth() / 3.0f && e.y >= 0.0f)	//F
		{
			frequency[count] = 349.23f;
			amplitude[count] = 0.9f;
			phaseDelta[count] = (float)(2.0 * double_Pi * frequency[count] / sampleRate);
			xcoor[count] = getWidth() / 6.0f - 10.0f;
			ycoor[count] = getHeight() / 6.0f;
		}
		else if (e.x < getWidth() - getWidth() / 3.0f && e.y >= 0.0)	//G
		{
			frequency[count] = 392.00f;
			amplitude[count] = 0.9f;
			phaseDelta[count] = (float)(2.0 * double_Pi * frequency[count] / sampleRate);
			xcoor[count] = getWidth() / 2.0f - 5.0f;
			ycoor[count] = getHeight() / 6.0f;
		}
	
		if (count < 9)
		{
			count++;
		}
		else
		{
			count = 0;
		}

		//repaint();
		//mouseDrag (e);
    }
	/*
    void mouseUp (const MouseEvent&) override
    {
      //  amplitude = 0.0f;
        repaint();
    }

	void mouseMove(const MouseEvent&) override
	{
		
	}
*/
    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }

	int initializeMyo()
	{
		try {

			// First, we create a Hub with our application identifier. Be sure not to use the com.example namespace when
			// publishing your application. The Hub provides access to one or more Myos.
			myo::Hub hub("com.tamuhack.djmyo");
			hub.setLockingPolicy(myo::Hub::lockingPolicyNone);
	//		std::cout << "Attempting to find a Myo..." << std::endl;

			// Next, we attempt to find a Myo to use. If a Myo is already paired in Myo Connect, this will return that Myo
			// immediately.
			// waitForMyo() takes a timeout value in milliseconds. In this case we will try to find a Myo for 10 seconds, and
			// if that fails, the function will return a null pointer.
			myo::Myo* myo = hub.waitForMyo(10000);

			// If waitForMyo() returned a null pointer, we failed to find a Myo, so exit with an error message.
			if (!myo) {
				throw std::runtime_error("Unable to find a Myo!");
			}

			// We've found a Myo.
	//		std::cout << "Connected to a Myo armband!" << std::endl << std::endl;

			// Next we construct an instance of our DeviceListener, so that we can register it with the Hub.
			
			// Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
			// Hub::run() to send events to all registered device listeners.
			hub.addListener(&collector);
			bool firstRun = true;
			// Finally we enter our main loop.
	//		while (1) {
				if (firstRun){
	//				std::cout << "Setting origin. Hold Still" << std::endl;
					hub.run(2000);
					collector.setOrigin();
					myo->vibrate(myo::Myo::vibrationMedium);
	//				std::cout << "Setting A. Hold Still" << std::endl;
					hub.run(2000);
					collector.setNoteOrientations('A');
					myo->vibrate(myo::Myo::vibrationMedium);
	//				std::cout << "Setting B. Hold Still" << std::endl;
					hub.run(2000);
					collector.setNoteOrientations('B');
					myo->vibrate(myo::Myo::vibrationMedium);
	//				std::cout << "Setting C. Hold Still" << std::endl;
					hub.run(2000);
					collector.setNoteOrientations('C');
					myo->vibrate(myo::Myo::vibrationMedium);
	//				std::cout << "Setting D. Hold Still" << std::endl;
					hub.run(2000);
					collector.setNoteOrientations('D');
					myo->vibrate(myo::Myo::vibrationMedium);
	//				std::cout << "Setting E. Hold Still" << std::endl;
					hub.run(2000);
					collector.setNoteOrientations('E');
					myo->vibrate(myo::Myo::vibrationMedium);
	//				std::cout << "Setting F. Hold Still" << std::endl;
					hub.run(2000);
					collector.setNoteOrientations('F');
					myo->vibrate(myo::Myo::vibrationMedium);
	//				std::cout << "Setting G. Hold Still" << std::endl;
					hub.run(2000);
					collector.setNoteOrientations('G');
					myo->vibrate(myo::Myo::vibrationMedium);
					firstRun = false;
				}
				// In each iteration of our main loop, we run the Myo event loop for a set number of milliseconds.
				// In this case, we wish to update our display 20 times a second, so we run for 1000/20 milliseconds.			
	//			hub.run(1000 / 20);
				// After processing events, we call the print() member function we defined above to print out the values we've
				// obtained from any events that have occurred.
	//			collector.print();
	//		}

			// If a standard exception occurred, we print out its message and exit.
		}
		catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << std::endl;
			std::cerr << "Press enter to continue.";
			std::cin.ignore();
			return 1;
		}
	}

	void getMyoAudio()
	{
	/*	myo::Hub hub("com.tamuhack.djmyo");
		hub.setLockingPolicy(myo::Hub::lockingPolicyNone);
		myo::Myo* myo = hub.waitForMyo(10000);
		hub.addListener(&collector);
		myo->vibrate(myo::Myo::vibrationMedium);
		*/
	}

private:
    //==============================================================================
	vector<float> phase;// = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	vector<float> phaseDelta;// = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	vector<float> frequency;// = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	vector<float> amplitude;// = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	int last_seconds = 0;
	int count = 0;

	vector<float> xcoor;
	vector<float> ycoor;

    double sampleRate;
    int expectedSamplesPerBlock;
    Point<float> lastMousePosition;

	DataCollector collector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


Component* createMainContentComponent() { return new MainContentComponent(); };

#endif  // MAINCOMPONENT_H_INCLUDED
