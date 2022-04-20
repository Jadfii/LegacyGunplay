modded class LongHornRecoil extends RecoilBase
{
	override void Init()
	{
		super.Init();

		m_MouseOffsetDistance *= 0.85;
		m_CamOffsetDistance *= 0.3;
	}
}