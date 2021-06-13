modded class SickNotfr extends NotifierBase
{
	void SickNotfr(NotifiersManager manager)
	{
		m_MinPauseBetweenMessages = 3;
		m_MaxPauseBetweenMessages = 5;
	}

	override private void DisplayMessage()
	{
		MessageStatus("I feel sick");
	}
}