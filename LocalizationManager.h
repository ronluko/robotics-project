/*
 * LocalizationManager.h
 *
 *  Created on: Jan 11, 2017
 *      Author: user
 */

#ifndef LOCALIZATIONMANAGER_H_
#define LOCALIZATIONMANAGER_H_

#include "Particle.h"
#include <vector>
#include <HamsterAPIClientCPP/Hamster.h>

using namespace std;
using namespace HamsterAPI;

class LocalizationManager {
private:
	vector<Particle *> particles;
	const OccupancyGrid &ogrid;
	Hamster *hamster;
	void getRandomLocation(Particle *particle);
	double computeBelief(Particle *particle, LidarScan scan);

public:
	LocalizationManager(const OccupancyGrid &ogrid, Hamster *hamster);
	void initParticles();
	void updateParticles(double deltaX, double deltaY, double deltaYaw,LidarScan scan);
	void printParticles() const;
	vector<Particle *> getParticles() const;
	void SortParticles();
	void resampleParticles();
	void GenerateNearParticle(Particle *targetParticle, Particle *sourceParticle);
    void GenerateNearTopParticle(Particle *targetParticle);
	virtual ~LocalizationManager();
};

#endif /* LOCALIZATIONMANAGER_H_ */
