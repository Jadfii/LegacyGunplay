class CfgPatches
{
    class OldEnvironment_Scripts
    {
        units[]={};
        weapons[]={};
        requiredVersion=0.1;
        requiredAddons[]=
        {
            "DZ_Data"
        };
    };
};

class CfgMods
{
	class OldEnvironment
	{
		dir = "OldEnvironment";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 1;
		name = "OldEnvironment";
		author = "Jad";
		authorID = "0"; 
		version = "0.1"; 
		extra = 0;
		type = "mod";
		
		dependencies[] = { "Game", "World", "Mission" };
		
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"OldEnvironment/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"OldEnvironment/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"OldEnvironment/Scripts/5_Mission"};
			};
		};
	};
};