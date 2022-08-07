const int GLOBAL_DATA = 4;
const int LOCAL_DATA = 14;

int dLocalData = 0;
int xLocalDataSize = 0;
int xLocalDataSlot = 0;
int xLocalDataName = 0;

int localDataArray = 0; // an array to temporarily store the slot data in memory

void saveAllData() {
	// TODO: Data save here
	int currentdata = 0;
	int slot = 0;

	// Find the highest birthday number and set our birthday to that +1
	int birthday = 0;
	for(i=11; <= 99) {
		currentdata = trGetScenarioUserData(0, "CommunityRPG\crpg"+i+".scx");
		if (currentdata > birthday) {
			birthday = currentdata;
		}
	}
	birthday = birthday + 1;
	trSetCurrentScenarioUserData(0, birthday);

	// save the data
	for(i=1; < 16) {
		// make all data 0 in preparation for updates
		zSetInt(localDataArray, i, 0);
	}
	// read data from the various quest vars in backwards order
	for(i=xGetDatabaseCount(dLocalData); >0) {
		xDatabaseNext(dLocalData, false); // database search is backwards this time
		slot = xGetInt(dLocalData, xLocalDataSlot);
		currentdata = trQuestVarGet(xGetString(dLocalData, xLocalDataName));
		zSetInt(localDataArray, slot, zGetInt(localDataArray, slot) * xGetInt(dLocalData, xLocalDataSize) + currentdata);
	}
	// save all the data into the slots
	for(i=1; < 16) {
		trSetCurrentScenarioUserData(i, zGetInt(localDataArray, i));
	}
}

// Reads data from the specified map
void loadAllData(int index = 0) {
	// TODO: Data load here
	string mapName = "crpg" + index + ".scx";
	int currentdata = 0;
	int slot = 0;

	// load global data
	for(i=1; < LOCAL_DATA) {
		// load all the raw global data into the array
		zSetInt(localDataArray, i, trGetScenarioUserData(i, mapName));
	}
	// load local data
	if (trGetScenarioUserData(0, "CommunityRPG\crpg0.scx") > trGetScenarioUserData(0)) {
		// if a new game was started, wipe local data
		for(i=LOCAL_DATA; < 16) {
			trSetCurrentScenarioUserData(i, 0);
		}
	} else {
		for(i=LOCAL_DATA; < 16) {
			// load all the raw local data into the array
			zSetInt(localDataArray, i, trGetScenarioUserData(i));
		}
	}

	// turn all the data into quest vars by traversing forwards
	for(i=xGetDatabaseCount(dLocalData); >0) {
		xDatabaseNext(dLocalData);
		slot = xGetInt(dLocalData, xLocalDataSlot);
		currentdata = zGetInt(localDataArray, slot);
		trQuestVarSet(xGetString(dLocalData, xLocalDataName), iModulo(xGetInt(dLocalData, xLocalDataSize), currentdata));
		zSetInt(localDataArray, slot, currentdata / xGetInt(dLocalData, xLocalDataSize));
	}
}

void addSavedData(string varName = "", int slot = 14, int size = 1) {
	xAddDatabaseBlock(dLocalData, true);
	xSetString(dLocalData, xLocalDataName, varName);
	xSetInt(dLocalData, xLocalDataSize, size);
	xSetInt(dLocalData, xLocalDataSlot, slot);
}

rule ACTIVE_setup_local_data
active
highFrequency
{
	dLocalData = xInitDatabase("localDataSegments");
	xLocalDataSize = xInitAddInt(dLocalData, "size");
	xLocalDataSlot = xInitAddInt(dLocalData, "slot");
	xLocalDataName = xInitAddString(dLocalData, "name");

	localDataArray = zNewArray(mInt, 16, "localData");

	// add data to slots here
	/*
	name | slot | maximum value
	addSavedData("cow", 1, 10);
	*/
	xsDisableSelf();
}