const int GLOBAL_DATA = 4;
const int LOCAL_DATA = 14;

int dLocalData = 0;
int xLocalDataSize = 0;
int xLocalDataSlot = 0;
int xLocalDataName = 0;

int localDataArray = 0; // an array to temporarily store the slot data in memory

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
	/*
	The dLocalData database contains every requested piece of data. This is how
	the data will be automatically loaded for us.
	*/
	dLocalData = xInitDatabase("localDataSegments");
	xLocalDataSize = xInitAddInt(dLocalData, "size");
	xLocalDataSlot = xInitAddInt(dLocalData, "slot");
	xLocalDataName = xInitAddString(dLocalData, "name");

	localDataArray = zNewArray(mInt, 16, "localData");

	
	/*
	Add data to slots here
	
	EXAMPLE
	name | slot | maximum value
	addSavedData("cow", 1, 10);
	*/

	/*
	EXAMPLE
	Slot 3
	Total size: 400,000
	*/
	addSavedData("exampleHealth", 3, 1000);
	addSavedData("exampleAttack", 3, 100);
	addSavedData("exampleQuest", 3, 4);

	/*
	Slot 4
	Total size: 0
	*/

	/*
	Slot 5
	Total size: 0
	*/

	/*
	Slot 6
	Total size: 0
	*/

	/*
	Slot 7
	Total size: 0
	*/
	xsDisableSelf();
}

void updateBirthday() {
	int birthday = 0;
	int currentdata = 0;
	// check the birthday of every map
	for(i=11; <= 99) {
		currentdata = trGetScenarioUserData(0, "CommunityRPG\crpg"+i+".scx");
		if (currentdata > birthday) {
			birthday = currentdata;
		}
	}
	// also check the birthday of map 0
	currentdata = trGetScenarioUserData(0, "CommunityRPG\crpg0.scx");
	if (currentdata > birthday) {
		birthday = currentdata;
	}
	// look at me. I'm the birthday now
	trSetCurrentScenarioUserData(0, birthday + 1);
}

void saveAllData() {
	xsSetContextPlayer(0);
	int currentdata = 0;
	int slot = 0;

	// Find the highest birthday number and set our birthday to that +1
	updateBirthday();

	// save the data
	for(i=1; < 16) {
		// make all data 0 in preparation for updates
		zSetInt(localDataArray, i, 0);
	}
	// read data from the various quest vars in backwards order
	xSetPointer(dLocalData, 1);
	for(i=xGetDatabaseCount(dLocalData); >0) {
		xDatabaseNext(dLocalData, true); // database search is backwards this time
		debugLog(xGetString(dLocalData, xLocalDataName));
		// read the data in the quest var
		slot = xGetInt(dLocalData, xLocalDataSlot);
		currentdata = trQuestVarGet(xGetString(dLocalData, xLocalDataName));
		
		// floor and ceiling the data so it fits in the data range
		currentdata = xsMax(0, currentdata);
		currentdata = xsMin(currentdata, xGetInt(dLocalData, xLocalDataSize) - 1);
		
		// shift the slot data over and insert our data
		zSetInt(localDataArray, slot, zGetInt(localDataArray, slot) * xGetInt(dLocalData, xLocalDataSize) + currentdata);
	}
	// save all the data into the slots
	for(i=1; < 16) {
		trSetCurrentScenarioUserData(i, zGetInt(localDataArray, i));
	}
}

// Reads data from the specified map
void loadAllData(int index = 0) {
	xsSetContextPlayer(0);
	string mapName = "CommunityRPG\crpg" + index + ".scx"; // this is the map that we will pull data from
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
			zSetInt(localDataArray, i, 0);
		}
	} else {
		for(i=LOCAL_DATA; < 16) {
			// load all the raw local data into the array
			zSetInt(localDataArray, i, trGetScenarioUserData(i));
		}
	}

	xSetPointer(dLocalData, 1);
	// turn all the data into quest vars by traversing forwards
	for(i=xGetDatabaseCount(dLocalData); >0) {
		debugLog(xGetString(dLocalData, xLocalDataName));
		// get the data from the slot
		slot = xGetInt(dLocalData, xLocalDataSlot);
		currentdata = zGetInt(localDataArray, slot); 

		// read the data segment
		trQuestVarSet(xGetString(dLocalData, xLocalDataName), iModulo(xGetInt(dLocalData, xLocalDataSize), currentdata));
		
		// remove the data segment and continue
		zSetInt(localDataArray, slot, currentdata / xGetInt(dLocalData, xLocalDataSize));
		xDatabaseNext(dLocalData);
	}
}
