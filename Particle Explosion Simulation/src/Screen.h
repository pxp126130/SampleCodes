/*
 * Screen.h
 *
 *  Created on: Oct 27, 2015
 *      Author: Parth
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include <SDL.h>
#include <cstring>

namespace parthppparikh {

class Screen {
public:
	// Constant values for Window size.
	const static int SCREEN_WIDTH = 800;
	const static int SCREEN_HEIGHT = 600;
private:
	// SDL library defined Private instances.
	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	SDL_Texture *m_texture;
	Uint32 *m_buffer1;
	Uint32 *m_buffer2;

public:
	Screen();
	bool init();
	bool processEvents();
	void setPixel(int r, int c, Uint8 red, Uint8 green, Uint8 blue);
	void update();
	void close();
	void boxBlur();
};

} /* namespace parthppparikh */

#endif /* SCREEN_H_ */
