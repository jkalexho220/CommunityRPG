int dWallTerrains = 0;
int xWallTerrainPrimary = 0;
int xWallTerrainSecondary = 0;
int xWallIsChasm = 0;

void addWallTerrain(vector pos = vector(0,0,0), bool chasm = false) {
	pos = vectorToGrid(pos);
	xAddDatabaseBlock(dWallTerrains, true);
	xSetInt(dWallTerrains, xWallTerrainPrimary, trGetTerrainType(xsVectorGetX(pos), xsVectorGetZ(pos)));
	xSetInt(dWallTerrains, xWallTerrainSecondary, trGetTerrainSubType(xsVectorGetX(pos), xsVectorGetZ(pos)));
	xSetBool(dWallTerrains, xWallIsChasm, chasm);
}

rule ACTIVE_setup_terrain_database
active
highFrequency
{
	xsDisableSelf();
	/* 
	Create a database of wall terrain types that the
	projectiles/teleporting will be blocked by
	*/
	dWallTerrains = xInitDatabase("WallTerrains");
	xWallTerrainPrimary = xInitAddInt(dWallTerrains, "primary");
	xWallTerrainSecondary = xInitAddInt(dWallTerrains, "secondary");
	xWallIsChasm = xInitAddBool(dWallTerrains, "chasm", false); // a player cannot teleport onto a chasm but projectiles can pass over it.
}