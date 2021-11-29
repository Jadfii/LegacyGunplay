modded class DayZPlayerCameras
{
	static const float TIME_CAMERACHANGE_IRONSIGHTS = 0.25;
	static const float TIME_CAMERACHANGE_OPTICS = 0.3;

	static const float TIME_CAMERACHANGE_MIN = 0.2;
	static const float TIME_CAMERACHANGE_MAX = 0.4;

	/* It's unfortunate that static methods have to be overriden, not modded with super :( */

	override static void RegisterTransitionTime(int pFrom, int pTo, float pTime, bool pTwoWay)
	{
		float transition_time = pTime;

		if (pTo == DAYZCAMERA_IRONSIGHTS || pFrom == DAYZCAMERA_IRONSIGHTS)
			transition_time = TIME_CAMERACHANGE_IRONSIGHTS;

		if (pTo == DAYZCAMERA_OPTICS || pFrom == DAYZCAMERA_OPTICS)
			transition_time = TIME_CAMERACHANGE_OPTICS;

		int key = (pFrom * 65536) + pTo;
		m_TransitionTimes.Set(key, transition_time);
		if (pTwoWay)
		{
			RegisterTransitionTime(pTo, pFrom, pTime, false);
		}
	}

	override static float OnCameraChange(DayZPlayer pPlayer, int pFrom, int pTo)
	{
		float transTime = GetTransitionTime(pFrom, pTo);

		DbgPrintPlayerCameras("OnCameraChange | ORIG transTime: "+transTime);

		if (pTo != DAYZCAMERA_IRONSIGHTS && pFrom != DAYZCAMERA_IRONSIGHTS && pTo != DAYZCAMERA_OPTICS && pFrom != DAYZCAMERA_OPTICS)
			return transTime;

		PlayerBase player;
		if (!Class.CastTo(player, GetGame().GetPlayer())) return transTime;

		Weapon_Base weapon;
		if (!Class.CastTo(weapon, player.GetItemInHands())) return transTime;

		float weapon_weight = (weapon.GetWeight() / 1000) * 0.5;
		transTime *= weapon_weight;

		float barrel_length = weapon.GetWeaponLength();
		transTime *= barrel_length;
		
		transTime = Math.Clamp(transTime * 0.9, TIME_CAMERACHANGE_MIN, TIME_CAMERACHANGE_MAX);

		DbgPrintPlayerCameras("barrel_length: "+barrel_length);
		DbgPrintPlayerCameras("weapon_weight: "+weapon_weight);
		DbgPrintPlayerCameras("transTime: "+transTime);

		return transTime;
	}

	static void DbgPrintPlayerCameras(string val)
	{
		return;
		Print("DbgPrintPlayerCameras | " + val);
	}
}