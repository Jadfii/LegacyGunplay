modded class StuffedNotfr extends NotifierBase
{
	void StuffedNotfr(NotifiersManager manager)
	{
		m_MinPauseBetweenMessages = 3;
		m_MaxPauseBetweenMessages = 5;
	}

	override private void DisplayMessage()
	{
		float stomach = m_Player.m_PlayerStomach.GetStomachVolume();

		if (stomach >= PlayerConstants.VOMIT_THRESHOLD - 100)
		{
			MessageStatus("I am going to throw up");
		}
		else if (stomach >= PlayerConstants.BT_STOMACH_VOLUME_LVL3)
		{
			MessageStatus("I am SO full");
		}
		else if (stomach >= PlayerConstants.BT_STOMACH_VOLUME_LVL3 - 200)
		{
			MessageStatus("I am so full");
		}
	}
}