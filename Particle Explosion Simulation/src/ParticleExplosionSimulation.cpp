//============================================================================
// Name        : ParticleExplosionSimulation.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <SDL.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Screen.h"
#include "Swarm.h"

using namespace std;
using namespace parthppparikh;

int main(int argc, char *argv[]) {

	srand(time(NULL));
	Screen screen;
	Swarm swarm;

	if(screen.init() == false)
	{
		cout << "Error initializing SDL" << endl;
	}

	int halfHeight = screen.SCREEN_HEIGHT/2;
	int halfWidth = screen.SCREEN_WIDTH/2;

	/// GAME LOOP
	while (true) {
		int elapsed = SDL_GetTicks();

		swarm.update(elapsed);
		const Particle * const pParticles = swarm.getParticles();

		// Create Gradual Color
		unsigned char red = (unsigned char) ((1 + sin(elapsed * 0.0001)) * 128);
		unsigned char green = (unsigned char) ((1 + cos(elapsed * 0.0002)) * 128);
		unsigned char blue = (unsigned char) ((1 + sin(elapsed * 0.0003)) * 128);

		// Draw Particles
		for(int i = 0; i< Swarm::NPARTICLES;i++)
		{
			Particle particle = pParticles[i];
			int r = (particle.m_r * halfWidth) + halfHeight;
			int c = (particle.m_c + 1) * halfWidth;
			screen.setPixel(r,c,red,green,blue);
		}

		// Blur Screen
		screen.boxBlur();

		// Update Screen
		screen.update();

		// Window Close Event Capture and Notification
		if(!screen.processEvents()) {
			cout << "Window Closed by User." << endl;
			break;
		}
	}

	// Close Screen
	screen.close();
	return 0;
}
