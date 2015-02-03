// Dependencies
#include "ResponseGenerator.h"
#include "sharedmemory.h"
#include "fossa.h"
#include <sstream>
#include <vector>

// Constants
#define MAP_OBJECT_NAME "$pcars$"
#define HTTP_RESPONSE_503 "{\"status\": \"503 Service unavailable, is Project CARS running and is Shared Memory enabled?\"}"
#define HTTP_RESPONSE_409 "{\"status\": \"409 Conflict, are CREST and Project CARS both at the latest version?\"}"

ResponseGenerator::ResponseGenerator(){};

void sendServiceUnavailable(struct ns_connection *nc)    {
	// Send HTTP 503
	ns_printf(nc, "HTTP/1.1 503 Service unavailable\r\n"
		"Content-Type: application/json\r\n"
		"Cache-Control: no-cache\r\n"
		"Content-Length: %d\r\n\r\n%s",
		(int)strlen(HTTP_RESPONSE_503), HTTP_RESPONSE_503);
}

void sendConflict(struct ns_connection *nc)    {
	// Send HTTP 409
	ns_printf(nc, "HTTP/1.1 409 Conflict\r\n"
		"Content-Type: application/json\r\n"
		"Cache-Control: no-cache\r\n"
		"Content-Length: %d\r\n\r\n%s",
		(int)strlen(HTTP_RESPONSE_409), HTTP_RESPONSE_409);
}

void buildBuildInfo(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"buildinfo\":{";
	ss << "\"mVersion\":" << sharedData->mVersion << ",";
	ss << "\"mBuildVersionNumber\":" << sharedData->mBuildVersionNumber << "}";
}

void buildGameStates(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"gameStates\":{";
	ss << "\"mGameState\":" << sharedData->mGameState << ",";
	ss << "\"mSessionState\":" << sharedData->mSessionState << ",";
	ss << "\"mRaceState\":" << sharedData->mRaceState << "}";
}

void buildParticipant(std::stringstream& ss, const ParticipantInfo participantInfo)	{
	ss << "{\"mIsActive\":" << (participantInfo.mIsActive ? "true" : "false") << ",";
	ss << "\"mName\":\"" << participantInfo.mName << "\",";
	ss << "\"mWorldPosition\":[" << participantInfo.mWorldPosition[0] << "," << participantInfo.mWorldPosition[1] << "," << participantInfo.mWorldPosition[2] << "],";
	ss << "\"mCurrentLapDistance\":" << participantInfo.mCurrentLapDistance << ",";
	ss << "\"mRacePosition\":" << participantInfo.mRacePosition << ",";
	ss << "\"mLapsCompleted\":" << participantInfo.mLapsCompleted << ",";
	ss << "\"mCurrentLap\":" << participantInfo.mCurrentLap << ",";
	ss << "\"mCurrentSector\":" << participantInfo.mCurrentSector << "}";
}

void buildParticipants(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"participants\":{";
	ss << "\"mViewedParticipantIndex\":" << sharedData->mViewedParticipantIndex << ",";
	ss << "\"mNumParticipants\":" << sharedData->mNumParticipants;

	if (sharedData->mNumParticipants > -1)	{
		ss << ",";
		ss << "\"mParticipantInfo\":[";

		for (int i = 0; i < sharedData->mNumParticipants; i++)	{
			buildParticipant(ss, sharedData->mParticipantInfo[i]);
			if (i < (sharedData->mNumParticipants - 1))	{
				ss << ",";
			}
		}
		ss << "]";
	}
	ss << "}";
}

void buildUnfilteredInput(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"unfilteredInput\":{";
	ss << "\"mUnfilteredThrottle\":" << sharedData->mUnfilteredThrottle << ",";
	ss << "\"mUnfilteredBrake\":" << sharedData->mUnfilteredBrake << ",";
	ss << "\"mUnfilteredSteering\":" << sharedData->mUnfilteredSteering << ",";
	ss << "\"mUnfilteredClutch\":" << sharedData->mUnfilteredClutch << "}";
}

void buildVehicleInformation(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"vehicleInformation\":{";
	ss << "\"mCarName\":\"" << sharedData->mCarName << "\",";
	ss << "\"mCarClassName\":\""<< sharedData->mCarClassName << "\"}";
}

