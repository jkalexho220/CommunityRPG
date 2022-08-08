rule intro_00
active
highFrequency
runImmediately
{
	trUIFadeToColor(0,0,0,0,0,true);
	
	trDelayedRuleActivation("intro_01");
	xsDisableSelf();
}

/*
TODO: There will be a choice dialog for returning players warning
them that this will overwrite their savedata permanently
*/

rule intro_01
inactive
highFrequency
{
	updateBirthday();
	for(i=1; < 16) {
		trSetCurrentScenarioUserData(i, 0);
	}
	trQuestVarSet("travelDestination", 11); // placeholder for now
	xsEnableRule("travel_to_other_map");
	xsDisableSelf();
}