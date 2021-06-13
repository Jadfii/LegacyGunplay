class CfgPatches
{
    class OldEnvironment_GUI
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
	class OldEnvironment_GUI
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
                    "OldEnvironment/GUI/imagesets/dayz_crosshairs_modded.imageset"
                };
			};
		};
	};
};