#include "EEPROMHelper.h"

/// <summary>
/// Initializes the filesystem
/// </summary>
/// <param name="size">The size.</param>
void InitFileSystem()
{
	SERIALWRITELINE("InitFileSystem");

	LittleFSConfig cfg;
	cfg.setAutoFormat(true);
	if (!LittleFS.setConfig(cfg))
	{
		SERIALWRITELINE("failed to set FS config");
	}
	if (!LittleFS.begin())
	{
		SERIALWRITELINE("failed to init Little FS");
	}
}
///// <summary>
///// Initializes the eeprom with the given size
///// </summary>
///// <param name="size">The size.</param>
//void InitEEPROM(int size)
//{
//	EEPROM.begin(size);
//}
//
///// <summary>
///// Retrieves content from memory until a 0 occures and converts the bytes into a string.
///// </summary>
///// <param name="startadress">The startadress.</param>
///// <returns>The red string</returns>
//String ReadEEPROM(int startadress)
//{
//	auto redString = String();
//	auto maxAdress = EEPROM.length();
//	for (size_t i = 0; i < maxAdress; i++)
//	{
//		auto red = (char)EEPROM.read(startadress + i);
//		if (red != 0)
//		{
//			redString.concat(red);
//		}
//		else
//		{
//			break;
//		}
//	}
//	return redString;
//}
/// <summary>
/// Retrieves string from filepath.
/// </summary>
/// <param name="path">The full filepath starts with / and ends with an file.txt</param>
/// <returns>The red string</returns>
String ReadFile(String path)
{
	SERIALWRITE("Read ");
	SERIALWRITE(path);
	SERIALWRITE(" : ");

	File file = LittleFS.open(path, "r");
	if (!file) {
		SERIALWRITELINE("Failed to open file for reading");
		return "";
	}
	String filecontent = "";
	while (file.available()) 
	{
		filecontent = file.readString();
	}
	file.close();
	SERIALWRITELINE("Success");
	return filecontent;
}

///// <summary>
///// Writes the string to the eeprom and terminates the sequence with 0.
///// </summary>
///// <param name="startadress">The startadress.</param>
///// <param name="text">The text to be written.</param>
///// <param name="andAdress">A limit for storage usage. -1 for auto.</param>
///// <returns>true if all is ok, false othwerwise</returns>
//bool WriteEEPROM(int startadress, String text, int endAdress)
//{
//	int length = text.length();
//	if (endAdress != -1 && length > endAdress)
//	{
//		SERIALWRITE("new hostname is to long, cuttin last symbols: " + String(length - endAdress));
//		length = endAdress;
//	}
//	char* textbytes = new char[length];
//	text.toCharArray(textbytes, length + 1);
//	for (auto i = 0; i < length; i++)
//	{
//		EEPROM.write(startadress + i, textbytes[i]);
//	}
//	EEPROM.write(startadress + length, 0);
//	auto result = EEPROM.commit();
//	if (!result)
//	{
//		SERIALWRITELINE("ERROR! EEPROM commit failed");
//	}
//	return result;
//}
void listDir(const char* dirname) {
	Serial.printf("Listing directory: %s\n", dirname);

	Dir root = LittleFS.openDir(dirname);

	while (root.next()) {
		File file = root.openFile("r");
		Serial.print("  FILE: ");
		Serial.print(root.fileName());
		Serial.print("  SIZE: ");
		Serial.print(file.size());
		time_t cr = file.getCreationTime();
		time_t lw = file.getLastWrite();
		file.close();
		struct tm* tmstruct = localtime(&cr);
		Serial.printf("    CREATION: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
		tmstruct = localtime(&lw);
		Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
	}
}
/// <summary>
/// Writes the string to the eeprom and terminates the sequence with 0.
/// </summary>
/// <param name="path">The path to be written.</param>
/// <param name="text">The text to be written.</param>
/// <param name="andAdress">A limit for storage usage. -1 for auto.</param>
/// <returns>true if all is ok, false othwerwise</returns>
bool WriteFile(String path, String message)
{
	SERIALWRITE("Write ");
	SERIALWRITE(path);
	SERIALWRITE(" : ");

	File file = LittleFS.open(path, "w");
	if (!file) {
		SERIALWRITELINE("Failed to open file for writing");
		return false;
	}
	if (file.print(message)) {
		SERIALWRITELINE("File written");
		file.close();
		return true;
	}
	else {
		SERIALWRITELINE("Write failed");
		file.close();
		return false;
	}
}

///// <summary>
///// Writes 0 to the eeprom.
///// </summary>
///// <param name="startAdress">The start adress.</param>
///// <param name="length">The length to be cleared</param>
///// <returns>true if all is ok, false othwerwise</returns>
//bool ClearEEPROM(int startAdress, int endAdress)
//{
//	for (int i = startAdress; i < endAdress; i++)
//	{
//		EEPROM.write(i, 0);
//	}
//	auto result = EEPROM.commit();
//	if (!result)
//	{
//		SERIALWRITELINE("ERROR! EEPROM commit failed");
//	}
//	return result;
//}

/// <summary>
/// Writes 0 to the eeprom.
/// </summary>
/// <param name="startAdress">The start adress.</param>
/// <param name="length">The length to be cleared</param>
/// <returns>true if all is ok, false othwerwise</returns>
bool TruncateFile(String path)
{
	File f = LittleFS.open(path, "w");
	if (!f)
	{
		SERIALWRITELINE("file open for truncate failed");
		return false;
	}
	f.truncate(f.size()); //untestet
	f.close();
	return true;
}