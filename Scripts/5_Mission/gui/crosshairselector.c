modded class CrossHairSelector extends ScriptedWidgetEventHandler
{
	string m_dynamicCrossHairName = "dynamic_crosshair";
	
	ref DynamicCrossHair dynamicCrossHair;

	override protected void Init()
	{
		super.Init();
				
		Widget child = m_Root.GetChildren();
		while(child)
		{
			if (child.GetName() == m_dynamicCrossHairName)
			{
				dynamicCrossHair = new DynamicCrossHair(child);
			}

			child = child.GetSibling();
		}
	}

	override protected void ShowCrossHair(CrossHair crossHair)
	{
		if (crossHair && crossHair.GetName() == "old_crosshair")
		{
			//! we will want to show our dynamic crosshair if the old crosshair is being shown
			if (dynamicCrossHair)
			{
				if (!dynamicCrossHair.IsCurrent() && dynamicCrossHair.IsShown())
				{
					dynamicCrossHair.Show();
				}

				//! update our crosshair position
				dynamicCrossHair.UpdatePosition();
			}
		}
		else
		{
			if (dynamicCrossHair) dynamicCrossHair.Hide();
		}

		super.ShowCrossHair(crossHair);

		HumanInputController hic = m_Player.GetInputController();
		// hide static crosshair if in freelook
		if (GetCurrentCrossHair() && hic.CameraIsFreeLook())
		{
			GetCurrentCrossHair().Hide();
		}
	}

	override protected void SelectCrossHair()
	{
		if(!m_AM) return;
		
		bool firearmInHands = m_Player.GetItemInHands() && m_Player.GetItemInHands().IsWeapon();

		//! firearms
		if (firearmInHands && m_Player.IsRaised() && !m_Player.IsInIronsights() && !m_Player.IsInOptics() && !m_Player.GetCommand_Melee2())
		{
			ShowCrossHair(GetCrossHairByName("old_crosshair"));
		}
		else
		{
			ShowCrossHair(null);
		}
	}
}