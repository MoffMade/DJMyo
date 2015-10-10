#include "djmyo_imu.h"
#define ANGLE_THRESHOLD 20
double DataCollector::angle_between(myo::Vector3<float> v1, myo::Vector3<float> v2){
	return v1.angleTo(v2)*180.0/3.14159265;	//Return angle between v1 and v2 in degrees
}
char DataCollector::quat_to_note(myo::Quaternion<float> q){
	myo::Vector3<float> v(q.x(), q.y(), q.z());
	if (angle_between(v, a_vec) <= ANGLE_THRESHOLD){		
		return 'A';
	}
	else if (angle_between(v, b_vec) <= ANGLE_THRESHOLD){
		return 'B';
	}
	else if (angle_between(v, c_vec) <= ANGLE_THRESHOLD){
		return 'C';
	}
	else if (angle_between(v, d_vec) <= ANGLE_THRESHOLD){
		return 'D';
	}
	else if (angle_between(v, e_vec) <= ANGLE_THRESHOLD){
		return 'E';
	}
	else if (angle_between(v, f_vec) <= ANGLE_THRESHOLD){
		return 'F';
	}
	else if (angle_between(v, g_vec) <= ANGLE_THRESHOLD){
		return 'G';
	}
	else
		return 'X';
}
DataCollector::DataCollector(){
	data = new MyoData;
	currentNote = new char('X');
	noteToSet = 'A';
}
DataCollector::DataCollector(char* note){
	data = new MyoData;
	currentNote = note;
	noteToSet = 'A';
}
void DataCollector::setOrigin(){
	origin = data->abs_orient;
}
void DataCollector::setNoteOrientations(char note){
	myo::Vector3<float> q(data->ref_orient.x(), data->ref_orient.y(), data->ref_orient.z());
	switch (note){
	case 'A':
		a_vec = q;
		break;
	case 'B':
		b_vec = q;
		break;
	case 'C':
		c_vec = q;
		break;
	case 'D':
		d_vec = q;
		break;
	case 'E':
		e_vec = q;
		break;
	case 'F':
		f_vec = q;
		break;
	case 'G':
		g_vec = q;
		break;
	default:
		break;
	}
}
void DataCollector::onUnpair(myo::Myo*, uint64_t){
	delete data;
	data = new MyoData;
}
void DataCollector::onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat){
	data->abs_orient = quat;
	data->ref_orient = quat * origin;
	*currentNote = quat_to_note(data->ref_orient);
}
void DataCollector::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose){
	data->currentPose = pose;	
	if (pose == myo::Pose::waveOut && noteToSet <= 'G'){
		setNoteOrientations(noteToSet);
		myo->vibrate(myo::Myo::vibrationShort);
		noteToSet++;
	}
}
void DataCollector::onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState){
	data->onArm = true;
	data->whichArm = arm;
}
void DataCollector::onArmUnsync(myo::Myo* myo, uint64_t timestamp){
	data->onArm = true;
}
void DataCollector::onUnlock(myo::Myo* myo, uint64_t timestamp){
	data->isUnlocked = true;
}
void DataCollector::onLock(myo::Myo* myo, uint64_t timestamp){
	data->isUnlocked = false;
}
std::string DataCollector::print(){
	return "";
}