class PlayerRecoilConstants
{
	// This is applied alongside the prone modifier
	static const float RECOIL_MULTIPLIER_BIPOD = 0.4;

	// Set a range (+- this value) for the random recoil amount
	static const float RANDOM_RECOIL_AMOUNT = 0.15;

	static const float ERECT_MODIFIER = 1;
	static const float CROUCH_MODIFIER = 0.8;
	static const float PRONE_MODIFIER = 0.65;

	// Time (~ms) before mouse offset is applied
	static const float MOUSE_OFFSET_DELAY = 0.1;

	static const float HANDS_X_OFFSET_MODIFIER = 0.1;
	static const float HANDS_Y_OFFSET_MODIFIER = 0.8;

	static const float MOUSE_X_OFFSET_MODIFIER = 0.7;

	static const float CAM_Z_OFFSET_MODIFIER = 1.75;
}

modded class RecoilBase
{
	float m_CurrentHandsOffsetX;
	float m_CurrentHandsOffsetY;

	protected float m_RandomRecoilModifier;
	protected float m_AttachmentsModifier;
	protected float m_OpticsCamModifier;
	protected float m_StanceModifier;

	protected float m_OffsetDirection;

	protected int m_ShakeCount;

	override void PostInit(Weapon_Base weapon)
	{
		// Bump the mouse offset distance!
		m_MouseOffsetDistance *= 2.75;

		super.PostInit(weapon);

		// Generate random recoil modifier
		m_RandomRecoilModifier = m_Player.GetRandomGeneratorSyncManager().GetRandomInRange(RandomGeneratorSyncUsage.RGSRecoil, 1 - PlayerRecoilConstants.RANDOM_RECOIL_AMOUNT, 1 + PlayerRecoilConstants.RANDOM_RECOIL_AMOUNT);

		// Make the original recoil modifier ineffective
		m_RecoilModifier = Vector(1, 1, 1);

		// Decrease the X mouse offset
		m_MouseOffsetTarget[0] = m_MouseOffsetTarget[0] * PlayerRecoilConstants.MOUSE_X_OFFSET_MODIFIER;

		// Set a constant reload time
		m_ReloadTime = 0.25;
		// Since our reload time is constant, these "RelativeTime" values can
		// be constant, too
		m_HandsOffsetRelativeTime = 1;
		m_CamOffsetRelativeTime = 1;
		m_MouseOffsetRelativeTime = 0.5;

		// Here we tweak the recoil curves slightly
		// Mostly the vanilla curves are somewhat similar to what we want
		for (int i = 0; i < m_HandsCurvePoints.Count(); i++)
		{
			float y_modifier = 1;

			if (i == 0) y_modifier = 4;
			else if (i == 1) y_modifier = 2;
			else if (i == 2) y_modifier = 1;
			else if (i == 3) y_modifier = 0.2;

			vector handsPoint = m_HandsCurvePoints[i];
			m_HandsCurvePoints[i] = Vector(handsPoint[0], (handsPoint[1] * y_modifier), 0);
		}

		m_AttachmentsModifier = GetAttachmentsModifier(GetWeapon());
		m_OpticsCamModifier = GetOpticsCamModifier(GetWeapon());
		m_StanceModifier = GetStanceModifier();

		m_OffsetDirection = GetOffsetDirection();

		DbgPrintRecoilBase("PostInit | m_StanceModifier: "+m_StanceModifier);
		DbgPrintRecoilBase("PostInit | m_AttachmentsModifier: "+m_AttachmentsModifier);
		DbgPrintRecoilBase("PostInit | m_OpticsCamModifier: "+m_OpticsCamModifier);
		DbgPrintRecoilBase("PostInit | m_OffsetDirection: "+m_OffsetDirection);
		DbgPrintRecoilBase("PostInit | m_MouseOffsetTarget: "+m_MouseOffsetTarget);
	}

	override void ApplyMouseOffset(float pDt, out float pRecResultX, out float pRecResultY)
	{
		// apply delay
		if (m_TimeNormalized < PlayerRecoilConstants.MOUSE_OFFSET_DELAY) return;

		pRecResultX *= m_OffsetDirection;

		super.ApplyMouseOffset(pDt, pRecResultX, pRecResultY);
	}

	override void ApplyCamOffset(SDayZPlayerAimingModel pModel)
    {
		pModel.m_fCamPosOffsetZ += (m_CurrentHandsOffsetY * m_CamOffsetDistance) / 2;
		pModel.m_fCamPosOffsetZ *= m_AttachmentsModifier * m_OpticsCamModifier * m_RandomRecoilModifier * m_StanceModifier;
		pModel.m_fCamPosOffsetZ *= PlayerRecoilConstants.CAM_Z_OFFSET_MODIFIER;

		DbgPrintRecoilBase("ApplyCamOffset | pModel.m_fCamPosOffsetZ: "+pModel.m_fCamPosOffsetZ);
    }

	override void ApplyHandsOffset(float pDt, out float pRecResultX, out float pRecResultY)
	{
		super.ApplyHandsOffset(pDt, pRecResultX, pRecResultY);

		pRecResultX = pRecResultY * (m_OffsetDirection * m_RandomRecoilModifier) * PlayerRecoilConstants.HANDS_X_OFFSET_MODIFIER;
		pRecResultY *= PlayerRecoilConstants.HANDS_Y_OFFSET_MODIFIER;

		m_CurrentHandsOffsetX = pRecResultX;
		m_CurrentHandsOffsetY = pRecResultY;

		//DbgPrintRecoilBase("ApplyHandsOffset | pRecResultX: "+pRecResultX.ToString());
		//DbgPrintRecoilBase("ApplyHandsOffset | pRecResultY: "+pRecResultY.ToString());
	}

	void ApplyShakeOffset(float pDt, out float pRecResultX, out float pRecResultY)
	{
		// Disable this for now
		return;
		float weight = Math.InverseLerp(1, 0, m_TimeNormalized);

		m_ShakeCount++;
		int shakes_threshold = Math.Round(m_Player.GetRandomGeneratorSyncManager().GetRandomInRange(RandomGeneratorSyncUsage.RGSAimingModel, 2, 4));

		if (m_ShakeCount > shakes_threshold)
		{
			m_ShakeCount = 0;

			float modifier = m_Player.GetRandomGeneratorSyncManager().GetRandomInRange(RandomGeneratorSyncUsage.RGSRecoil, 0.1, 0.4);
			pRecResultX = modifier * weight * m_Player.GetRandomGeneratorSyncManager().GetRandomInRange(RandomGeneratorSyncUsage.RGSRecoil, 0, 1);
			pRecResultY = modifier * weight * m_Player.GetRandomGeneratorSyncManager().GetRandomInRange(RandomGeneratorSyncUsage.RGSRecoil, 0, 1);
		}

		DbgPrintRecoilBase("ApplyShakeOffset | pRecResultX: "+pRecResultX);
		DbgPrintRecoilBase("ApplyShakeOffset | pRecResultY: "+pRecResultY);
	}

	override void Update(SDayZPlayerAimingModel pModel, out float axis_mouse_x, out float axis_mouse_y, out float axis_hands_x, out float axis_hands_y, float pDt)
	{
		super.Update(pModel, axis_mouse_x, axis_mouse_y, axis_hands_x, axis_hands_y, pDt);

		ApplyShakeOffset(pDt, axis_hands_x, axis_hands_y);

		axis_mouse_x *= m_AttachmentsModifier * m_RandomRecoilModifier * m_StanceModifier;
		axis_mouse_y *= m_AttachmentsModifier * m_RandomRecoilModifier * m_StanceModifier;
		
		axis_hands_x *= m_AttachmentsModifier * m_RandomRecoilModifier * m_StanceModifier;
		axis_hands_y *= m_AttachmentsModifier * m_RandomRecoilModifier * m_StanceModifier;
	}

	float GetOpticsCamModifier(Weapon_Base weapon)
	{
		float modifier = 1;
		
		// Prevent camera clipping for the ACOG by reducing the Z camera offset
		ACOGOptic acog;
		if (weapon && Class.CastTo(acog, weapon.GetAttachedOptics()))
		{
			modifier = 0.5;
		}

		return modifier;
	}

	float GetBipodModifier()
	{
		float modifier = 1;

		if (m_Player && m_Player.IsUsingBipod())
		{
			modifier = PlayerRecoilConstants.RECOIL_MULTIPLIER_BIPOD;
		}

		return modifier;
	}

	float GetAttachmentsModifier(Weapon_Base weapon)
	{
		float modifier = 1;
		
		if (weapon)
		{
			modifier = weapon.GetPropertyModifierObject().m_RecoilModifier;
		}

		modifier *= GetBipodModifier();

		return modifier;
	}

	/** 
	 * Get a modifier from player stance.
	 * Lower stance means less recoil
	 * 
	*/
	float GetStanceModifier()
	{
		float stance_modifier = 1;
		if (!m_Player) return stance_modifier;

		if (m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_RAISEDERECT))
		{
			stance_modifier *= PlayerRecoilConstants.ERECT_MODIFIER;
		}
		else if (m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_RAISEDCROUCH))
		{
			stance_modifier *= PlayerRecoilConstants.CROUCH_MODIFIER;
		}
		else if (m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_RAISEDPRONE))
		{
			stance_modifier *= PlayerRecoilConstants.PRONE_MODIFIER;
		}

		return stance_modifier;
	}

	float GetOffsetDirection()
	{
		if (!m_Player || !m_Player.GetAimingModel()) return -1;

		return m_Player.GetAimingModel().GetRecoilOffsetDirection();
	}

	void DbgPrintRecoilBase(string val)
	{
		return;
		Print("DbgPrintRecoilBase | "+val);
	}
}