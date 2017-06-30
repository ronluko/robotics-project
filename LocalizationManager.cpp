/*
 * LocalizationManager.cpp
 *
 *  Created on: Jan 11, 2017
 *      Author: user
 */

#include "LocalizationManager.h"
#include <iostream>
#include <math.h>

using namespace std;
#define PARTICLES_NUM 400

LocalizationManager::LocalizationManager(const OccupancyGrid &ogrid, Hamster *hamster) : ogrid(ogrid), hamster(hamster) {
	// TODO Auto-generated constructor stub
	cout << "width: " << ogrid.getWidth() << ", height: " << ogrid.getHeight() << endl;
}

void LocalizationManager::getRandomLocation(Particle *particle) {
	do {
		particle->j = rand() % ogrid.getWidth();
		particle->i = rand() % ogrid.getHeight();

	} while (ogrid.getCell(particle->j, particle->i) != CELL_FREE);

	particle->x = (particle->j - (double)ogrid.getWidth() / 2) * ogrid.getResolution() ;
	particle->y = ((double)ogrid.getHeight() / 2 - particle->i) * ogrid.getResolution();

	//remove this lines
//	int pixelsj =((double)ogrid.getWidth() / 2) + (particle->x / ogrid.getResolution());
//	int pixelsi = ((double)ogrid.getHeight() / 2) - (particle->y/ ogrid.getResolution());

//	cout << "x: " << particle->x << ", y: " << particle->y << ", i: " << particle->i << ", j: " << particle->j << endl;
	particle->yaw = rand() % 360;
}

void LocalizationManager::initParticles() {
	particles.resize(PARTICLES_NUM);

	for (int i = 0; i < particles.size(); i++) {
		particles[i] = new Particle();
		getRandomLocation(particles[i]);

	}
}

double LocalizationManager::computeBelief(Particle *particle, LidarScan scan) {
	//LidarScan scan = hamster->getLidarScan();

	int hits = 0;
	int misses = 0;

	for (int i = 0; i < scan.getScanSize(); i++) {
		double angle = scan.getScanAngleIncrement() * i * DEG2RAD;

		if (scan.getDistance(i) < scan.getMaxRange() - 0.001) {
			// Obstacle_Pos = Particle_Pos + Scan_Distance * cos (Heading + Scan Angle)
			double obsX = particle->x + scan.getDistance(i) * sin(angle + particle->yaw * DEG2RAD - 180 * DEG2RAD);
			double obsY = particle->y + scan.getDistance(i) * cos(angle + particle->yaw * DEG2RAD - 180 * DEG2RAD);
			//double obsX = particle->x + scan.getDistance(i) * cos(angle + particle->yaw * DEG2RAD + 135 * DEG2RAD);
			//double obsY = particle->y + scan.getDistance(i) * sin(angle + particle->yaw * DEG2RAD + 135 * DEG2RAD);

//			int pixelsX = obsX / ogrid.getResolution() + ogrid.getWidth() / 2;
//			int pixelsY = obsY / ogrid.getResolution() + ogrid.getHeight() / 2;

			int pixelsi =((double)ogrid.getWidth() / 2) + (obsX / ogrid.getResolution());
			int pixelsj = ((double)ogrid.getHeight() / 2) - (obsY/ ogrid.getResolution());

//			if ((ogrid.getCell(pixelsi-1, pixelsj-1) == CELL_OCCUPIED)||
//					(ogrid.getCell(pixelsi-1, pixelsj) == CELL_OCCUPIED)||
//					(ogrid.getCell(pixelsi-1, pixelsj+1) == CELL_OCCUPIED)||
//					(ogrid.getCell(pixelsi, pixelsj-1) == CELL_OCCUPIED)||
//					(ogrid.getCell(pixelsi, pixelsj) == CELL_OCCUPIED)||
//					(ogrid.getCell(pixelsi, pixelsj+1) == CELL_OCCUPIED)||
//					(ogrid.getCell(pixelsi+1, pixelsj-1) == CELL_OCCUPIED)||
//					(ogrid.getCell(pixelsi+1, pixelsj) == CELL_OCCUPIED)||
//					(ogrid.getCell(pixelsi+1, pixelsj+1) == CELL_OCCUPIED)) {
			if (ogrid.getCell(pixelsi, pixelsj) == CELL_OCCUPIED ){
				hits++;
			} else {
				misses++;
			}
			//cout << "hits : "<< hits <<" misses : "<< misses <<"belief : "<< (float)hits / (hits + misses)<<endl;
		}
	}
	return (float)hits / (hits + misses);
}

void LocalizationManager::updateParticles(double deltaX, double deltaY, double deltaYaw,LidarScan scan) {
	for (int i = 0; i < particles.size(); i++) {
		Particle *particle = particles[i];
		particle->x += deltaX * cos(particle->yaw * DEG2RAD);
		particle->y += deltaY * sin(particle->yaw * DEG2RAD);
		particle->yaw += deltaYaw;
		particle->i = (double)ogrid.getHeight() / 2 - particle->y / ogrid.getResolution() ;
		particle->j = particle->x / ogrid.getResolution() + ogrid.getWidth() / 2;

		if(ogrid.getCell(particle->j, particle->i)==CELL_UNKNOWN)
			getRandomLocation(particle);
		particle->belief = computeBelief(particle,scan);
	}
	//resampleParticles();
}

