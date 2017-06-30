/*
 * Map.h
 *
 *  Created on: Mar 6, 2017
 *      Author: user
 */

#ifndef MAP_H_
#define MAP_H_

#include <HamsterAPIClientCPP/Hamster.h>
#include "Particle.h"
#include <vector>
using namespace std;
using namespace HamsterAPI;

class Map {
private:
	cv::Mat mat;
	OccupancyGrid &ogrid;
public:
	Map(OccupancyGrid &ogrid);
	void initMap();
	void drawParticles(vector<Particle *> particles);
	void drawObs(vector<Particle *> particles, HamsterAPI::LidarScan scan);
	void showMap();
	virtual ~Map();
};

#endif /* MAP_H_ */
