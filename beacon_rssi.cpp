#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>


using namespace std;

vector<float> getRssiList(const char* filePath){
	float rssis[8];
	string line;
	int index = 0;
	vector<float> rssiList;
	ifstream myfile;
	myfile.open(filePath);
	if(myfile.is_open()){
		while(getline(myfile, line)){
			rssis[index] = atof(line.c_str());
			++index;
		}
		myfile.close();
		rssiList.assign(rssis, rssis + index);
	}
	else{
		ofstream newfile;
		newfile.open(filePath);
		if(newfile.is_open()){
			myfile.close();
		}else cout << "Unable to open file during making!";
	}
	return rssiList;
}

void writeRssiList(const char* filePath, vector<float> rssiList){
	ofstream myfile;
	myfile.open(filePath);
	if(myfile.is_open()){
		for(vector<float>::iterator it = rssiList.begin(); it != rssiList.end(); ++it){
			myfile << *it << '\n';
		}
		myfile.close();
	}else cout << "Unable to open file!";
}
float average(vector<float> list){
	if(list.size() > 0){
		float sum;
		for(vector<float>::iterator it = list.begin(); it != list.end(); ++it){
			sum += *it;
		}
		return sum/list.size();
	}else return 0;
}
void printList(vector<float> list){
	if(list.size() > 0){
		float sum;
		for(vector<float>::iterator it = list.begin(); it != list.end(); ++it){
			cout << *it <<'\n';
		}
	}
}


const char* AVG_FILE_PATH = "/var/ramdrive/beacon_avg_data.data";
const int MAX_QUEUE_SIZE = 8;
const float MAX_RSSI = 218.0;
const float MIN_RSSI = 162.0;
const float THRESHOLD_LOW = 28.0;
const float THRESHOLD_HIGH = 40.0;
const bool DEBUG = false;

int main(int argc, char** argv){
	if(argc <= 1){
		cout << "Argument Needed!" << argc;
		return 2;
	}
	vector<float> rssiList = getRssiList(AVG_FILE_PATH);
	string hex = argv[1];
	int curr_rssi = 0;
	sscanf(hex.c_str(), "%x", &curr_rssi);
	rssiList.push_back(curr_rssi);
	if(rssiList.size() > MAX_QUEUE_SIZE){
		rssiList.erase(rssiList.begin());
	}
	if(DEBUG) printList(rssiList);
	float avg_rssi  = average(rssiList);
	float rssi_perc = (avg_rssi - MIN_RSSI)/(MAX_RSSI - MIN_RSSI) * 100.0;
	if(DEBUG) cout << avg_rssi << '\n' << rssi_perc;
	writeRssiList(AVG_FILE_PATH, rssiList);
	if(rssi_perc < THRESHOLD_LOW) return 255;
	else if (rssi_perc > THRESHOLD_HIGH) return 1;
	else return 0;
}
