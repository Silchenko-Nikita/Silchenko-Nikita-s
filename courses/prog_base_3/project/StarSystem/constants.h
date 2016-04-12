#pragma once

namespace Constants {
	const double PI = 3.14159;
	const double G = 6.67408e-11;

	const double metersPerPixel = 1.0e9; // abstract pixel, not necessary screen

	const double starsDiamDisplayFactor = 10.0; // change of diameter of star being displayed
	const double planetsDiamDisplayFactor = 150.0; // change of diameter of planet being displayed
	const double sputniksDiamDisplayFactor = 150.0; // change of diameter of sputnik being displayed
	const double sputniksDistDisplayFactor = (sputniksDiamDisplayFactor + planetsDiamDisplayFactor) * 0.035; // change of distance from sputnik to it's parent when displayed

	const int deltaTime = 10; // for Timer func, millisecs
	const double timeAccel = 5.0e6;
}