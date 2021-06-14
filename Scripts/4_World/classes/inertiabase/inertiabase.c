class PlayerInertiaConstants
{
	static const float DEFAULT_STRENGTH = 20;
	static const float DEFAULT_SMOOTH_TIME = 0.6;

	static const float MIN_SMOOTH_TIME = 0.1;
	static const float MAX_SMOOTH_TIME = 0.6;
	// this is max inertia multiplier
	static const float MAX_STRENGTH = 110;
	// this is max aim change amount to multiply by strength
	static const float MAX_TURN_CHANGE = 25;
}

class InertiaBase
{
	Weapon_Base m_Weapon;
	PlayerBase m_Player;

	float m_velocityYaw[1];
	float m_velocityPitch[1];
	float m_velocityOffsetYaw[1];
	float m_velocityOffsetPitch[1];
	float m_DynamicsModifier;

	void InertiaBase(Weapon_Base weapon)
	{
		m_Weapon = weapon;
		m_Player = PlayerBase.Cast(weapon.GetHierarchyRootPlayer());

		if (weapon)
		{
			m_Weapon.GetPropertyModifierObject().UpdateModifiers();
		}

		m_DynamicsModifier = 1;
	}

	float GetDynamicsModifier()
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
		DbgPrintInertiaBase("dynamics_modifier: "+dynamics_modifier);

		return dynamics_modifier;
	}

	float GetDynamicsStrength()
	{
		return Math.Clamp(PlayerInertiaConstants.DEFAULT_STRENGTH * m_DynamicsModifier, 0, PlayerInertiaConstants.MAX_STRENGTH);
	}

	float GetDynamicsSmoothTime()
	{
		float dynamics_smoothing = PlayerInertiaConstants.DEFAULT_SMOOTH_TIME;

		return Math.Clamp(dynamics_smoothing * m_DynamicsModifier * 1.5, PlayerInertiaConstants.MIN_SMOOTH_TIME, PlayerInertiaConstants.MAX_SMOOTH_TIME);
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

		m_DynamicsModifier = GetDynamicsModifier();
		float dynamics_strength = GetDynamicsStrength();
		float dynamics_smooth_time = GetDynamicsSmoothTime();

		DbgPrintInertiaBase("dynamics_strength: "+dynamics_strength);
		DbgPrintInertiaBase("dynamics_smooth_time: "+dynamics_smooth_time);

		float inertia_value_x = -(dynamics_strength * aimChangeX);
		float inertia_value_y = -(dynamics_strength * aimChangeY);

		DbgPrintInertiaBase("inertia_value_x: "+inertia_value_x);
		DbgPrintInertiaBase("inertia_value_y: "+inertia_value_y);

		final_offset_x = Math.SmoothCD(pModel.m_fAimXHandsOffset + hands_offset_x, inertia_value_x + hands_offset_x, m_velocityYaw, dynamics_smooth_time, 1000, pDt);
		final_offset_y = Math.SmoothCD(pModel.m_fAimYHandsOffset + hands_offset_y, inertia_value_y + hands_offset_y, m_velocityPitch, dynamics_smooth_time, 1000, pDt);

		DbgPrintInertiaBase("-------------------");
	}

	void DbgPrintInertiaBase(string val)
	{
		return;
		Print("DbgPrintInertiaBase | " + val);
	}
}