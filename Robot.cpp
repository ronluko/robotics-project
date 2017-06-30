/*
 * Robot.cpp
 *
 *  Created on: Jan 18, 2017
 *      Author: user
 */

#include "Robot.h"

Robot::Robot(Hamster *hamster) :hamster(hamster), prevX(0), prevY(0), prevYaw(0), currX(0), currY(0), currYaw(0) {
	// TODO Auto-generated constructor stub
	updatePose();
}

double Robot::getDeltaX() const {
	return currX - prevX;
}

double Robot::getDeltaY() const {
	return currY - prevY;
}

double Robot::getDeltaYaw() const {
	return currYaw - prevYaw;
}

void Robot::updatePose() {
	Pose pose = hamster->getPose();

	prevX = currX;
	prevY = currY;
	prevYaw = currYaw;

	currX = pose.getX();
	currY = pose.getY();
	currYaw = pose.getHeading();
}

Robot::~Robot() {
	// TODO Auto-generated destructor stub
}

