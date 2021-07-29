class CfgPatches
{
	class LegacyGunplay_Worlds
	{
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};

class cfgWorlds
{
	class DefaultWorld
	{
		class Weather
		{
			class Overcast
			{
				class Weather1;
				class Weather2;
				class Weather3;
				class Weather4;
				class Weather5;
			};
		};
	};
	class CAWorld: DefaultWorld
	{
		class Weather: Weather
		{
			class VolFog
			{
				CameraFog=0;
				Item1[]={100, 0.025, 0.475, 0.13, 0.8};
				Item2[]={250, 0.01, 0.45, 0.05, 0.8};
			};
		};
	};
};