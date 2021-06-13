modded class FracturedLegNotfr extends NotifierBase
{
	void FracturedLegNotfr(NotifiersManager manager)
	{
		m_MinPauseBetweenMessages = 3;
		m_MaxPauseBetweenMessages = 5;
	}

	override private void DisplayMessage()
	{
		float avgLegHealth = m_Player.GetHealth("RightLeg", "") + m_Player.GetHealth("LeftLeg", "") + m_Player.GetHealth("RightFoot", "") + m_Player.GetHealth("LeftFoot", "");
		avgLegHealth *= 0.25;

		if (m_Player.m_BrokenLegState == eBrokenLegs.BROKEN_LEGS && avgLegHealth <= PlayerConstants.BROKEN_LEGS_LOW_HEALTH_THRESHOLD)
		{
			MessageImportant("I think my leg is broken");
		}
		else if (avgLegHealth <= PlayerConstants.BROKEN_LEGS_HIGH_HEALTH_THRESHOLD)
		{
			MessageStatus("My leg hurts");
		}
		else
		{
			MessageStatus("My leg is painful");
		}
	}
}