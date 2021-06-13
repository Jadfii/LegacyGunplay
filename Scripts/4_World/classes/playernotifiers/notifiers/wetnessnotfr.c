modded class WetnessNotfr extends NotifierBase
{
	void WetnessNotfr(NotifiersManager manager)
	{
		m_MinPauseBetweenMessages = 3;
		m_MaxPauseBetweenMessages = 5;
	}

	override private void DisplayMessage()
	{
		//float wetness = m_Player.GetStatWet().Get();
		float wetness = m_Player.m_Environment.GetItemsWetnessMax();
		
		if (wetness >= GameConstants.STATE_DRENCHED) 
		{
			MessageStatus("I am completely drenched");
		}
		else if (wetness >= GameConstants.STATE_SOAKING_WET)
		{
			MessageStatus("I am soaked through");
		}
		else if (wetness >= GameConstants.STATE_WET)
		{
			MessageStatus("My body feels wet");
		}
		else if (wetness >= GameConstants.STATE_DAMP)
		{
			MessageStatus("I feel damp");
		}
	}
}