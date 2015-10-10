#include "djmyo_imu.h"
double DataCollector::angle_between(myo::Vector3<float> v1, myo::Vector3<float> v2){
	return v1.angleTo(v2)*180.0/3.14159265;	//Return angle between v1 and v2 in degrees
}
char DataCollector::quat_to_note(myo::Quaternion<float> q){
	/*
	A- X .51	Y -.16	Z 0.85
	B- X .43	Y -.36	Z .81
	C- X .26	Y -.45	Z .69
	D- X .40	Y .12	Z .89
	E- X .30	Y .04	Z .76
	F- X .32	Y .39	Z .77
	G- X .27	Y .35	Z .60	
	*/
	myo::Vector3<float> v(q.x(), q.y(), q.z());
	if (angle_between(v, a_vec) <=30){
		
		return 'A';
	}
	else if (angle_between(v, b_vec) <= 30){
		return 'B';
	}
	else if (angle_between(v, c_vec) <= 30){
		return 'C';
	}
	else if (angle_between(v, d_vec) <= 30){
		return 'D';
	}
	else if (angle_between(v, e_vec) <= 30){
		return 'E';
	}
	else if (angle_between(v, f_vec) <= 30){
		return 'F';
	}
	else if (angle_between(v, g_vec) <= 30){
		return 'G';
	}
	else
		return 'X';
}
DataCollector::DataCollector(){
	data = new MyoData;
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
	using std::atan2;
	using std::asin;
	using std::sqrt;
	using std::max;
	using std::min;
	
	data->abs_orient = quat;
	data->ref_orient = quat * origin;
	
}
void DataCollector::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose){
	data->currentPose = pose;	
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
void DataCollector::print(){
	printf("\r");
	// Print out the orientation. Orientation data is always available, even if no arm is currently recognized.
	//std::cout << '[' << data->roll << ',' << data->pitch << ',' << data->yaw << ']';
	//printf("[W:%4f, X:%4f, Y:%4f, Z:%4f]\t\t", data->ref_orient.w(), data->ref_orient.x(), data->ref_orient.y(), data->ref_orient.z());
	printf("Note: %c", quat_to_note(data->ref_orient));
	//std::cout << '[' << data->orient.w() << ',' << data->orient.x() << ',' << data->orient.y() << ',' << data->orient.z() << ']';
	std::cout << std::flush;
}