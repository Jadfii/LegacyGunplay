modded class DayZPlayerCameras
{
	static const float TIME_CAMERACHANGE_IRONSIGHTS = 0.25;
	static const float TIME_CAMERACHANGE_OPTICS = 0.3;

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
}