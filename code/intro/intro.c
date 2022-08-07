rule intro_00
active
highFrequency
runImmediately
{
	trUIFadeToColor(0,0,0,0,0,true);
	
	trDelayedRuleActivation("intro_01");
	xsDisableSelf();
}

rule intro_01
inactive
highFrequency
{
	saveAllData();
	trQuestVarSet("travelDestination", 11);
	xsEnableRule("travel_to_other_map");
	xsDisableSelf();
}