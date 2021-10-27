modded class SiagaRecoil extends RecoilBase
{
	override void Init()
	{
		super.Init();

		m_MouseOffsetDistance *= 0.5;
		m_CamOffsetDistance *= 0.1;
	}
}