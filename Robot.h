/*
 * Robot.h
 *
 *  Created on: Jan 18, 2017
 *      Author: user
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <HamsterAPIClientCPP/Hamster.h>
using namespace HamsterAPI;

class Robot {
private:
	double currX, currY, currYaw;
	double prevX, prevY, prevYaw;
	Hamster *hamster;

public:
	Robot(Hamster *hamster);
	double getDeltaX() const;
	double getDeltaY() const;
	double getDeltaYaw() const;
	void updatePose();
	virtual ~Robot();
};

#endif /* ROBOT_H_ */