void buildEventInformation(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"eventInformation\":{";
	ss << "\"mLapsInEvent\":" << sharedData->mLapsInEvent << ",";
	ss << "\"mTrackLocation\":\"" << sharedData->mTrackLocation << "\",";
	ss << "\"mTrackVariation\":\"" << sharedData->mTrackVariation << "\",";
	ss << "\"mTrackLength\":" << sharedData->mTrackLength << "}";
}

void buildTimings(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"timings\":{";
	ss << "\"mLapInvalidated\":" << (sharedData->mLapInvalidated ? "true" : "false") << ",";
	ss << "\"mBestLapTime\":" << sharedData->mBestLapTime << ",";
	ss << "\"mLastLapTime\":" << sharedData->mLastLapTime << ",";
	ss << "\"mCurrentTime\":" << sharedData->mCurrentTime << ",";
	ss << "\"mSplitTimeAhead\":" << sharedData->mSplitTimeAhead << ",";
	ss << "\"mSplitTimeBehind\":" << sharedData->mSplitTimeBehind << ",";
	ss << "\"mSplitTime\":" << sharedData->mSplitTime << ",";
	ss << "\"mEventTimeRemaining\":" << sharedData->mEventTimeRemaining << ",";
	ss << "\"mPersonalFastestLapTime\":" << sharedData->mPersonalFastestLapTime << ",";
	ss << "\"mWorldFastestLapTime\":" << sharedData->mWorldFastestLapTime << ",";
	ss << "\"mCurrentSector1Time\":" << sharedData->mCurrentSector1Time << ",";
	ss << "\"mCurrentSector2Time\":" << sharedData->mCurrentSector2Time << ",";
	ss << "\"mCurrentSector3Time\":" << sharedData->mCurrentSector3Time << ",";
	ss << "\"mFastestSector1Time\":" << sharedData->mFastestSector1Time << ",";
	ss << "\"mFastestSector2Time\":" << sharedData->mFastestSector2Time << ",";
	ss << "\"mFastestSector3Time\":" << sharedData->mFastestSector3Time << ",";
	ss << "\"mPersonalFastestSector1Time\":" << sharedData->mPersonalFastestSector1Time << ",";
	ss << "\"mPersonalFastestSector2Time\":" << sharedData->mPersonalFastestSector2Time << ",";
	ss << "\"mPersonalFastestSector3Time\":" << sharedData->mPersonalFastestSector3Time << ",";
	ss << "\"mWorldFastestSector1Time\":" << sharedData->mWorldFastestSector1Time << ",";
	ss << "\"mWorldFastestSector2Time\":" << sharedData->mWorldFastestSector2Time << ",";
	ss << "\"mWorldFastestSector3Time\":" << sharedData->mWorldFastestSector3Time << "}";
}

void buildFlags(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"flags\":{";
	ss << "\"mHighestFlagColour\":" << sharedData->mHighestFlagColour << ",";
	ss << "\"mHighestFlagReason\":" << sharedData->mHighestFlagReason << "}";
}

void buildPitInfo(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"pitInfo\":{";
	ss << "\"mPitMode\":" << sharedData->mPitMode << ",";
	ss << "\"mPitSchedule\":" << sharedData->mPitSchedule << "}";
}

void buildCarState(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"carState\":{";
	ss << "\"mCarFlags\":" << sharedData->mCarFlags << ",";
	ss << "\"mOilTempCelsius\":" << sharedData->mOilTempCelsius << ",";
	ss << "\"mWaterTempCelsius\":" << sharedData->mWaterTempCelsius << ",";
	ss << "\"mWaterPressureKPa\":" << sharedData->mWaterPressureKPa << ",";
	ss << "\"mFuelPressureKPa\":" << sharedData->mFuelPressureKPa << ",";
	ss << "\"mFuelLevel\":" << sharedData->mFuelLevel << ",";
	ss << "\"mFuelCapacity\":" << sharedData->mFuelCapacity << ",";
	ss << "\"mSpeed\":" << sharedData->mSpeed << ",";
	ss << "\"mRpm\":" << sharedData->mRpm << ",";
	ss << "\"mMaxRPM\":" << sharedData->mMaxRPM << ",";
	ss << "\"mBrake\":" << sharedData->mBrake << ",";
	ss << "\"mThrottle\":" << sharedData->mThrottle << ",";
	ss << "\"mClutch\":" << sharedData->mClutch << ",";
	ss << "\"mSteering\":" << sharedData->mSteering << ",";
	ss << "\"mGear\":" << sharedData->mGear << ",";
	ss << "\"mNumGears\":" << sharedData->mNumGears << ",";
	ss << "\"mOdometerKM\":" << sharedData->mOdometerKM << ",";
	ss << "\"mAntiLockActive\":" << (sharedData->mAntiLockActive ? "true" : "false") << ",";
	ss << "\"mLastOpponentCollisionIndex\":" << sharedData->mLastOpponentCollisionIndex << ",";
	ss << "\"mLastOpponentCollisionMagnitude\":" << sharedData->mLastOpponentCollisionMagnitude << ",";
	ss << "\"mBoostActive\":" << (sharedData->mBoostActive ? "true" : "false") << ",";
	ss << "\"mBoostAmount\":" << sharedData->mBoostAmount << "}";
}

