modded class BloodNotfr extends NotifierBase
{
	void BloodNotfr(NotifiersManager manager)
	{
		m_MinPauseBetweenMessages = 3;
		m_MaxPauseBetweenMessages = 5;
	}

	override private void DisplayMessage()
	{
		int bleed_count = m_Player.GetBleedingManagerServer().GetBleedingSourcesCount();
		
		if (bleed_count == 1)
		{
			MessageImportant("I feel warm blood on my clothes");
		}
		else if (bleed_count > 1)
		{
			MessageImportant("I feel warm blood on multiple pieces of clothing");
		}
	}
}