/** 
 * INFO
 * 
 * inertiaMultiplier is a multiplier value (0..4)
 *  (no buttstock = 3 / no handguard = 2)
 * recoilMultiplier is a multiplier applied to the mouse/hands/camera offsets
 *  (no buttstock = 1.4 / no handguard = 1.15)
 * swayMultiplier
 * 	(default = 1)
 * 
 * default buttstock modifier (not defined) = 1
 * 
*/

class CfgPatches
{
	class OldEnvironment_Weapons
	{
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Weapons_Optics",
			"DZ_Weapons_Muzzles",
			"DZ_Weapons_Magazines",
			"DZ_Weapons_Supports",
			"DZ_Weapons_Firearms",
			"DZ_Weapons_Firearms_FAL",
			"DZ_Weapons_Firearms_MosinNagant",
			"DZ_Weapons_Firearms_M4",
			"DZ_Weapons_Firearms_AK74",
			"DZ_Weapons_Firearms_AK101",
			"DZ_Weapons_Firearms_AKM"
		};
	};
};

/* Add displayName to bipod slot */
class CfgSlots
{
	class Slot_weaponBipod
	{
		displayName="Bipod";
	};
};

class cfgWeapons
{
	class FAL_Base;
	class FAL: FAL_Base
	{
		modes[]={"SemiAuto"};
	};
	class Mosin9130_Base;
	class Mosin9130: Mosin9130_Base
	{
		attachments[]=
		{
			"weaponWrap",
			"weaponOpticsMosin",
			"suppressorImpro",
			"weaponMuzzleMosin",
			"weaponBayonetMosin",
			"weaponBipod"
		};
	};
	class M4A1_Base;
	class M4A1: M4A1_Base
	{
		attachments[]=
		{
			"weaponButtstockM4",
			"WeaponHandguardM4",
			"weaponWrap",
			"weaponOptics",
			"weaponFlashlight",
			"weaponMuzzleM4",
			"weaponBayonet",
			"weaponBipod"
		};
	};
	class AK74_Base;
	class AK74: AK74_Base
	{
		attachments[]=
		{
			"weaponButtstockAK",
			"WeaponHandguardAK",
			"weaponWrap",
			"weaponOpticsAK",
			"weaponFlashlight",
			"weaponMuzzleAK",
			"weaponBayonetAK",
			"weaponBipod"
		};
	};
	class AK101_Base;
	class AK101: AK101_Base
	{
		attachments[]=
		{
			"weaponButtstockAK",
			"WeaponHandguardAK",
			"weaponWrap",
			"weaponOpticsAK",
			"weaponFlashlight",
			"weaponMuzzleAK",
			"weaponBayonetAK",
			"weaponBipod"
		};
	};
	class AKM_Base;
	class AKM: AKM_Base
	{
		attachments[]=
		{
			"weaponButtstockAK",
			"WeaponHandguardAK",
			"weaponWrap",
			"weaponOpticsAK",
			"weaponFlashlight",
			"weaponMuzzleAK",
			"weaponBayonetAK",
			"weaponBipod"
		};
	};
};

class CfgMagazines
{
	/* MAGAZINES */
	class Magazine_Base;
	class Mag_MP5_15Rnd: Magazine_Base
	{
		inertiaMultiplier=0.9;
	};
	class Mag_MP5_30Rnd: Magazine_Base
	{
		inertiaMultiplier=1.1;
	};
	class Mag_AKM_Drum75Rnd: Magazine_Base
	{
		inertiaMultiplier=2;
	};
	class Mag_STANAGCoupled_30Rnd: Magazine_Base
	{
		inertiaMultiplier=1.5;
	};
	class Mag_CMAG_40Rnd: Magazine_Base
	{
		inertiaMultiplier=1.2;
	};
	class Mag_Saiga_Drum20Rnd: Magazine_Base
	{
		inertiaMultiplier=1.4;
	};
};

