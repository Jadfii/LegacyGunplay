/*modded class IngameHud extends Hud
{
	override void Init(Widget hud_panel_widget)
	{
		super.Init(hud_panel_widget);

		m_Notifiers.Show(false);
		m_StatesWidgetNames.Clear();
		m_StatesWidgets.Clear();

		m_Badges.Show(false);
		m_BadgesWidgetNames.Clear();
		m_BadgesWidgets.Clear();
		m_BadgesWidgetDisplay.Clear();
	}

	override void RefreshHudVisibility()
	{
		super.RefreshHudVisibility();

		m_Notifiers.Show(false);
		m_Badges.Show(false);
		m_BadgeNotifierDivider.Show(false);
	}

	override void DisplayTendencyTemp(int key, int tendency, int status)
	{
		ImageWidget w = ImageWidget.Cast(m_Notifiers.FindAnyWidget(String("Icon" + m_StatesWidgetNames.Get(key))));

		if (w) super.DisplayTendencyTemp(key, tendency, status);
	}
}*/