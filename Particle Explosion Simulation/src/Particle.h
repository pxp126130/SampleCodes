/*
 * Particle.h
 *
 *  Created on: Oct 27, 2015
 *      Author: Parth
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <math.h>
#include <stdlib.h>

namespace parthppparikh {

class Particle {
public:
	// row and column positions ( row == y-axis; column == x-axis)
	double m_r;
	double m_c;
private:
	// direction and speed of particle
	double m_speed;
	double m_direction;
public:
	// Constructor
	Particle();
	// Update Method
	void update(int interval);
private:
	// Initialization Method
	void init();
};

} /* namespace parthppparikh */

#endif /* PARTICLE_H_ */
