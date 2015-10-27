/*
 * Swarm.h
 *
 *  Created on: Oct 27, 2015
 *      Author: Parth
 */

#ifndef SWARM_H_
#define SWARM_H_

#include "Particle.h"
namespace parthppparikh {

class Swarm {
public:
	const static int NPARTICLES = 3000;
private:
	Particle * m_pParticles;
	int lastTime;
public:
	Swarm();
	virtual ~Swarm();
	const Particle * const getParticles() { return m_pParticles;}
	void update(int elapsed);
};

} /* namespace parthppparikh */

#endif /* SWARM_H_ */
