class CfgPatches
{
    class LegacyGunplay_Scripts
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
	class LegacyGunplay
	{
		dir = "LegacyGunplay";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 1;
		name = "LegacyGunplay";
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
				files[] = {"LegacyGunplay/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"LegacyGunplay/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"LegacyGunplay/Scripts/5_Mission"};
			};
		};
	};
};