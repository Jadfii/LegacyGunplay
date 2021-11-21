modded class PlayerBase extends ManBase
{
	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)	
	{
		super.CommandHandler(pDt,pCurrentCommandID,pCurrentCommandFinished);

		if (IsADS() && IsSwayExhausted())
		{
			//StaminaHUDNotifier(false);
		}
	}

	override void HideClothing(ItemOptics optic, bool state)
	{
		super.HideClothing(optic, state);
		float default_fov = 30 * Math.DEG2RAD;
		array<int> clothingArray = new array<int>;

		// I don't want to NOT call super, so we're just going to re-enable
		// some clothing pieces if they shouldn't be invisible
		if (state && DayZPlayerCameraOptics.Cast(m_CurrentCamera) && GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT && optic.GetCurrentStepFOV() > default_fov)
		{
			clothingArray.Insert(InventorySlots.BODY);
			SetInvisibleRecursive(false, this, clothingArray);
		}
	}

	override bool CanSprint()
	{
		return !m_MovementState.IsLeaning() && super.CanSprint();
	}
}