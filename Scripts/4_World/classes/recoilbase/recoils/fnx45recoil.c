modded class Fnx45Recoil extends RecoilBase
{
	override void Init()
	{
		super.Init();

		m_HandsCurvePoints.Clear();
		
		vector point_1;
		vector point_2;
		vector point_3;
		vector point_4;
		point_1[0] = m_Player.GetRandomGeneratorSyncManager().GetRandomInRange(RandomGeneratorSyncUsage.RGSRecoil,0.5,1.5); point_1[1] = 2; point_1[2] = 0;
		point_2[0] = m_Player.GetRandomGeneratorSyncManager().GetRandomInRange(RandomGeneratorSyncUsage.RGSRecoil,-0.25,0.25); point_2[1] = 3; point_2[2] = 0;
		point_3[0] = m_Player.GetRandomGeneratorSyncManager().GetRandomInRange(RandomGeneratorSyncUsage.RGSRecoil,-0.5,-2); point_3[1] = 1; point_3[2] = 0;
		point_4[0] = m_Player.GetRandomGeneratorSyncManager().GetRandomInRange(RandomGeneratorSyncUsage.RGSRecoil,-0.25,0); point_4[1] = 0.5; point_4[2] = 0;
		m_HandsCurvePoints.Insert(point_1);//forms a 2 dimensional spline(z is ignored)
		m_HandsCurvePoints.Insert(point_2);
		m_HandsCurvePoints.Insert(point_3);
		m_HandsCurvePoints.Insert(point_4);
		m_HandsCurvePoints.Insert("0 0 0");

		/*for (int i = 0; i < m_HandsCurvePoints.Count(); i++)
		{
			vector handsPoint = m_HandsCurvePoints[i];
			m_HandsCurvePoints[i] = Vector(handsPoint[0], (handsPoint[1] * 0.6), 0);
		}*/

		m_CamOffsetDistance *= 0.3;
	}
}