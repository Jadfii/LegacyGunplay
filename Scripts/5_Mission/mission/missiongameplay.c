modded class MissionGameplay extends MissionBase
{
	Widget m_HudCrossHairsWidget;

	void MissionGameplay()
	{
		m_HudCrossHairsWidget = null;
	}

	override void OnInit()
	{
		super.OnInit();

		if (!m_HudCrossHairsWidget)
		{
			m_HudCrossHairsWidget = GetGame().GetWorkspace().CreateWidgets("LegacyGunplay/GUI/layouts/day_z_crosshairs.layout");

			m_HudCrossHairsWidget.Show(false);
		}
	}

	override void OnMissionFinish()
	{
		super.OnMissionFinish();

		delete m_HudCrossHairsWidget;
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		Man player = GetGame().GetPlayer();
		if (player && m_LifeState == EPlayerStates.ALIVE && !player.IsUnconscious() && m_HudCrossHairsWidget)
		{
			m_HudCrossHairsWidget.Show(true);
		}
	}
}