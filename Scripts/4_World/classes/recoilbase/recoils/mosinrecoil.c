modded class MosinRecoil extends RecoilBase
{
	override void Init()
	{
		super.Init();

		m_MouseOffsetDistance *= 0.9;
		m_CamOffsetDistance *= 0.25;
	}
}