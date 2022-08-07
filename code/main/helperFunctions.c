
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