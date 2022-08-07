
// Selects the player unit
void selectPlayer() {
	trUnitSelectClear();
	trUnitSelectByQV("player");
}

/*
Spawns the player at the vector position pos with heading "heading"
*/
void spawnPlayer(vector pos = vector(0,0,0), int heading = 0) {
	trQuestVarSet("player", trGetNextUnitScenarioNameNumber());
	trArmyDispatch("1,0","Hoplite",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),heading,true);
	// may need additional functionality
}

/*
Initiates travel to another tile.
*/
void travelTo(int dest = 0) {
	xsEnableRule("travel_to_other_map");
	trQuestVarSet("travelTime", trTimeMS() + 1000);
	trQuestVarSet("travelDestination", 0 - dest);
	trUIFadeToColor(0,0,0,1000,0,true);
}


rule travel_to_other_map
inactive
highFrequency
{
	if (trTimeMS() > trQuestVarGet("travelTime")) {
		saveAllData();
		loadScenario("CommunityRPG\map"+1*trQuestVarGet("travelDestination"));
	}
}