modded class AkmRecoil extends RecoilBase
{
	override void Init()
	{
		super.Init();

		m_CamOffsetDistance *= 0.8;
		m_MouseOffsetDistance *= 0.8;
	}
}