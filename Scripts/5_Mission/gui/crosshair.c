modded class CrossHair
{
	void SetPos(float x, float y)
	{
		if (!m_Widget) return;

		m_Widget.SetPos(x, y);
	}

	void SetSize(float x, float y)
	{
		if (!m_Widget) return;

		m_Widget.SetSize(x, y);
	}

	void SetCurrent(bool state)
	{
		m_Current = state;
	}
}