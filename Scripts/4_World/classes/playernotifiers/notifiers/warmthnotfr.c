modded class WarmthNotfr extends NotifierBase
{
	void WarmthNotfr(NotifiersManager manager)
	{
		m_MinPauseBetweenMessages = 3;
		m_MaxPauseBetweenMessages = 5;
	}

	override private void DisplayMessage()
	{
		DSLevelsTemp level = DetermineLevel(GetObservedValue(), PlayerConstants.THRESHOLD_HEAT_COMFORT_MINUS_WARNING, PlayerConstants.THRESHOLD_HEAT_COMFORT_MINUS_CRITICAL, PlayerConstants.THRESHOLD_HEAT_COMFORT_MINUS_EMPTY, PlayerConstants.THRESHOLD_HEAT_COMFORT_PLUS_WARNING,  PlayerConstants.THRESHOLD_HEAT_COMFORT_PLUS_CRITICAL, PlayerConstants.THRESHOLD_HEAT_COMFORT_PLUS_EMPTY);

		switch (level)
		{
			case DSLevelsTemp.BLINKING_PLUS:
				MessageImportant("I am hyperthermic");
				break;
			case DSLevelsTemp.CRITICAL_PLUS:
				MessageStatus("I am sweating");
				break;
			case DSLevelsTemp.WARNING_PLUS:
				MessageStatus("I am overheating");
				break;
			case DSLevelsTemp.WARNING_MINUS:
				MessageStatus("I am shaking");
				break;
			case DSLevelsTemp.CRITICAL_MINUS:
				MessageStatus("I am freezing");
				break;
			case DSLevelsTemp.BLINKING_MINUS:
				MessageImportant("I am hypothermic");
				break;

			default:
				break;
		}
	}
}