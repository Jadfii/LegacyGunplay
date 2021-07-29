modded class Ak74Recoil extends RecoilBase
{
	override void Init()
	{
		super.Init();
	
		m_MouseOffsetDistance *= 0.6;
		m_CamOffsetDistance *= 0.9;
	}
}