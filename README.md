# CommunityRPG
This is the most ambitious thing I've ever done.

## How to compile the code
Navigate to the code subfolder and run the following command:
> python xmlify.py

This will produce a .xml file titled **CRPG Trigger Loader.xml**. Move this to the trigger2 folder in Age of Mythology and insert the trigger into the scenario.

## How to customize the code for a specific map
For some maps, you may want additional functionality unique to that map coded in. To do so, you will need to do the following:
1. Add a new folder under the code subfolder with a unique name that is descriptive of your map. 
2. In this folder, add a **files.txt** file. This file should contain the filenames of the files in the folder, in the order that you want them compiled.
3. Navigate to the code subfolder and run the following command:
> python xmlify.py -a *<folder_name>*

Replace the *<folder_name>* with the name of your folder. This command will produce a new .xml file titled **CRPG Trigger Loader <folder_name>.xml**, which also includes a single trigger effect with all of your code packaged in. Add this trigger effect to your scenario, replacing the generic _C Trigger Loader effect.

*Look at the code/intro folder to see an example of this for the introductory scenario*.
