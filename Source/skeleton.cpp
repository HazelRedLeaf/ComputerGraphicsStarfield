#include <iostream>
#include <glm/glm.hpp>
#include <SDL.h>
#include "SDLauxiliary.h"
#include "TestModel.h"

using namespace std;
using glm::vec3;
using glm::mat3;

/* ----------------------------------------------------------------------------*/
/* GLOBAL VARIABLES                                                            */
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
SDL_Surface* screen;
int t;
vector<vec3> stars(1000); // store location of all stars

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */
void Update();
void Draw();
void Interpolate(vec3 a, vec3 b, vector<vec3>& result);

int main(int argc, char* argv[])
{
	screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT);
	t = SDL_GetTicks();	// Set start value for timer.

	// create initial random positions for the stars
	for(int i = 0; i < 1000; i++)
	{
		// For x and y:
		//				float(rand()) / float(RAND_MAX) * (MAX - MIN) + MIN
		//				MAX = 1, MIN = -1
		stars[i].x = float(rand()) / float(RAND_MAX) * 2 - 1;
		stars[i].y = float(rand()) / float(RAND_MAX) * 2 - 1;
		stars[i].z = float(rand()) / float(RAND_MAX);
	}

		for(int i = 0; i < 1000; i++)
	{
		if (stars[i].x < -1 || stars[i].x > 1 ||
			stars[i].y < -1 || stars[i].y > 1 ||
			stars[i].z < 0 || stars[i].z > 1)
			cout << "( "
				 << stars[i].x << ", "
				 << stars[i].y << ", "
				 << stars[i].z << ")" << endl;
	}

	while(NoQuitMessageSDL())
	{
		Update();
		Draw();
	}

	SDL_SaveBMP(screen, "screenshot.bmp");

	return 0;
}

void Update()
{
	// Compute frame time:
	int t2 = SDL_GetTicks();
	float dt = float(t2-t);
	t = t2;
	//cout << "Render time: " << dt << " ms." << endl;
}

void Draw()
{
	if(SDL_MUSTLOCK(screen))
		SDL_LockSurface(screen);

	// initialise a black screen
	for(int y = 0; y < SCREEN_HEIGHT; ++y)
	{
		for(int x = 0; x < SCREEN_WIDTH; ++x)
		{
			vec3 color(0.0, 0.0, 0.0);
			PutPixelSDL(screen, x, y, color);
		}
	}

	for(int i = 0; i < 1000; ++i)
	{
		// focal length
		float f = SCREEN_HEIGHT / 2;

		// 2D screen coordinates (u, v)
		float u = f * stars[i].x / stars[i].z + SCREEN_WIDTH/ 2;
		float v = f * stars[i].y / stars[i].z + SCREEN_HEIGHT / 2;

		// position stars
		vec3 color(1.0, 1.0, 1.0);
		PutPixelSDL(screen, u, v, color);
	}

	if(SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);

	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void Interpolate(vec3 a, vec3 b, vector<vec3>& result)
{
	for( unsigned int i=0; i < result.size(); i++)
	{
		result[i].x = a.x + i * (b.x - a.x) / (result.size() - 1);
		result[i].y = a.y + i * (b.y - a.y) / (result.size() - 1);
		result[i].z = a.z + i * (b.z - a.z) / (result.size() - 1);
	}
}
