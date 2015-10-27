/*
 * Swarm.cpp
 *
 *  Created on: Oct 27, 2015
 *      Author: Parth
 */

#include "Swarm.h"

namespace parthppparikh {

// Constructor to initialize the instance variables of class Swarm.
Swarm::Swarm(): lastTime(0) {
	m_pParticles = new Particle[NPARTICLES];
}

// Deconstructor to free memory.
Swarm::~Swarm() {
	delete m_pParticles;
}

// Update each particle created in the Swarm
void Swarm::update(int elapsed) {

	int interval = elapsed - lastTime;
	for(int i = 0 ; i < NPARTICLES; i++) {
		m_pParticles[i].update(interval);
	}

	lastTime = elapsed;
}

} /* namespace parthppparikh */
