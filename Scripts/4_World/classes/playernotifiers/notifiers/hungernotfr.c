modded class HungerNotfr extends NotifierBase
{
	private const float	HUNGRY_TRESHOLD = 1000;
	private const float	VERY_HUNGRY_TRESHOLD = 500;
	private const float	FATALLY_HUNGRY_TRESHOLD = 0;

	void HungerNotfr(NotifiersManager manager)
	{
		m_MinPauseBetweenMessages = 3;
		m_MaxPauseBetweenMessages = 5;
	}

	override private void DisplayMessage()
	{
		EStatLevels energy_level = m_Player.GetStatLevelEnergy();

		switch (energy_level)
		{
			case EStatLevels.CRITICAL:
				MessageImportant("I'm dying of starvation");
				break;
			case EStatLevels.LOW:
				MessageStatus("I'm extremely hungry");
				break;
			case EStatLevels.MEDIUM:
				MessageStatus("My stomach grumbles");
				break;
			
			default:
				break;
		}
	}
}