void buildMotionDeviceRelated(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"motionAndDeviceRelated\":{";
	ss << "\"mOrientation\":[" << sharedData->mOrientation[0] << "," << sharedData->mOrientation[1] << "," << sharedData->mOrientation[2] << "],";
	ss << "\"mLocalVelocity\":[" << sharedData->mLocalVelocity[0] << "," << sharedData->mLocalVelocity[1] << "," << sharedData->mLocalVelocity[2] << "],";
	ss << "\"mWorldVelocity\":[" << sharedData->mWorldVelocity[0] << "," << sharedData->mWorldVelocity[1] << "," << sharedData->mWorldVelocity[2] << "],";
	ss << "\"mAngularVelocity\":[" << sharedData->mAngularVelocity[0] << "," << sharedData->mAngularVelocity[1] << "," << sharedData->mAngularVelocity[2] << "],";
	ss << "\"mLocalAcceleration\":[" << sharedData->mLocalAcceleration[0] << "," << sharedData->mLocalAcceleration[1] << "," << sharedData->mLocalAcceleration[2] << "],";
	ss << "\"mWorldAcceleration\":[" << sharedData->mWorldAcceleration[0] << "," << sharedData->mWorldAcceleration[1] << "," << sharedData->mWorldAcceleration[2] << "],";
	ss << "\"mExtentsCentre\":[" << sharedData->mExtentsCentre[0] << "," << sharedData->mExtentsCentre[1] << "," << sharedData->mExtentsCentre[2] << "]}";
}

