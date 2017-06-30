/*
 * RandomWalk.cpp
 *
 *  Created on: Mar 5, 2017
 *      Author: user
 */

#include "RandomWalk.h"
#include <iostream>

using namespace std;
const float speed=0.3;

RandomWalk::RandomWalk(Hamster *hamster) :hamster(hamster) {
}
void RandomWalk::StartRandomWalk(int StartingAngle,int EndAngle,LidarScan scan){
	this->ObstacleAvoidance(StartingAngle,EndAngle,scan);
}
void RandomWalk::ObstacleAvoidance(int StartingAngle,int EndAngle,LidarScan scan){
	HamsterAPI::LidarScan ld = scan;
	bool isObstacleInFront = false;
	bool isObstacleClose = false;
	for (int i = StartingAngle; i <= EndAngle; i++) {
		 if (ld.getDistance(i) < 0.4) {
			isObstacleClose = true;
			break;
			}
		}
	int minAngle=90;
	float minDistance = ld.getDistance(90);
    for (int i = StartingAngle; i <= EndAngle; i++) {
		 if (ld.getDistance(i) < 0.8) {
			 isObstacleInFront = true;
			 break;
			}

		}
	for (int i = 90; i <= 270; i++) {
	    float ab = ld.getDistance(i);
    	if (ld.getDistance(i)<minDistance){
		    minAngle=i;
		    minDistance=ld.getDistance(i);
		   }
	   }
	//cout << ld.getDistance(165)<<"sdsds  "<< ld.getDistance(195)<<"dsa  "<< minDistance << "  "<< minAngle<<endl;
	if (isObstacleClose) {
		hamster->sendSpeed(-speed, 0);
		//cout << "back"<<endl;
	    //cout << "Front: " << ld.getDistance(180) << endl;
		} else if (isObstacleInFront) {
				    if(minAngle < 180)
						{
							//cout <<(minAngle-90)/2 <<endl;
							hamster->sendSpeed(speed, 45);
							//cout << "left"<<endl;
						}else{
							//cout <<(minAngle-270)/2 <<endl;
							hamster->sendSpeed(speed,-45);
							//cout << "right"<<endl;
						}
						//cout << "Front: " << ld.getDistance(180) << endl;
				     	}
					 else
					   {
						hamster->sendSpeed(speed, 0);
						//cout << "straight"<<endl;
				   	   }
}



RandomWalk::~RandomWalk() {
	// TODO Auto-generated destructor stub
}
