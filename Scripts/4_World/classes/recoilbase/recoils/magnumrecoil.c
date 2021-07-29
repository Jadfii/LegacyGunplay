modded class MagnumRecoil extends RecoilBase
{
	override void Init()
	{
		super.Init();

		m_MouseOffsetDistance *= 0.8;
		m_CamOffsetDistance *= 0.3;
	}
}