//void loop() {
//	// Fill along the length of the strip in various colors...
//	colorWipe(leds.Color(255, 0, 0), 50); // Red
//	colorWipe(leds.Color(0, 255, 0), 50); // Green
//	colorWipe(leds.Color(0, 0, 255), 50); // Blue
//	colorWipe(leds.Color(255, 0, 100), 50); // Lila
//	colorWipe(leds.Color(0, 80, 255), 50); // Türkis
//	colorWipe(leds.Color(255, 255, 0), 50); //Gelb
//
//	colorTyke(leds.Color(0, 0, 255), leds.Color(255, 0, 0), 10, 25);
//	//colorTyke(strip.Color(255, 255, 0), strip.Color(0, 255, 0), 10, 25);
//
//	colorKnightRider(leds.Color(0, 0, 255), 50, 10, 10);
//
//	// Do a theater marquee effect in various colors...
//	//  theaterChase(strip.Color(127, 0, 30), 50); // White, half brightness
//	//  theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
//	//  theaterChase(strip.Color(  0,   0, 127), 50); // Blue, half brightness
//
//	// 10 Sekunden warten
//	// 12 Iteration (1 Itaeration ~ 25 Sekunden => 12*25=300 Sek = 5 Minuten)
//	rainbow(10, 12);             // Flowing rainbow cycle along the whole strip
//	//  theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
//}
//
//void colorTyke(uint32_t color_left, uint32_t color_right, int iteration, int wait) {
//	leds.clear();
//	for (int i = 0; i < iteration; i++) { // For each pixel in strip...
//		for (int k = 0; k < leds.numPixels(); k++) { // For each pixel in strip...
//			leds.setPixelColor(k, color_right);         //  Set pixel's color (in RAM)
//			leds.setPixelColor(leds.numPixels() - k, color_left);        //  Set pixel's color (in RAM)
//			leds.show();                          //  Update strip to match
//			delay(wait);                           //  Pause for a moment
//		}
//	}
//}
//
//// Some functions of our own for creating animated effects -----------------
//
//// Fill strip pixels one after another with a color. Strip is NOT cleared
//// first; anything there will be covered pixel by pixel. Pass in color
//// (as a single 'packed' 32-bit value, which you can get by calling
//// strip.Color(red, green, blue) as shown in the loop() function above),
//// and a delay time (in milliseconds) between pixels.
//void colorWipe(uint32_t color, int wait) {
//	for (int i = 0; i < leds.numPixels(); i++) { // For each pixel in strip...
//		leds.setPixelColor(i, color);         //  Set pixel's color (in RAM)
//		leds.show();                          //  Update strip to match
//		delay(wait);                           //  Pause for a moment
//	}
//}
//
//// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
//// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
//// between frames.
//void theaterChase(uint32_t color, int wait) {
//	for (int a = 0; a < 100; a++) { // Repeat 10 times...
//		for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
//			leds.clear();         //   Set all pixels in RAM to 0 (off)
//			// 'c' counts up from 'b' to end of strip in steps of 3...
//			for (int c = b; c < leds.numPixels(); c += 3) {
//				leds.setPixelColor(c, color); // Set pixel 'c' to value 'color'
//			}
//			leds.show(); // Update strip with new contents
//			delay(wait);  // Pause for a moment
//		}
//	}
//}
//
//// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
//void rainbow(int wait, int count) {
//	// Hue of first pixel runs 5 complete loops through the color wheel.
//	// Color wheel has a range of 65536 but it's OK if we roll over, so
//	// just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
//	// means we'll make 5*65536/256 = 1280 passes through this outer loop:
//	for (long firstPixelHue = 0; firstPixelHue < count * 5 * 65536; firstPixelHue += 256) {
//		for (int i = 0; i < leds.numPixels(); i++) { // For each pixel in strip...
//		  // Offset pixel hue by an amount to make one full revolution of the
//		  // color wheel (range of 65536) along the length of the strip
//		  // (strip.numPixels() steps):
//			int pixelHue = firstPixelHue + (i * 65536L / leds.numPixels());
//			// strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
//			// optionally add saturation and value (brightness) (each 0 to 255).
//			// Here we're using just the single-argument hue variant. The result
//			// is passed through strip.gamma32() to provide 'truer' colors
//			// before assigning to each pixel:
//			leds.setPixelColor(i, leds.gamma32(leds.ColorHSV(pixelHue)));
//		}
//		leds.show(); // Update strip with new contents
//		delay(wait);  // Pause for a moment
//	}
//}
//
//// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
//void theaterChaseRainbow(int wait) {
//	int firstPixelHue = 0;     // First pixel starts at red (hue 0)
//	for (int a = 0; a < 30; a++) { // Repeat 30 times...
//		for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
//			leds.clear();         //   Set all pixels in RAM to 0 (off)
//			// 'c' counts up from 'b' to end of strip in increments of 3...
//			for (int c = b; c < leds.numPixels(); c += 3) {
//				// hue of pixel 'c' is offset by an amount to make one full
//				// revolution of the color wheel (range 65536) along the length
//				// of the strip (strip.numPixels() steps):
//				int      hue = firstPixelHue + c * 65536L / leds.numPixels();
//				uint32_t color = leds.gamma32(leds.ColorHSV(hue)); // hue -> RGB
//				leds.setPixelColor(c, color); // Set pixel 'c' to value 'color'
//			}
//			leds.show();                // Update strip with new contents
//			delay(wait);                 // Pause for a moment
//			firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
//		}
//	}
//}