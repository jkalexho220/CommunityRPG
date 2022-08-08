
// Selects the player unit
void selectPlayer() {
	trUnitSelectClear();
	trUnitSelectByQV("player");
}

/*
Spawns the player at the vector position pos with heading "heading" by 
teleporting the player object to that location
*/
void spawnPlayer(vector pos = vector(0,0,0), int heading = 0) {
	selectPlayer();
	trUnitTeleport(xsVectorGetX(pos),0,xsVectorGetZ(pos));
	trUnitSetHeading(heading);
	// may need additional functionality
}

void displayMapName(string name = "") {
	trOverlayText(name, 5.0, -1, -1, -1);
}