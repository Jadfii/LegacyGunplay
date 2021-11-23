modded class DayZPlayerCamera1stPerson extends DayZPlayerCameraBase
{
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