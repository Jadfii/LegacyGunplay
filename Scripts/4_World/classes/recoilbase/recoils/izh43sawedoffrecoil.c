class Izh43SawedOffRecoil extends Izh43Recoil
{
	override void Init()
	{
		super.Init();
	
		m_MouseOffsetDistance *= 1.5;
		m_CamOffsetDistance *= 0.9;
	}
}