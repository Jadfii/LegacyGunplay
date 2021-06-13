class PlayerInertiaConstants
{
	// this is max after multiplied by aim change
	static const float MAX_INERTIA = 800;
	// this is max inertia multiplier
	static const float MAX_STRENGTH = 90;
}

class InertiaBase
{
	Weapon_Base m_Weapon;
	PlayerBase m_Player;

	float m_velocityYaw[1];
	float m_velocityPitch[1];
	float m_velocityOffsetYaw[1];
	float m_velocityOffsetPitch[1];
	float m_dynamicsStrength = 20;
	float m_dynamicsSmoothTime = 0.6;

	void InertiaBase(Weapon_Base weapon)
	{
		m_Weapon = weapon;
		m_Player = PlayerBase.Cast(weapon.GetHierarchyRootPlayer());

		if (weapon)
		{
			m_Weapon.GetPropertyModifierObject().UpdateModifiers();
		}
	}

	float GetDynamicsStrength()
	{
		float dynamics_modifier = 1;

		if (!m_Weapon)
			return dynamics_modifier;

		float attachments_modifier = m_Weapon.GetPropertyModifierObject().m_InertiaModifier;
		dynamics_modifier *= attachments_modifier;

		float barrel_length = m_Weapon.GetPropertyModifierObject().m_BarrelLength * 0.6;
		dynamics_modifier *= barrel_length;
		
		float weapon_weight = m_Weapon.GetPropertyModifierObject().m_Weight;
		dynamics_modifier *= weapon_weight;

		DbgPrintInertiaBase("attachments_modifier: "+attachments_modifier);
		DbgPrintInertiaBase("barrel_length: "+barrel_length);
		DbgPrintInertiaBase("weapon_weight: "+weapon_weight);

		return Math.Clamp(m_dynamicsStrength * dynamics_modifier, m_dynamicsStrength, PlayerInertiaConstants.MAX_STRENGTH);
	}

	/** 
	 * We are applying the inertia directly onto the aiming model
	 * hands_offset_x/y should be the final values for the hands offset before inertia
	 * 
	*/
	void Update(float pDt, SDayZPlayerAimingModel pModel, float hands_offset_x, float hands_offset_y, out float final_offset_x, out float final_offset_y)
	{
		DbgPrintInertiaBase("DbgPrintInertiaBase | Player: "+m_Player+" | Update");

		//! get aim changes to apply our inertia
		HumanInputController hic = m_Player.GetInputController();
		float aimChangeX = hic.GetAimChange()[0] * Math.RAD2DEG;
		float aimChangeY = hic.GetAimChange()[1] * Math.RAD2DEG;

		DbgPrintInertiaBase("aimChangeX: "+aimChangeX);
		DbgPrintInertiaBase("aimChangeY: "+aimChangeY);

		float dynamics_strength = GetDynamicsStrength();

		DbgPrintInertiaBase("dynamics_strength: "+dynamics_strength);

		float inertia_value_x = -(dynamics_strength * aimChangeX);
		float inertia_value_y = -(dynamics_strength * aimChangeY);

		// limit inertia values
		inertia_value_x = Math.Clamp(inertia_value_x, -PlayerInertiaConstants.MAX_INERTIA, PlayerInertiaConstants.MAX_INERTIA);
		inertia_value_y = Math.Clamp(inertia_value_y, -PlayerInertiaConstants.MAX_INERTIA, PlayerInertiaConstants.MAX_INERTIA);

		DbgPrintInertiaBase("inertia_value_x: "+inertia_value_x);
		DbgPrintInertiaBase("inertia_value_y: "+inertia_value_y);

		final_offset_x = Math.SmoothCD(pModel.m_fAimXHandsOffset + hands_offset_x, inertia_value_x + hands_offset_x, m_velocityYaw, m_dynamicsSmoothTime, 1000, pDt);
		final_offset_y = Math.SmoothCD(pModel.m_fAimYHandsOffset + hands_offset_y, inertia_value_y + hands_offset_y, m_velocityPitch, m_dynamicsSmoothTime, 1000, pDt);

		DbgPrintInertiaBase("-------------------");
	}

	void DbgPrintInertiaBase(string val)
	{
		return;
		Print("DbgPrintInertiaBase | " + val);
	}
}