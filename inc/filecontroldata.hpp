/**
 * @file filecontroldata.hpp
 * @brief include for FileControlData class
 */

#pragma once

#include <cstdint>

class FileControlData
{
public:
	FileControlData(char _id, uint8_t _no_damage_progress, uint16_t _enemies_killed);
	~FileControlData();
	//TODO add interface for changing updating data
private:
	char* fileGrouptext = " FileX  [ 7-65535 ] ";
	static const char* levelGroupText;
	static const char* levelCastleText;
	bool levelCastleState = false;
	static const char* levelMoonText;
	bool levelMoonState = false;
	static const char* levelSnowText;
	bool levelSnowState = false;
	static const char* levelDesertName;
	bool levelDesertState = false;
	static const char* levelForestName;
	bool levelForestState = false;
	static const char* levelBeachText;
	bool levelBeachState = false;
	static const char* introName;
	bool introState = false;
	static const char* endingName;
	bool endingState = false;
	static const char* gemsText;
	static const char* livesText;
	char* gemsValue = "99";
	char* livesValue = "9999";
	static const char* gems0Text;
	static const char* gems9Text;
	static const char* lives5Text;
	static const char* lives9Text;
	static const char* maxText;
	static const char* deleteText;
	static const char* saveText;
	static const char* reloadText;
};
