class PlayerRecoilConstants
{
	// This should be good, but not too crazy
	static const float RECOIL_MULTIPLIER_BIPOD = 0.4;

	// Set a range (+- this value) for the random recoil amount
	static const float RANDOM_RECOIL_AMOUNT = 0.15;
}

modded class RecoilBase
{
	float m_CurrentHandsOffsetX;
	float m_CurrentHandsOffsetY;

	protected float m_RandomRecoilModifier;
	protected float m_AttachmentsModifier;
	protected float m_OpticsCamModifier;

	protected float m_MouseOffsetDelay = 0.1;

	override void PostInit(Weapon_Base weapon)
	{
		// Bump the mouse offset distance!
		m_MouseOffsetDistance *= 3.75;

		super.PostInit(weapon);

		// Generate random recoil modifier
		m_RandomRecoilModifier = m_Player.GetRandomGeneratorSyncManager().GetRandomInRange(RandomGeneratorSyncUsage.RGSRecoil, 1 - PlayerRecoilConstants.RANDOM_RECOIL_AMOUNT, 1 + PlayerRecoilConstants.RANDOM_RECOIL_AMOUNT);

		// Make the original recoil modifier ineffective
		m_RecoilModifier = Vector(1, 1, 1);

		// Decrease the X mouse offset
		m_MouseOffsetTarget[0] = m_MouseOffsetTarget[0] * 0.2;

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

		DbgPrintRecoilBase("PostInit | m_AttachmentsModifier: "+m_AttachmentsModifier);
		DbgPrintRecoilBase("PostInit | m_OpticsCamModifier: "+m_OpticsCamModifier);
		DbgPrintRecoilBase("PostInit | m_ReloadTime: "+m_ReloadTime);
		DbgPrintRecoilBase("PostInit | m_Angle: "+m_Angle);
		DbgPrintRecoilBase("PostInit | m_MouseOffsetTarget: "+m_MouseOffsetTarget);
		DbgPrintRecoilBase("PostInit | m_CamOffsetDistance: "+m_CamOffsetDistance);
	}

	override void ApplyMouseOffset(float pDt, out float pRecResultX, out float pRecResultY)
	{
		// apply delay
		if (m_TimeNormalized < m_MouseOffsetDelay) return;

		super.ApplyMouseOffset(pDt, pRecResultX, pRecResultY);
	}

	override void ApplyCamOffset(SDayZPlayerAimingModel pModel)
    {
		pModel.m_fCamPosOffsetZ += (m_CurrentHandsOffsetY * m_CamOffsetDistance) / 2;
		pModel.m_fCamPosOffsetZ *= m_AttachmentsModifier;
		pModel.m_fCamPosOffsetZ *= m_OpticsCamModifier;
		pModel.m_fCamPosOffsetZ *= m_RandomRecoilModifier;

		DbgPrintRecoilBase("ApplyCamOffset | pModel.m_fCamPosOffsetZ: "+pModel.m_fCamPosOffsetZ);
    }

	override void ApplyHandsOffset(float pDt, out float pRecResultX, out float pRecResultY)
	{
		super.ApplyHandsOffset(pDt, pRecResultX, pRecResultY);

		m_CurrentHandsOffsetX = pRecResultX;
		m_CurrentHandsOffsetY = pRecResultY;

		DbgPrintRecoilBase("ApplyHandsOffset | pRecResultX: "+pRecResultX.ToString());
		DbgPrintRecoilBase("ApplyHandsOffset | pRecResultY: "+pRecResultY.ToString());
	}

	override void Update(SDayZPlayerAimingModel pModel, out float axis_mouse_x, out float axis_mouse_y, out float axis_hands_x, out float axis_hands_y, float pDt)
	{
		super.Update(pModel, axis_mouse_x, axis_mouse_y, axis_hands_x, axis_hands_y, pDt);

		axis_mouse_x *= m_AttachmentsModifier * m_RandomRecoilModifier;
		axis_mouse_y *= m_AttachmentsModifier * m_RandomRecoilModifier;
		
		axis_hands_x *= m_AttachmentsModifier * m_RandomRecoilModifier;
		axis_hands_y *= m_AttachmentsModifier * m_RandomRecoilModifier;
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

	void DbgPrintRecoilBase(string val)
	{
		return;
		Print("DbgPrintRecoilBase | "+val);
	}
}