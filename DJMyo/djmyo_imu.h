#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <algorithm>
#include "myo\myo.hpp"

/*
Quaternion Positioning


*/
typedef struct MyoData{	
	bool onArm;
	myo::Arm whichArm;
	bool isUnlocked;
	myo::Quaternion<float> abs_orient;
	myo::Quaternion<float> ref_orient;
	myo::Pose currentPose;
	MyoData() : onArm(false), isUnlocked(false), abs_orient(), ref_orient(), currentPose(){}
} MyoData;

class DataCollector : public myo::DeviceListener{
	MyoData *data;
	myo::Quaternion<float> origin;
	myo::Vector3<float> a_vec;
	myo::Vector3<float> b_vec;
	myo::Vector3<float> c_vec;
	myo::Vector3<float> d_vec;
	myo::Vector3<float> e_vec;
	myo::Vector3<float> f_vec;
	myo::Vector3<float> g_vec;
	char noteToSet;
public:
	DataCollector();
	void setOrigin();
	void setNoteOrientations(char note);
	void onUnpair(myo::Myo*, uint64_t );
	void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat);
	void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose);
	void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState);
	void onArmUnsync(myo::Myo* myo, uint64_t timestamp);
	void onUnlock(myo::Myo* myo, uint64_t timestamp);
	void onLock(myo::Myo* myo, uint64_t timestamp);
	char quat_to_note(myo::Quaternion<float> q);
	double angle_between(myo::Vector3<float> v1, myo::Vector3<float> v2);
	void print();
};