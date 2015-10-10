#include "djmyo_imu.h"


/*
Seven notes : A B C D E F G
Two Accidentals: Sharp Flat

Short vibrate across note boundaries
*/
int main(int argc, char** argv)
{
	// We catch any exceptions that might occur below -- see the catch statement for more details.
	try {

		// First, we create a Hub with our application identifier. Be sure not to use the com.example namespace when
		// publishing your application. The Hub provides access to one or more Myos.
		myo::Hub hub("com.tamuhack.djmyo");
		hub.setLockingPolicy(myo::Hub::lockingPolicyNone);
		std::cout << "Attempting to find a Myo..." << std::endl;

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
		std::cout << "Connected to a Myo armband!" << std::endl << std::endl;
		
		// Next we construct an instance of our DeviceListener, so that we can register it with the Hub.
		DataCollector collector;

		// Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
		// Hub::run() to send events to all registered device listeners.
		hub.addListener(&collector);
		bool firstRun = true;
		// Finally we enter our main loop.
		while (1) {
			if (firstRun){
				std::cout << "Setting origin. Hold Still" << std::endl;
				hub.run(2000);
				collector.setOrigin();
				myo->vibrate(myo::Myo::vibrationMedium);
				std::cout << "Setting A. Hold Still" << std::endl;
				hub.run(2000);
				collector.setNoteOrientations('A');
				myo->vibrate(myo::Myo::vibrationMedium);
				std::cout << "Setting B. Hold Still" << std::endl;
				hub.run(2000);
				collector.setNoteOrientations('B');
				myo->vibrate(myo::Myo::vibrationMedium);
				std::cout << "Setting C. Hold Still" << std::endl;
				hub.run(2000);
				collector.setNoteOrientations('C');
				myo->vibrate(myo::Myo::vibrationMedium);
				std::cout << "Setting D. Hold Still" << std::endl;
				hub.run(2000);
				collector.setNoteOrientations('D');
				myo->vibrate(myo::Myo::vibrationMedium);
				std::cout << "Setting E. Hold Still" << std::endl;
				hub.run(2000);
				collector.setNoteOrientations('E');
				myo->vibrate(myo::Myo::vibrationMedium);
				std::cout << "Setting F. Hold Still" << std::endl;
				hub.run(2000);
				collector.setNoteOrientations('F');
				myo->vibrate(myo::Myo::vibrationMedium);
				std::cout << "Setting G. Hold Still" << std::endl;
				hub.run(2000);
				collector.setNoteOrientations('G');
				myo->vibrate(myo::Myo::vibrationMedium);
				firstRun = false;
			}
			// In each iteration of our main loop, we run the Myo event loop for a set number of milliseconds.
			// In this case, we wish to update our display 20 times a second, so we run for 1000/20 milliseconds.			
			hub.run(1000/20);
			// After processing events, we call the print() member function we defined above to print out the values we've
			// obtained from any events that have occurred.
			collector.print();
		}

		// If a standard exception occurred, we print out its message and exit.
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::cerr << "Press enter to continue.";
		std::cin.ignore();
		return 1;
	}
}