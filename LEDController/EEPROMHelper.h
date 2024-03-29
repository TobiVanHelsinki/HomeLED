#pragma once
#include <ESP8266WebServer.h>
//#include <EEPROM.h>
#include <LittleFS.h>
#include "ProjectHeader.h"

///// <summary>
///// Test Initializes the eeprom with the given size
///// </summary>
///// <param name="size">The size.</param>
//void InitEEPROM(int size);
//
///// <summary>
///// Retrieves content from memory until a 0 occures and converts the bytes into a string.
///// </summary>
///// <param name="startadress">The startadress.</param>
///// <returns>The red string</returns>
//String ReadEEPROM(int startadress);

///// <summary>
///// Writes the string to the eeprom and terminates the sequence with 0.
///// </summary>
///// <param name="startadress">The startadress.</param>
///// <param name="text">The text to be written.</param>
///// <param name="andAdress">A limit for storage usage. -1 for auto.</param>
///// <returns>true if all is ok, false othwerwise</returns>
//bool WriteEEPROM(int startadress, String text, int andAdress);
//
///// <summary>
///// Writes 0 to the eeprom.
///// </summary>
///// <param name="startAdress">The start adress.</param>
///// <param name="length">The length to be cleared</param>
///// <returns>true if all is ok, false othwerwise</returns>
//bool ClearEEPROM(int startAdress, int length);
void listDir(const char* dirname);
/// <summary>
/// Initializes the filesystem
/// </summary>
/// <param name="size">The size.</param>
void InitFileSystem();

/// <summary>
/// Retrieves string from filepath.
/// </summary>
/// <param name="path">The full filepath starts with / and ends with an file.txt</param>
/// <returns>The red string</returns>
String ReadFile(String path);

/// <summary>
/// Writes the string to the eeprom and terminates the sequence with 0.
/// </summary>
/// <param name="path">The path to be written.</param>
/// <param name="text">The text to be written.</param>
/// <param name="andAdress">A limit for storage usage. -1 for auto.</param>
/// <returns>true if all is ok, false othwerwise</returns>
bool WriteFile(String path, String text);

/// <summary>
/// Writes 0 to the eeprom.
/// </summary>
/// <param name="startAdress">The start adress.</param>
/// <param name="length">The length to be cleared</param>
/// <returns>true if all is ok, false othwerwise</returns>
bool TruncateFile(String path);