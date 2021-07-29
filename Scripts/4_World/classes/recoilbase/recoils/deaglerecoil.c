modded class DEagleRecoil extends RecoilBase
{
	override void Init()
	{
		super.Init();

		m_MouseOffsetDistance *= 0.8;
		m_CamOffsetDistance *= 0.15;
	}
}