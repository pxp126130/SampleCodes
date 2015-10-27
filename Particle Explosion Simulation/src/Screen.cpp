/*
 * Screen.cpp
 *
 *  Created on: Oct 27, 2015
 *      Author: Parth
 */

#include "Screen.h"

namespace parthppparikh {

// Constructor to initialize the instance variables of class Screen.
Screen::Screen() :
		m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer1(NULL), m_buffer2(NULL) {
}

// Initialization function for the Window.
bool Screen::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}

	// Create Window Function from the SDL library.
	m_window = SDL_CreateWindow(
			"Particle Explosion Simulation",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN
			);

	// Call SDL_Quit if Window is not created.
	if (m_window == NULL) {
		SDL_Quit();
		return false;
	}

	// initialize renderer object.
	m_renderer = SDL_CreateRenderer(
			m_window,
			-1,
			SDL_RENDERER_PRESENTVSYNC
			);

	// initialize texture object.
	m_texture = SDL_CreateTexture(
			m_renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STATIC,
			SCREEN_WIDTH, SCREEN_HEIGHT
			);

	if (m_renderer == NULL) {
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	if (m_texture == NULL) {
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	// define Primary Buffer.
	m_buffer1 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
	// define Secondary Buffer -- to be used for blur effect.
	m_buffer2 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];

	// set buffer memories to create black window.
	memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
	memset(m_buffer2, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

	return true;
}

// Check if window is closed.
bool Screen::processEvents() {

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}

// Set inidividual pixel values.
void Screen::setPixel(int r, int c, Uint8 red, Uint8 green, Uint8 blue) {

	if (r < 0 || r >= SCREEN_HEIGHT || c < 0 || c >= SCREEN_WIDTH) {
		return;
	}
	Uint32 color = 0;

	color += red;
	color <<= 8;
	color += green;
	color <<= 8;
	color += blue;
	color <<= 8;
	color += 0xFF;

	// Map from row,column pair to 1-D buffer matrix.
	m_buffer1[(r * SCREEN_WIDTH) + c] = color;
}

// Update Window using SDL library functions.
void Screen::update() {
	SDL_UpdateTexture(m_texture, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
	SDL_RenderPresent(m_renderer);
}

// Blur existing pixels on Screen
void Screen::boxBlur() {
	Uint32 *temp = m_buffer1;
	m_buffer1 = m_buffer2;
	m_buffer2 = temp;

	for (int r = 0; r < SCREEN_HEIGHT; r++) {
		for (int c = 0; c < SCREEN_WIDTH; c++) {

			int redTotal = 0;
			int greenTotal = 0;
			int blueTotal = 0;

			for (int row = -1; row <= 1; row++) {
				for (int col = -1; col <= 1; col++) {
					int currentr = r + row;
					int currentc = c + col;
					if (currentr >= 0 && currentr < SCREEN_HEIGHT && currentc >= 0 && currentc < SCREEN_WIDTH) {

						Uint32 color = m_buffer2[currentr * SCREEN_WIDTH + currentc];

						Uint8 red = color >> 24;
						Uint8 green = color >> 16;
						Uint8 blue = color >> 8;

						redTotal += red;
						greenTotal += green;
						blueTotal += blue;
					}
				}
			}

			Uint8 red = redTotal/9;
			Uint8 green = greenTotal/9;
			Uint8 blue = blueTotal/9;
			setPixel(r, c, red, green, blue);
		}
	}
}

// Free Memory and close the window.
void Screen::close() {
	delete[] m_buffer1;
	delete[] m_buffer2;
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyTexture(m_texture);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

} /* namespace parthppparikh */
