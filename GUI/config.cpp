class CfgPatches
{
    class LegacyGunplay_GUI
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
	class LegacyGunplay_GUI
	{
		extra=0;
		type="mod";
		dependencies[]={"gui"};
		class defs
		{
			class imageSets
			{
				files[]=
                {
                    "LegacyGunplay/GUI/imagesets/dayz_crosshairs_modded.imageset"
                };
			};
		};
	};
};