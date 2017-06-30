/*
 * Map.cpp
 *
 *  Created on: Mar 6, 2017
 *      Author: user
 */

#include "Map.h"

Map::Map(OccupancyGrid &ogrid) : ogrid(ogrid), mat(ogrid.getWidth(), ogrid.getHeight(), CV_8UC3, cv::Scalar(255, 255, 255)) {
	cv::namedWindow("Map");
}

void Map::initMap() {
	for (int i = 0; i < ogrid.getHeight(); i++) {
		for (int j = 0; j < ogrid.getWidth(); j++) {

			Cell c = ogrid.getCell(i, j);
			if (c == CELL_FREE) {
				mat.at<cv::Vec3b>(i, j)[0] = 255;
				mat.at<cv::Vec3b>(i, j)[1] = 255;
				mat.at<cv::Vec3b>(i, j)[2] = 255;
			}
			else if (c == CELL_OCCUPIED)
			{
				mat.at<cv::Vec3b>(i, j)[0] = 0;
				mat.at<cv::Vec3b>(i, j)[1] = 0;
				mat.at<cv::Vec3b>(i, j)[2] = 0;
			}
			else
			{
				mat.at<cv::Vec3b>(i, j)[0] = 150;
				mat.at<cv::Vec3b>(i, j)[1] = 150;
				mat.at<cv::Vec3b>(i, j)[2] = 150;

			}
		}
	}
}

void Map::drawParticles(vector<Particle *> particles) {
	initMap();

	for (int i = 0; i < particles.size(); i++) {
		Particle *p = particles[i];
		if (particles[i]->belief<0.7){
			mat.at<cv::Vec3b>(p->j, p->i)[0] = 0;
			mat.at<cv::Vec3b>(p->j, p->i)[1] = 0;
			mat.at<cv::Vec3b>(p->j, p->i)[2] = 255;
		}
		else{
			mat.at<cv::Vec3b>(p->j, p->i)[0] = 255;
			mat.at<cv::Vec3b>(p->j, p->i)[1] = 0;
			mat.at<cv::Vec3b>(p->j, p->i)[2] = 0;
		}
		if(i>=particles.size()-5) {
			mat.at<cv::Vec3b>(p->j, p->i)[0] = 0;
			mat.at<cv::Vec3b>(p->j, p->i)[1] = 255;
			mat.at<cv::Vec3b>(p->j, p->i)[2] = 0;
		}
	}
}

void Map::drawObs(vector<Particle *> particles, HamsterAPI::LidarScan scan){
//	LidarScan scan = hamster->getLidarScan();

//		int hits = 0;
//		int misses = 0;
for (int j = 0; j < particles.size(); j++) {
	Particle *particle = particles[j];
	for (int i = 0; i < scan.getScanSize(); i++) {
				double angle = scan.getScanAngleIncrement() * i * DEG2RAD;

				if (scan.getDistance(i) < scan.getMaxRange() - 0.001) {
					// Obstacle_Pos = Particle_Pos + Scan_Distance * cos (Heading + Scan Angle)
					float dist=scan.getDistance(i);


					double obsX = particle->x + scan.getDistance(i) * sin(angle + particle->yaw * DEG2RAD -180 * DEG2RAD);
					double obsY = particle->y + scan.getDistance(i) * cos(angle + particle->yaw * DEG2RAD -180 * DEG2RAD);

//					int pixelsX = obsX / ogrid.getResolution() + ogrid.getWidth() / 2;
//					int pixelsY = obsY / ogrid.getResolution() + ogrid.getHeight() / 2;

					int pixelsi =((double)ogrid.getWidth() / 2) + (obsX / ogrid.getResolution());
					int pixelsj = ((double)ogrid.getHeight() / 2) - (obsY/ ogrid.getResolution());

//					particle->x = (particle->j - (double)ogrid.getWidth() / 2) * ogrid.getResolution() ;
//					particle->y = ((double)ogrid.getHeight() / 2 - particle->i) * ogrid.getResolution();

//					cout<<"test"<<endl;

//					double x = obsX / ogrid.getResolution();
//					double y = obsY / ogrid.getResolution();

					mat.at<cv::Vec3b>(pixelsi,pixelsj)[0] = 238;
					mat.at<cv::Vec3b>(pixelsi,pixelsj)[1] = 0;
					mat.at<cv::Vec3b>(pixelsi,pixelsj)[2] = 0;
				}
			}
	}
}

void Map::showMap() {
	// Show the matrix on the window
	cv::imshow("Map", mat);

	// Delay for 1 millisecond to allow the window to process
	// incoming events
	cv::waitKey(1);
}

Map::~Map() {
	// TODO Auto-generated destructor stub
}