void LocalizationManager::resampleParticles() {
//	int count = 0;
//	for (int k = 0; k < particles.size(); k++) {
//		if (ogrid.getCell(particles[k]->j, particles[k]->i) != CELL_FREE) {
////			particles[k]->belief = 0;
//			count++;
//		}
//	}
	SortParticles();
	double sum;
	for (int i = 0; i < particles.size(); i++) {
				sum += particles[i]->belief;
			}
	double avg= sum/PARTICLES_NUM*1.6;
	int RemoveSize = particles.size() * 0.4;
//	cout<<"A "<<particles.size()*0.35*(1-avg)<<endl;
	int totalScore = 0;
	double BetterPartAVG=0;

		for (int i = RemoveSize; i < particles.size(); i++) {
			totalScore += (particles[i]->belief) * 1000000;
			BetterPartAVG += (particles[i]->belief);
		}

		//cout<<avg<<endl;

	//cout<<particles[PARTICLES_NUM - 1]->belief<<endl;
	//if (particles[PARTICLES_NUM - 1]->belief < 0.19) {
//		if(particles.size()*0.05*(1-avg)>4){
////	if (avg<0.1) {
//		initParticles();
//		return;
//	}
//	if (RemoveSize < count) {
//		RemoveSize = count;
	//}
//	for (int i = 0; i < particles.size()*0.1; i++) {
//	for (int i = 0; i < particles.size()*0.05*(1-avg); i++) {


		double f = MIN(RemoveSize*pow(1-avg,2),RemoveSize*0.7);
		BetterPartAVG=BetterPartAVG/(particles.size()-RemoveSize);
	//	double f = MIN(pow(particles.size() * 0.35,(1-avg)*2),RemoveSize*0.85);

		if(BetterPartAVG<0.2){
		for (int i = 0; i < f; i++) {
			if (particles[i]->belief<0.5)
			     getRandomLocation(particles[i]);
			else GenerateNearTopParticle(particles[i]);
	}
		}
		cout<<"AVG : "<<avg<<endl;
	cout<<"B "<<f<<endl;
	cout<<"RealPartAVG "<<BetterPartAVG<<" Best one "<< particles[PARTICLES_NUM-1]->belief<<endl;
	/* These will need to be populated at the outset */
	//int scores[particles.size() - RemoveSize];

	if(particles[PARTICLES_NUM-1]->belief>0.4){
	for (int j = f; j < RemoveSize; j++) {
		int r = rand() % totalScore;
		int t = 0;
		for (int i = RemoveSize; i < particles.size();++i) {
			t += (particles[i]->belief) * 1000000;
			if (r < t) {
				GenerateNearParticle(particles[j], particles[i]);
			}
		}

	}
	}


}
void LocalizationManager::GenerateNearTopParticle(Particle *targetParticle){
	do {
	targetParticle->j = (rand() % 5)+(particles[PARTICLES_NUM-1]->j-2);
	targetParticle->i = (rand() % 5)+(particles[PARTICLES_NUM-1]->i-2);

	} while (ogrid.getCell(targetParticle->j, targetParticle->i) != CELL_FREE);

	targetParticle->x = (targetParticle->j - (double)ogrid.getWidth() / 2) * ogrid.getResolution();
	targetParticle->y = ((double)ogrid.getHeight() / 2 - targetParticle->i) * ogrid.getResolution();

	//cout << "x: " << targetParticle->x << ", y: " << targetParticle->y << ", i: " << targetParticle->i << ", j: " << targetParticle->j << endl;
	targetParticle->yaw = particles[PARTICLES_NUM-1]->yaw + (rand() % 11)-5;

}
void LocalizationManager::GenerateNearParticle(Particle *targetParticle, Particle *sourceParticle){
do {
targetParticle->j = (rand() % 5)+(sourceParticle->j-2);
targetParticle->i = (rand() % 5)+(sourceParticle->i-2);

} while (ogrid.getCell(targetParticle->j, targetParticle->i) != CELL_FREE);

targetParticle->x = (targetParticle->j - (double)ogrid.getWidth() / 2) * ogrid.getResolution();
targetParticle->y = ((double)ogrid.getHeight() / 2 - targetParticle->i) * ogrid.getResolution();

//cout << "x: " << targetParticle->x << ", y: " << targetParticle->y << ", i: " << targetParticle->i << ", j: " << targetParticle->j << endl;
targetParticle->yaw = sourceParticle->yaw + (rand() % 31)-15;
}

void LocalizationManager::printParticles() const {
	for (int i = 0; i < particles.size(); i++) {
		Particle *particle = particles[i];
//		cout << "Particle " << i << ": " << particle->x << "," << particle->y << ", belief: " << particle->belief << endl;
	}
}

vector<Particle *> LocalizationManager::getParticles() const {
	return particles;
}

void LocalizationManager::SortParticles()
{
int pos_min,temp;
for (int i=0; i < particles.size(); i++)
{
    pos_min = i;
	for (int j=i+1; j < particles.size(); j++)
	{
	if (particles[j]->belief < particles[pos_min]->belief)
               pos_min=j;
	}
        if (pos_min != i)
        {
        	Particle *temp = particles[i];
        	particles[i] = particles[pos_min];
        	particles[pos_min] = temp;
        }
}
}

LocalizationManager::~LocalizationManager() {
	// TODO Auto-generated destructor stub
}

