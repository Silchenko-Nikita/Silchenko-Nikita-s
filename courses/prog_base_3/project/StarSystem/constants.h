#pragma once

// define or not define? That is the question

# define CONST_PI 3.14 /*for constexpr*/

namespace Constants {
	const double PI = 3.14;
	const double G = 6.67e-11;

	const double metersPerPixel = 1.0e9; // abstract pixel, not necessary screen

	const double starsDisplayFactor = 10.0;
	const double planetsDisplayFactor = 150.0;
	const double sputniksDisplayFactor = 150.0;

	const int deltaTime = 10; // for Timer func, millisecs
	const double timeAccel = 5.0e6;
}