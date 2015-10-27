/*
 * Particle.cpp
 *
 *  Created on: Oct 27, 2015
 *      Author: Parth
 */

#include "Particle.h"

namespace parthppparikh {

// Constructor to initialize the instance variables of class Particle.
Particle::Particle() : m_r(0), m_c(0) {
	init();
}

// Function to initialize/reset instance variables of class Particle.
void Particle::init() {

	m_r = 0;
	m_c = 0;
	m_direction = (2 * M_PI * rand()) / RAND_MAX;
	m_speed = (0.01 * rand()) / RAND_MAX;
	m_speed *= m_speed;
}

// Update Particle direction, speed and position based on last update time.
void Particle::update(int interval) {

	m_direction += interval * 0.0002;

	double r_speed = m_speed * sin(m_direction + 0.01);
	double c_speed = m_speed * cos(m_direction);

	m_r += r_speed * interval;
	m_c += c_speed * interval;

	// Re-initalize if Particle position exceeds bounds.
	if (m_r < -1 || m_r > 1 || m_c < -1 || m_c > 1) {
		init();
	}

	// Re-initalize random particles.
	if (rand() < RAND_MAX / 200) {
		init();
	}
}

} /* namespace parthppparikh */