void buildWheelsTyres(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"wheelsAndTyres\":{";
	ss << "\"mTyreFlags\":[" << sharedData->mTyreFlags[0] << "," << sharedData->mTyreFlags[1] << "," << sharedData->mTyreFlags[2] << "," << sharedData->mTyreFlags[3] << "],";
	ss << "\"mTerrain\":[" << sharedData->mTerrain[0] << "," << sharedData->mTerrain[1] << "," << sharedData->mTerrain[2] << "," << sharedData->mTerrain[3] << "],";
	ss << "\"mTyreY\":[" << sharedData->mTyreY[0] << "," << sharedData->mTyreY[1] << "," << sharedData->mTyreY[2] << "," << sharedData->mTyreY[3] << "],";
	ss << "\"mTyreRPS\":[" << sharedData->mTyreRPS[0] << "," << sharedData->mTyreRPS[1] << "," << sharedData->mTyreRPS[2] << "," << sharedData->mTyreRPS[3] << "],";
	ss << "\"mTyreSlipSpeed\":[" << sharedData->mTyreSlipSpeed[0] << "," << sharedData->mTyreSlipSpeed[1] << "," << sharedData->mTyreSlipSpeed[2] << "," << sharedData->mTyreSlipSpeed[3] << "],";
	ss << "\"mTyreTemp\":[" << sharedData->mTyreTemp[0] << "," << sharedData->mTyreTemp[1] << "," << sharedData->mTyreTemp[2] << "," << sharedData->mTyreTemp[3] << "],";
	ss << "\"mTyreGrip\":[" << sharedData->mTyreGrip[0] << "," << sharedData->mTyreGrip[1] << "," << sharedData->mTyreGrip[2] << "," << sharedData->mTyreGrip[3] << "],";
	ss << "\"mTyreHeightAboveGround\":[" << sharedData->mTyreHeightAboveGround[0] << "," << sharedData->mTyreHeightAboveGround[1] << "," << sharedData->mTyreHeightAboveGround[2] << "," << sharedData->mTyreHeightAboveGround[3] << "],";
	ss << "\"mTyreLateralStiffness\":[" << sharedData->mTyreLateralStiffness[0] << "," << sharedData->mTyreLateralStiffness[1] << "," << sharedData->mTyreLateralStiffness[2] << "," << sharedData->mTyreLateralStiffness[3] << "],";
	ss << "\"mTyreWear\":[" << sharedData->mTyreWear[0] << "," << sharedData->mTyreWear[1] << "," << sharedData->mTyreWear[2] << "," << sharedData->mTyreWear[3] << "],";
	ss << "\"mBrakeDamage\":[" << sharedData->mBrakeDamage[0] << "," << sharedData->mBrakeDamage[1] << "," << sharedData->mBrakeDamage[2] << "," << sharedData->mBrakeDamage[3] << "],";
	ss << "\"mSuspensionDamage\":[" << sharedData->mSuspensionDamage[0] << "," << sharedData->mSuspensionDamage[1] << "," << sharedData->mSuspensionDamage[2] << "," << sharedData->mSuspensionDamage[3] << "],";
	ss << "\"mBrakeTempCelsius\":[" << sharedData->mBrakeTempCelsius[0] << "," << sharedData->mBrakeTempCelsius[1] << "," << sharedData->mBrakeTempCelsius[2] << "," << sharedData->mBrakeTempCelsius[3] << "],";
	ss << "\"mTyreTreadTemp\":[" << sharedData->mTyreTreadTemp[0] << "," << sharedData->mTyreTreadTemp[1] << "," << sharedData->mTyreTreadTemp[2] << "," << sharedData->mTyreTreadTemp[3] << "],";
	ss << "\"mTyreLayerTemp\":[" << sharedData->mTyreLayerTemp[0] << "," << sharedData->mTyreLayerTemp[1] << "," << sharedData->mTyreLayerTemp[2] << "," << sharedData->mTyreLayerTemp[3] << "],";
	ss << "\"mTyreCarcassTemp\":[" << sharedData->mTyreCarcassTemp[0] << "," << sharedData->mTyreCarcassTemp[1] << "," << sharedData->mTyreCarcassTemp[2] << "," << sharedData->mTyreCarcassTemp[3] << "],";
	ss << "\"mTyreRimTemp\":[" << sharedData->mTyreRimTemp[0] << "," << sharedData->mTyreRimTemp[1] << "," << sharedData->mTyreRimTemp[2] << "," << sharedData->mTyreRimTemp[3] << "],";
	ss << "\"mTyreInternalAirTemp\":[" << sharedData->mTyreInternalAirTemp[0] << "," << sharedData->mTyreInternalAirTemp[1] << "," << sharedData->mTyreInternalAirTemp[2] << "," << sharedData->mTyreInternalAirTemp[3] << "]}";
}

void buildCarDamage(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"carDamage\":{";
	ss << "\"mCrashState\":" << sharedData->mCrashState << ",";
	ss << "\"mAeroDamage\":" << sharedData->mAeroDamage << ",";
	ss << "\"mEngineDamage\":" << sharedData->mEngineDamage << "}";
}

void buildWeather(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"weather\":{";
	ss << "\"mAmbientTemperature\":" << sharedData->mAmbientTemperature << ",";
	ss << "\"mTrackTemperature\":" << sharedData->mTrackTemperature << ",";
	ss << "\"mRainDensity\":" << sharedData->mRainDensity << ",";
	ss << "\"mWindSpeed\":" << sharedData->mWindSpeed << ",";
	ss << "\"mWindDirectionX\":" << sharedData->mWindDirectionX << ",";
	ss << "\"mWindDirectionY\":" << sharedData->mWindDirectionY << ",";
	ss << "\"mCloudBrightness\":" << sharedData->mCloudBrightness << "}";
}

bool contains(std::string stringToSearch, std::string stringToFind)	{
	// Returns true if the stringToSearch contains the stringToFind
	if (stringToSearch.compare("") > 0)	{
		return ((int)stringToSearch.find(stringToFind) > (int)std::string::npos);
	}
	else{
		// queryString is empty, return everything
		return true;
	}
}

void addSeparator(std::stringstream& ss, bool skip)	{
	// Adds a comma unless skipped
	if (!skip)	{
		ss << ",";
	}
}

