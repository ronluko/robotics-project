/*
 * RandomWalk.h
 *
 *  Created on: Mar 5, 2017
 *      Author: user
 */

#ifndef RANDOMWALK_H_
#define RANDOMWALK_H_

#include "Particle.h"
#include <vector>
#include <HamsterAPIClientCPP/Hamster.h>

using namespace std;
using namespace HamsterAPI;

class RandomWalk {
private:
	Hamster *hamster;
	void ObstacleAvoidance(int StartingAngle,int EndAngle,LidarScan scan);

public:
	RandomWalk(Hamster *hamster);
	void StartRandomWalk(int StartingAngle,int EndAngle,LidarScan scan);

	virtual ~RandomWalk();
};




#endif /* RANDOMWALK_H_ */
