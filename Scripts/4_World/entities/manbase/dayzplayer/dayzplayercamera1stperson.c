modded class DayZPlayerCamera1stPerson extends DayZPlayerCameraBase
{
	void DayZPlayerCamera1stPerson(DayZPlayer pPlayer, HumanInputController pInput)
	{
		m_OffsetLS = "0.05 0.02 0"; //x - up, y - forward
	}

	override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		if (m_iBoneIndex != -1)
		{
			m_OffsetLS = m_pPlayer.GetBonePositionLS(m_iBoneIndex);
		}
		else
		{
			m_OffsetLS = "0 0 0";
		}
		
		super.OnUpdate(pDt, pOutResult);
	}

	float GetLRAngle()
	{
		return m_fLeftRightAngle;
	}
}