void buildResponse(std::stringstream& ss, const SharedMemory* sharedData, std::string queryString)	{
	ss << "{";
	bool skipSeparator = true;
	if (contains(queryString, "buildInfo"))	{
		buildBuildInfo(ss, sharedData);
		skipSeparator = false;
	}
	if (contains(queryString, "gameStates"))	{
		addSeparator(ss, skipSeparator);
		buildGameStates(ss, sharedData);
		skipSeparator = false;
	}
	if (contains(queryString, "participants"))	{
		addSeparator(ss, skipSeparator);
		buildParticipants(ss, sharedData);
		skipSeparator = false;
	}
	if (contains(queryString, "unfilteredInputs"))	{
		addSeparator(ss, skipSeparator);
		buildUnfilteredInput(ss, sharedData);
		skipSeparator = false;
	}
	if (contains(queryString, "vehicleInformation"))	{
		addSeparator(ss, skipSeparator);
		buildVehicleInformation(ss, sharedData);
		skipSeparator = false;
	}
	if (contains(queryString, "eventInformation"))	{
		addSeparator(ss, skipSeparator);
		buildEventInformation(ss, sharedData);
		skipSeparator = false;
	}
	if (contains(queryString, "timings"))	{
		addSeparator(ss, skipSeparator);
		buildTimings(ss, sharedData);
		skipSeparator = false;
	}
	if (contains(queryString, "flags"))	{
		addSeparator(ss, skipSeparator);
		buildFlags(ss, sharedData);
		skipSeparator = false;
	}
	if (contains(queryString, "pitInfo"))	{
		addSeparator(ss, skipSeparator);
		buildPitInfo(ss, sharedData);
		skipSeparator = false;
	}
	if (contains(queryString, "carState"))	{
		addSeparator(ss, skipSeparator);
		buildCarState(ss, sharedData);
		skipSeparator = false;
	}
	if (contains(queryString, "motionDeviceRelated"))	{
		addSeparator(ss, skipSeparator);
		buildMotionDeviceRelated(ss, sharedData);
		skipSeparator = false;
	}
	if (contains(queryString, "wheelsAndTyres"))	{
		addSeparator(ss, skipSeparator);
		buildWheelsTyres(ss, sharedData);
		skipSeparator = false;
	}
	if (contains(queryString, "carDamage"))	{
		addSeparator(ss, skipSeparator);
		buildCarDamage(ss, sharedData);
		skipSeparator = false;
	}
	if (contains(queryString, "weather"))	{
		addSeparator(ss, skipSeparator);
		buildWeather(ss, sharedData);
		skipSeparator = false;
	}
	ss << "}";
}

std::string getQueryString(struct http_message *hm)	{
	if (hm->query_string.len > 0)	{
		std::string queryString (hm->query_string.p, hm->query_string.len);
		return queryString;
	}else{
		return "";
	}
}

void buildSharedMemoryJson(struct ns_connection *nc, const SharedMemory* sharedData, struct http_message *hm)  {

	// Build the JSON response
	std::stringstream ss;
	buildResponse(ss, sharedData, getQueryString(hm));

	// build HTTP OK response with JSON response body
	ns_printf(nc, "HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json\r\n"
		"Cache-Control: no-cache\r\n"
		"Content-Length: %d\r\n\r\n%s",
		(int)strlen(ss.str().c_str()), ss.str().c_str());
}

void processSharedMemoryData(struct ns_connection *nc, const SharedMemory* sharedData, struct http_message *hm)   {
	// Ensure we're sync'd to the correct data version
	if (sharedData->mVersion != SHARED_MEMORY_VERSION)	{
		// build conflict response
		sendConflict(nc);
		printf("Data version mismatch, please make sure that your pCARS version matches your CREST version\n");
	}
	else{
		buildSharedMemoryJson(nc, sharedData, hm);
	}

}

void processFile(struct ns_connection *nc, HANDLE fileHandle, struct http_message *hm)    {

	const SharedMemory* sharedData = (SharedMemory*)MapViewOfFile(fileHandle, PAGE_READONLY, 0, 0, sizeof(SharedMemory));

	if (sharedData == NULL)	{
		// File found, but could not be mapped to shared memory data
		sendServiceUnavailable(nc);
	}
	else{
		// Process file
		processSharedMemoryData(nc, sharedData, hm);
		// Unmap file
		UnmapViewOfFile(sharedData);
	}

}

void ResponseGenerator::generateResponse(struct ns_connection *nc, struct http_message *hm)	{
	// Open the memory mapped file
	HANDLE fileHandle = OpenFileMappingA(PAGE_READONLY, FALSE, MAP_OBJECT_NAME);

	if (fileHandle == NULL)	{
		// File is not available, build service unavailable response
		sendServiceUnavailable(nc);
	}
	else{
		// File is available, process the file
		processFile(nc, fileHandle, hm);
		// Close the file
		CloseHandle(fileHandle);
	}

}