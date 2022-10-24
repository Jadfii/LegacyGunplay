modded class DayZPlayerImplementAiming
{
	float m_CurrentHandsOffsetX;
	float m_CurrentHandsOffsetY;

	protected float m_movementTimeAcc = 0;
	protected float m_movementAmplitudeX = 0.6;
	protected float m_movementAmplitudeY = 0.25;
	protected float m_movementAmplitudeZ = 0.012;
	protected float m_movementFrequencyWalk = 1.3;
	protected float m_movementFrequencySlow = 0.55;
	protected float m_movementFrequencyCrouched = 0.85;

	float m_CurrentMovementNoise;
	float m_MovementOffsetTimer;

	protected float m_RecoilOffsetDirection = -1;

	protected ref SwayBase m_CurrentSway;
	protected ref InertiaBase m_CurrentInertia;

	void DayZPlayerImplementAiming(DayZPlayerImplement player)
	{
	}

	SwayBase GetCurrentSway()
	{
		return m_CurrentSway;
	}

	Weapon GetWeaponInHands()
	{
		Weapon weapon_in_hands;
		if (m_PlayerPb && m_PlayerPb.GetItemInHands()) Class.CastTo(weapon_in_hands, m_PlayerPb.GetItemInHands());
		
		return weapon_in_hands;
	}

	float GetRecoilOffsetDirection()
	{
		return m_RecoilOffsetDirection;
	}

	float GenerateRandomOffsetDirection()
	{
		if (!m_PlayerPb) return -1;
		return Math.Round(m_PlayerPb.GetRandomGeneratorSyncManager().GetRandomInRange(RandomGeneratorSyncUsage.RGSAimingModel, 0, 1)) * 2 - 1;
	}

	override void OnRaiseBegin(DayZPlayerImplement player)
	{
		super.OnRaiseBegin(player);

		Weapon_Base weapon = GetWeaponInHands();
		if (weapon)
		{
			m_CurrentSway = new SwayBase(weapon);
			m_CurrentInertia = new InertiaBase(weapon);
		}
	}

	protected void ApplyMovementNoise(out float x_axis, out float y_axis, out float z_axis, float pDt)
	{
		HumanCommandMove hcm = m_PlayerPb.GetCommand_Move();
		if (hcm)
		{
			float speed = hcm.GetCurrentMovementSpeed();
			
			// If moving, increment the accumulator
			if (speed > 0)
			{
				m_movementTimeAcc += pDt;
			}
			else
			{
				m_movementTimeAcc = 0;
			}

			float frequencyModifier;
			if (m_PlayerPb.m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH)
				frequencyModifier = m_movementFrequencyCrouched;
			else if (speed > 1)
				frequencyModifier = m_movementFrequencyWalk;
			else
				frequencyModifier = m_movementFrequencySlow;

			float movement_noise = speed * Math.Sin(Math.PI2 * frequencyModifier * m_movementTimeAcc);

			// If we've stopped moving in this tick, kick off a timer
			// to lerp the offset towards 0
			if (speed == 0 && Math.AbsFloat(m_CurrentMovementNoise) > 0.01)
			{
				// Speed up the cooldown
				m_MovementOffsetTimer += pDt * 5;

				movement_noise = Math.Lerp(m_CurrentMovementNoise, 0, m_MovementOffsetTimer);
				// If finished, reset the timer and set the prev movement offset back to 0
				if (m_MovementOffsetTimer >= 1)
				{
					m_MovementOffsetTimer = 0;
					m_CurrentMovementNoise = 0;
				}
			}
			else
			{
				m_CurrentMovementNoise = movement_noise;
			}

			// Apply the movement offset per axis
			x_axis += m_movementAmplitudeX * movement_noise;
			y_axis += m_movementAmplitudeY * movement_noise;
			z_axis += m_movementAmplitudeZ * movement_noise;
		}
	}

	override bool ProcessAimFilters(float pDt, SDayZPlayerAimingModel pModel, int stance_index)
	{
		//return super.ProcessAimFilters(pDt, pModel, stance_index);

		float breathing_offset_x;
		float breathing_offset_y;
		
		float noise_offset_x;
		float noise_offset_y;		

		float movement_offset_x;
		float movement_offset_y;
		float movement_offset_z;
		
		float shake_offset_x;
		float shake_offset_y;
		
		float recoil_offset_mouse_x;
		float recoil_offset_mouse_y;
		
		float recoil_offset_hands_x;
		float recoil_offset_hands_y;
		
		float kuru_offset_x;
		float kuru_offset_y;
		
		DbgPrintAimingImplement("Player: " + m_PlayerPb + " | ProcessAimFilters | timestamp: " + m_PlayerPb.GetSimulationTimeStamp());
		
		ApplyMovementNoise(movement_offset_x, movement_offset_y, movement_offset_z, pDt);

		pModel.m_fCamPosOffsetX = movement_offset_x * 0.004;
		pModel.m_fCamPosOffsetZ = movement_offset_z;
		
		int shake_level = m_PlayerPb.GetShakeLevel();
		if (shake_level != 0)
		{
			ApplyShakes(shake_offset_x, shake_offset_y, shake_level);
		}

		//! get recoil
		if (m_CurrentRecoil)
		{
			m_CurrentRecoil.Update(pModel, recoil_offset_mouse_x, recoil_offset_mouse_y, recoil_offset_hands_x, recoil_offset_hands_y, pDt);
		}
		else
		{
			m_RecoilOffsetDirection = GenerateRandomOffsetDirection();
		}

		//DbgPrintAimingImplement("pModel.m_fCamPosOffsetZ: " + pModel.m_fCamPosOffsetZ);

		//DbgPrintAimingImplement("recoil_offset_hands_x: " + recoil_offset_hands_x);
		//DbgPrintAimingImplement("recoil_offset_hands_y: " + recoil_offset_hands_y);

		if (m_CurrentSway)
		{
			m_CurrentSway.Update(noise_offset_x, noise_offset_y, pDt);
		}
		
		if (m_KuruShake)
		{
			m_KuruShake.Update(pDt, kuru_offset_x, kuru_offset_y);
		}
		
		//! hands offset
		float hands_offset_x = breathing_offset_x + noise_offset_x + recoil_offset_hands_x + (shake_offset_x * 0.4) + kuru_offset_x + movement_offset_x;
		float hands_offset_y = breathing_offset_y + noise_offset_y + recoil_offset_hands_y + (shake_offset_y * 0.4) + kuru_offset_y + movement_offset_y;

		float final_hands_offset_x = hands_offset_x;
		float final_hands_offset_y = hands_offset_y;

		//! inertia
		// once all the offsets are done, apply the inertia
		if (m_CurrentInertia)
		{
			m_CurrentInertia.Update(pDt, pModel, hands_offset_x, hands_offset_y, final_hands_offset_x, final_hands_offset_y);
		}

		pModel.m_fAimXHandsOffset = final_hands_offset_x;
		pModel.m_fAimYHandsOffset = final_hands_offset_y;
		m_CurrentHandsOffsetX = final_hands_offset_x;
		m_CurrentHandsOffsetY = final_hands_offset_y;

		DbgPrintAimingImplement("m_fAimYHandsOffset: " + pModel.m_fAimYHandsOffset);
		DbgPrintAimingImplement("m_fCurrentAimY: " + pModel.m_fCurrentAimY);
		DbgPrintAimingImplement("aimChangeY: " + m_PlayerPb.GetInputController().GetAimChange()[1]);
		DbgPrintAimingImplement("IsUsingBipod: " + m_PlayerPb.IsUsingBipod());

		//! cam offset
		float cam_offset_x = -kuru_offset_x + m_CamShakeX;
		float cam_offset_y = -kuru_offset_y + m_CamShakeY;

		pModel.m_fAimXCamOffset = cam_offset_x;
		pModel.m_fAimYCamOffset	= cam_offset_y;
		
		//! clamp aim ranges
		if (stance_index == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
		{
			float newVal = DayZPlayerUtils.LinearRangeClamp(pModel.m_fCurrentAimX, pModel.m_fCurrentAimY, m_AimXClampRanges);
			pModel.m_fAimYHandsOffset += newVal - pModel.m_fCurrentAimY;
		}
		
		//! mouse offset
		pModel.m_fAimXMouseShift = recoil_offset_mouse_x -kuru_offset_x / 10;
		pModel.m_fAimYMouseShift = recoil_offset_mouse_y + kuru_offset_y / 10;
	
		DbgPrintAimingImplement("----------------------------");
		return true;
	}

	override void DbgPrintAimingImplement(string val)
	{
		return;
		Print("DayZPlayerImplementAiming | " + val);
	}
}