#include "Mode_Fire.h"

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
////
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation,
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on leds->numPixels(); it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking.
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
Mode_Fire::Mode_Fire(ILEDProvider* leds) : ModeBase(leds)
{
}

static inline int qsub8(int i, int j)
{
	return floor(i - j);
}


void Mode_Fire::NextState()
{
	// Array of temperature readings at each simulation cell
	//uint8_t heat[leds->numPixels()];
	//
	//// Step 1.  Cool down every cell a little
	//for (int i = 0; i < leds->numPixels(); i++) {
	//	heat[i] = qsub8(heat[i], random8(0, ((cooling * 2) / leds->numPixels()) + 1));
	//}

	//// Step 2.  Heat from each cell drifts 'up' and diffuses a little
	//for (int k = leds->numPixels() - 1; k >= 2; k--) {
	//	heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 3]) / 3;
	//}

	//// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
	//if (random8() < probabilitySparking) {
	//	int y = random8(4);
	//	heat[y] = qadd8(heat[y], random8(200, 255));
	//}

	//// Step 4.  Map from heat cells to LED colors
	//for (int j = 0; j < leds->numPixels(); j++) {
	//	CRGB color = HeatColor(heat[j]);
	//	int pixelnumber;
	//	if (reverseDirection) {
	//		pixelnumber = (leds->numPixels() - 1) - j;
	//	}
	//	else {
	//		pixelnumber = j;
	//	}
	//	leds[pixelnumber] = color;
	//}
}

String Mode_Fire::ID = "fire";

String Mode_Fire::GetID()
{
	return ID;
}

std::vector<String> Mode_Fire::ParameterNames()
{
	std::vector<String> names;
	names.push_back("spark");
	names.push_back("cool");
	auto baseNames = ModeBase::ParameterNames();
	for (size_t i = 0; i < baseNames.size(); i++)
	{
		names.push_back(baseNames.at(i));
	}
	return names;
}

String Mode_Fire::HandleProperty(String Name, String Value)
{
	if (Name == "spark")
	{
		if (!Value.isEmpty())
		{
			SetinBoundsAndReport(&probabilitySparking, "spark", Value, 0, 255);
		}
		return "spark=" + String(probabilitySparking) + "&";
	}
	else if (Name == "cool")
	{
		if (!Value.isEmpty())
		{
			SetinBoundsAndReport(&cooling, "cool", Value, 0, 255);
		}
		return "cool=" + String(cooling) + "&";
	}
	return ModeBase::HandleProperty(Name, Value);
}