class cfgVehicles
{
	/* SCOPES */
	class ItemOptics;
	/*class ACOGOptic: ItemOptics
	{
		class OpticsInfo
		{
			opticsZoomMin=0.105;
			opticsZoomMax=0.105;
			opticsZoomInit=0.105;
		};
	};
	class PUScopeOptic: ItemOptics
	{
		class OpticsInfo
		{
			opticsZoomMin=0.15;
			opticsZoomMax=0.15;
			opticsZoomInit=0.15;
		};
	};*/
	class KazuarOptic: ItemOptics
	{
		inertiaMultiplier=1.3;
		recoilMultiplier=1.1;
	};

	/* MUZZLES */
	class ItemSuppressor;
	class ImprovisedSuppressor: ItemSuppressor
	{
		inertiaMultiplier=1.2;
	};
	class Mosin_Compensator: ItemSuppressor
	{
		recoilMultiplier=0.6;
	};
	class MP5_Compensator: ItemSuppressor
	{
		recoilMultiplier=0.6;
	};
	class M4_Suppressor: ItemSuppressor
	{
		recoilMultiplier=0.8;
	};
	class AK_Suppressor: ItemSuppressor
	{
		recoilMultiplier=0.85;
	};
	class PistolSuppressor: ItemSuppressor
	{
		recoilMultiplier=0.8;
	};

	/* SUPPORTS (BUTTSTOCKS, HANDGUARDS) */
	//bttstcks
	class Inventory_Base;
	class M4_OeBttstck: Inventory_Base
	{
		inertiaMultiplier=0.8;
		recoilMultiplier=1;
	};
	class M4_MPBttstck: Inventory_Base
	{
		inertiaMultiplier=1.5;
		recoilMultiplier=0.8;
	};
	class M4_CQBBttstck: Inventory_Base
	{
		inertiaMultiplier=0.6;
		recoilMultiplier=1.2;
	};
	class AK_WoodBttstck: Inventory_Base
	{
		inertiaMultiplier=1.2;
		recoilMultiplier=0.9;
	};
	class AK74_WoodBttstck: Inventory_Base
	{
		inertiaMultiplier=1.2;
		recoilMultiplier=0.9;
	};
	class AK_FoldingBttstck: Inventory_Base
	{
		inertiaMultiplier=0.75;
		recoilMultiplier=1.1;
	};
	class AK_PlasticBttstck: Inventory_Base
	{
		inertiaMultiplier=0.9;
		recoilMultiplier=1;
	};
	class Fal_OeBttstck: Inventory_Base
	{
		inertiaMultiplier=0.9;
		recoilMultiplier=1;
	};
	class Fal_FoldingBttstck: Inventory_Base
	{
		inertiaMultiplier=1.2;
		recoilMultiplier=0.9;
	};
	//hndgrds
	class M4_PlasticHndgrd: Inventory_Base
	{
		inertiaMultiplier=1.05;
		recoilMultiplier=1;
	};
	class M4_RISHndgrd: Inventory_Base
	{
		inertiaMultiplier=1.05;
		recoilMultiplier=1.05;
	};
	class M4_MPHndgrd: Inventory_Base
	{
		inertiaMultiplier=1.05;
		recoilMultiplier=0.9;
	};
	class AK_WoodHndgrd: Inventory_Base
	{
		inertiaMultiplier=1.05;
		recoilMultiplier=0.95;
	};
	class AK74_Hndgrd: Inventory_Base
	{
		inertiaMultiplier=1.05;
		recoilMultiplier=0.95;
	};
	class AK_RailHndgrd: Inventory_Base
	{
		inertiaMultiplier=1.05;
		recoilMultiplier=1.1;
	};
	class AK_PlasticHndgrd: Inventory_Base
	{
		inertiaMultiplier=0.95;
		recoilMultiplier=1;
	};
	class MP5_RailHndgrd: Inventory_Base
	{
		inertiaMultiplier=0.95;
		recoilMultiplier=1.05;
	};

	//bipods
	class AtlasBipod: Inventory_Base
	{
		displayName="ATLAS Bipod";
		descriptionShort="This bipod provides solid support for longer range engagements. When bipod is deployed and the firer is prone, weapon accuracy is increased dramatically.";
		itemSize[]={1, 2};
	};
};