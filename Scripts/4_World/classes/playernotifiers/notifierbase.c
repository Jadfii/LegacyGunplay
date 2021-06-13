modded class NotifierBase
{
	private float m_SecsSinceLastMessage;
	private float m_SecsToMessage;
	protected float	m_MinPauseBetweenMessages;
	protected float	m_MaxPauseBetweenMessages;

	void NotifierBase(NotifiersManager manager)
	{
		m_MinPauseBetweenMessages = -1;
	}

	protected void DisplayMessage() {}

	void SendMessageRPC(string text, bool important)
	{
		if (!m_Player) return;

		if (GetGame().IsServer())
		{
			GetGame().RPCSingleParam(m_Player, ERPCsModded.RPC_USER_STATUS_MESSAGE, new Param2<string, bool>(text, important), true, m_Player.GetIdentity());
		}
		else
		{
			if (important) m_Player.MessageImportant(text);
			else m_Player.MessageStatus(text);
		}
	}

	void MessageStatus(string text)
	{
		SendMessageRPC(text, false);
	}

	void MessageImportant(string text)
	{
		SendMessageRPC(text, true);
	}

	override void OnTick(float current_time)
	{
		if (m_MinPauseBetweenMessages != -1)
		{
			if (m_SecsSinceLastMessage >= m_SecsToMessage)
			{
				DisplayMessage();
				m_SecsToMessage = Math.RandomInt(m_MinPauseBetweenMessages, m_MaxPauseBetweenMessages) * 10000;
				m_SecsSinceLastMessage = 0;
			}
			else
			{
				m_SecsSinceLastMessage += current_time - m_TickIntervalLastTick;
			}
		}

		super.OnTick(current_time);
	}
}