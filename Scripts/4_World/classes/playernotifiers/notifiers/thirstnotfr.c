modded class ThirstNotfr extends NotifierBase
{
	void ThirstNotfr(NotifiersManager manager)
	{
		m_MinPauseBetweenMessages = 3;
		m_MaxPauseBetweenMessages = 5;
	}

	override private void DisplayMessage()
	{
		EStatLevels water_level = m_Player.GetStatLevelWater();

		switch (water_level)
		{
			case EStatLevels.CRITICAL:
				MessageImportant("I'm dying of dehydration");
				break;
			case EStatLevels.LOW:
				MessageStatus("I really need to drink");
				break;
			case EStatLevels.MEDIUM:
				MessageStatus("I feel thirsty");
				break;

			default:
				break;
		}
	}
}