#include "EEPROMHelper.h"

/// <summary>
/// Initializes the eeprom with the given size
/// </summary>
/// <param name="size">The size.</param>
void InitEEPROM(int size)
{
	EEPROM.begin(size);
}

/// <summary>
/// Retrieves content from memory until a 0 occures and converts the bytes into a string.
/// </summary>
/// <param name="startadress">The startadress.</param>
/// <returns>The red string</returns>
String ReadEEPROM(int startadress)
{
	auto redString = String();
	auto maxAdress = EEPROM.length();
	for (size_t i = 0; i < maxAdress; i++)
	{
		auto red = (char)EEPROM.read(startadress + i);
		if (red != 0)
		{
			redString.concat(red);
		}
		else
		{
			break;
		}
	}
	return redString;
}

/// <summary>
/// Writes the string to the eeprom and terminates the sequence with 0.
/// </summary>
/// <param name="startadress">The startadress.</param>
/// <param name="text">The text to be written.</param>
/// <param name="andAdress">A limit for storage usage. -1 for auto.</param>
/// <returns>true if all is ok, false othwerwise</returns>
bool WriteEEPROM(int startadress, String text, int endAdress)
{
	int length = text.length();
	if (endAdress != -1 && length > endAdress)
	{
		SERIALWRITE("new hostname is to long, cuttin last symbols: " + String(length - endAdress));
		length = endAdress;
	}
	char* textbytes = new char[length];
	text.toCharArray(textbytes, length + 1);
	for (auto i = 0; i < length; i++)
	{
		EEPROM.write(startadress + i, textbytes[i]);
	}
	EEPROM.write(startadress + length, 0);
	auto result = EEPROM.commit();
	if (!result)
	{
		SERIALWRITELINE("ERROR! EEPROM commit failed");
	}
	return result;
}

/// <summary>
/// Writes 0 to the eeprom.
/// </summary>
/// <param name="startAdress">The start adress.</param>
/// <param name="length">The length to be cleared</param>
/// <returns>true if all is ok, false othwerwise</returns>
bool ClearEEPROM(int startAdress, int endAdress)
{
	for (int i = startAdress; i < endAdress; i++)
	{
		EEPROM.write(i, 0);
	}
	auto result = EEPROM.commit();
	if (!result)
	{
		SERIALWRITELINE("ERROR! EEPROM commit failed");
	}
	return result;
}