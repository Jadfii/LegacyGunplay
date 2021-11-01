modded class FALRecoil extends RecoilBase
{
	override void Init()
	{
		super.Init();
	
		m_MouseOffsetDistance *= 1.15;
		m_CamOffsetDistance *= 1.2;
	}
}