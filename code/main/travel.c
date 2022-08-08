int dTravelEntrances = 0;

int xTravelEntrancePos = 0;
int xTravelEntranceIndex = 0;
int xTravelEntranceName = 0;
int xTravelEntranceRadius = 0;
int xTravelEntranceActive = 0;
int xTravelEntranceHeading = 0;

rule ACTIVE_setup_travel_entrances
active
highFrequency
{
	dTravelEntrances = xInitDatabase("travelEntrances");
	xTravelEntrancePos = xInitAddVector(dTravelEntrances, "pos");
	xTravelEntranceIndex = xInitAddInt(dTravelEntrances, "dest");
	xTravelEntranceName = xInitAddString(dTravelEntrances, "name");
	xTravelEntranceRadius = xInitAddFloat(dTravelEntrances, "radius");
	xTravelEntranceActive = xInitAddBool(dTravelEntrances, "active");
	xTravelEntranceHeading = xInitAddInt(dTravelEntrances, "heading");

	xsDisableSelf();
}

// sets up a map entrance and adds it to the dTravelEntrances database
void initializeMapEntrance(vector pos = vector(0,0,0), int destIndex = 0, string destName = "", float radius = 0, int heading = 0) {
	xAddDatabaseBlock(dTravelEntrances, true);
	xSetVector(dTravelEntrances, xTravelEntrancePos, pos);
	xSetInt(dTravelEntrances, xTravelEntranceIndex, destIndex);
	xSetString(dTravelEntrances, xTravelEntranceName, destName);
	xSetFloat(dTravelEntrances, xTravelEntranceRadius, xsPow(radius, 2)); // we square the radius so distance calcs are easier later
	xSetBool(dTravelEntrances, xTravelEntranceActive, true); // set to true because the player will spawn on top of one of the entrances
	xSetInt(dTravelEntrances, xTravelEntranceHeading, heading);
	trEventSetHandler(100000 + destIndex, "travelTo");
}


/*
Initiates travel to another tile.
*/
void travelTo(int dest = 0) {
	xsEnableRule("travel_to_other_map");
	trQuestVarSet("travelTime", trTimeMS() + 1000);
	trQuestVarSet("travelDestination", dest - 100000);
	trUIFadeToColor(0,0,0,1000,0,true);
	// TODO: Additional things such as remembering the player's current health
	saveAllData();
}

/* 
Finds the neighbor that the player came from at the start of the map
by searching all the neighbors and picking the one with the highest
birthday number.
*/
int findMostRecentNeighbor() {
	int index = 0;
	int birthday = -1;
	int current = 0;
	for(i=xGetDatabaseCount(dTravelEntrances); >0) {
		xDatabaseNext(dTravelEntrances);
		current = trGetScenarioUserData(0, "CommunityRPG\crpg" + xGetInt(dTravelEntrances, xTravelEntranceIndex) + ".scx");
		if (current > birthday) {
			birthday = current;
			index = xGetInt(dTravelEntrances, xTravelEntranceIndex); // save the destination index
		}
	}
	trQuestVarSet("enteredFrom", index);
	debugLog("Came from " + index);
	debugLog("Birthday is " + birthday);
	trSetCurrentScenarioUserData(0, birthday);
	return(index);
}

/*
Teleports the player object to the right entrance and also damages them 
to match the saved health
*/
void enterMap() {
	if (xGetDatabaseCount(dTravelEntrances) > 0) {
		// spawn the player at the entrance that they came from
		for(i=xGetDatabaseCount(dTravelEntrances); >0) {
			xDatabaseNext(dTravelEntrances);
			if (xGetInt(dTravelEntrances, xTravelEntranceIndex) == trQuestVarGet("enteredFrom")) {
				spawnPlayer(xGetVector(dTravelEntrances, xTravelEntrancePos), xGetInt(dTravelEntrances, xTravelEntranceHeading));
				break;
			}
		}
		// TODO: Damage the player based on their current health loaded from data save
		selectPlayer();

		// Start checking the entrances
		trDelayedRuleActivation("check_travel_entrances");
	}
}


rule travel_to_other_map
inactive
highFrequency
{
	if (trTimeMS() > trQuestVarGet("travelTime")) {
		trGameLoadScenario("CommunityRPG\crpg" + 1*trQuestVarGet("travelDestination") + ".scx");
	}
}


rule check_travel_entrances
inactive
highFrequency
{
	/*
	There is a 1-second delay before this trigger starts looping to
	give the player object time to teleport to the correct location.
	Otherwise, the player will receive a choice dialog upon immediately
	entering the map
	*/
	if (trTime() > cActivationTime) {
		/*
		Each trigger loop, this advances through the travel entrances database
		by one and checks the player unit's distance to the target position
		*/
		if (xGetDatabaseCount(dTravelEntrances) > 0) {
			xDatabaseNext(dTravelEntrances);
			// if the player is within the radius of this entrance
			if (unitDistanceToVector(1*trQuestVarGet("player"), xGetVector(dTravelEntrances, xTravelEntrancePos)) < xGetFloat(dTravelEntrances, xTravelEntranceRadius)) {
				// if the player just entered, we give them a choice dialog
				if (xGetBool(dTravelEntrances, xTravelEntranceActive) == false) {
					xSetBool(dTravelEntrances, xTravelEntranceActive, true);
					trShowChoiceDialog("Travel to " + xGetString(dTravelEntrances, xTravelEntranceName) + "?", 
						"Yes", 100000 + xGetInt(dTravelEntrances, xTravelEntranceIndex),
						"No",-1);
				}
			} else {
				// otherwise, set the entrance to inactive
				xSetBool(dTravelEntrances, xTravelEntranceActive, false);
			}
			
		}
	}
}