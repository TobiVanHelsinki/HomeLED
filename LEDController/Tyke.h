#pragma once
// Konstanten
const int tasterPin = D5;
const int rotPin = D1;
const int gruenPin = D3;
const int blauPin = D2;

// Variablen
int lichtmodus = 0;
int tasterStatus = LOW;
int helligkeit = 15;
int fadeschritte = 1;

void setup()
{
	pinMode(tasterPin, INPUT);
	pinMode(rotPin, OUTPUT);
	pinMode(gruenPin, OUTPUT);
	pinMode(blauPin, OUTPUT);
}

void loop()
{
	//Abfrage ob Taster gedrückt ist
	tasterStatus = digitalRead(tasterPin);

	//Wenn Taster gedrückt ist...
	if (tasterStatus == HIGH)
	{
		lichtmodus++;
		delay(1000);
	}

	//+++++++++++++++++LEUCHTPROGRAMME++++++++++++++++++

	// Modus 0 = Licht aus
	if (lichtmodus == 0)
	{
		analogWrite(blauPin, 0);
		analogWrite(rotPin, 0);
		analogWrite(gruenPin, 0);
	}

	// Modus 1 Blau
	else if (lichtmodus == 1)
	{
		analogWrite(blauPin, 255);
		analogWrite(rotPin, 0);
		analogWrite(gruenPin, 0);
	}

	// Modus 2 Blau
	else if (lichtmodus == 2)
	{
		analogWrite(blauPin, 30);
		analogWrite(rotPin, 0);
		analogWrite(gruenPin, 0);
	}

	// Modus 3 Blau Pulsierend
	else if (lichtmodus == 3)
	{
		analogWrite(blauPin, helligkeit);
		analogWrite(rotPin, 0);
		analogWrite(gruenPin, 0);
		helligkeit = helligkeit + fadeschritte;
		delay(50);
		if (helligkeit == 15 || helligkeit == 255)
		{
			fadeschritte = -fadeschritte;
		}
	}

	// Modus 4 Gruen
	else if (lichtmodus == 4)
	{
		analogWrite(blauPin, 0);
		analogWrite(rotPin, 0);
		analogWrite(gruenPin, 255);
	}

	// Modus 5 Gruen
	else if (lichtmodus == 5)
	{
		analogWrite(blauPin, 0);
		analogWrite(rotPin, 0);
		analogWrite(gruenPin, 30);
	}

	// Modus 6 Gruen Pulsierend
	else if (lichtmodus == 6)
	{
		analogWrite(blauPin, 0);
		analogWrite(rotPin, 0);
		analogWrite(gruenPin, helligkeit);
		helligkeit = helligkeit + fadeschritte;
		delay(50);
		if (helligkeit == 15 || helligkeit == 255)
		{
			fadeschritte = -fadeschritte;
		}
	}

	// Modus 7 Rot
	else if (lichtmodus == 7)
	{
		analogWrite(blauPin, 0);
		analogWrite(rotPin, 255);
		analogWrite(gruenPin, 0);
	}

	// Modus 8 Rot
	else if (lichtmodus == 8)
	{
		analogWrite(blauPin, 0);
		analogWrite(rotPin, 30);
		analogWrite(gruenPin, 0);
	}

	// Modus 9 Rot Pulsierend
	else if (lichtmodus == 9)
	{
		analogWrite(blauPin, 0);
		analogWrite(rotPin, helligkeit);
		analogWrite(gruenPin, 0);
		helligkeit = helligkeit + fadeschritte;
		delay(50);
		if (helligkeit == 15 || helligkeit == 255)
		{
			fadeschritte = -fadeschritte;
		}
	}

	// Modus 10 Gelb
	else if (lichtmodus == 10)
	{
		analogWrite(blauPin, 0);
		analogWrite(rotPin, 100);
		analogWrite(gruenPin, 15);
	}

	// Modus 11 DeepPink3
	else if (lichtmodus == 11)
	{
		analogWrite(blauPin, 25);
		analogWrite(rotPin, 75);
		analogWrite(gruenPin, 0);
	}

	// Modus 12 Weiß
	else if (lichtmodus == 12)
	{
		analogWrite(blauPin, 255);
		analogWrite(rotPin, 255);
		analogWrite(gruenPin, 255);
	}

	// Anzahl der Leutchtmodi auf 13 begrenzen. (0 bis 12)
	else
	{
		lichtmodus = 0;
	}
}
