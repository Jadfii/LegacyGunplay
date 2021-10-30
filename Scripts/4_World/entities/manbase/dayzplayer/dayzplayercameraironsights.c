class PlayerCameraConstants
{
	static const float OPTICS_FOV_MODIFIER = 1.25;
	static const float HOLDBREATH_FOV_MODIFIER = 0.85;
}

modded class DayZPlayerCameraIronsights extends DayZPlayerCameraBase
{
	void DayZPlayerCameraIronsights(DayZPlayer pPlayer, HumanInputController pInput)
	{
		m_iBoneIndex = pPlayer.GetBoneIndexByName("RightHand_Dummy");
		if (m_iBoneIndex == -1)
		{
			Print("DayZPlayerCameraIronsights: main bone not found");
		}

		m_dynamicsStrength = 2;
		m_dynamicsSmoothTime = 0.3;
	}

	bool IsCurrentOptic2D()
	{
		ItemOptics optics = ItemOptics.Cast(GetCurrentSightEntity());

		if (!optics)
			return false;

		// Exception if optic is 2D but has the ability to access ironsights
		if (optics.HasWeaponIronsightsOverride() && optics.IsUsingWeaponIronsightsOverride())
			return false;

		if (GetGame().ConfigIsExisting("cfgVehicles " + optics.GetType() + " OpticsInfo useModelOptics"))
		{
			int is2D = GetGame().ConfigGetInt("cfgVehicles " + optics.GetType() + " OpticsInfo useModelOptics");
			
			return is2D == 1;
		}

		return false;
	}
	
	override float HoldBreathFOVEffect(float pDt)
	{
		if (m_pPlayer.IsHoldingBreath())
		{
			m_fFovAbsolute = Math.SmoothCD(m_fFovAbsolute, GameConstants.DZPLAYER_CAMERA_FOV_IRONSIGHTS * PlayerCameraConstants.HOLDBREATH_FOV_MODIFIER, m_fFovAbsVel, 0.3, 1000, pDt);
		}
		else if (m_isEntering) //sets FOV immediatelly to avoid "floating camera" effect
		{
			m_fFovAbsolute = GameConstants.DZPLAYER_CAMERA_FOV_IRONSIGHTS;
			m_isEntering = false;
		}
		else
		{
			m_fFovAbsolute = Math.SmoothCD(m_fFovAbsolute, GameConstants.DZPLAYER_CAMERA_FOV_IRONSIGHTS, m_fFovAbsVel, 0.2, 1000, pDt);
		}

		return m_fFovAbsolute;
	}

	override void SetCameraPP(bool state, DayZPlayerCamera launchedFrom)
	{
		PPEffects.ResetPPMask();
		PPEffects.SetLensEffect(0, 0, 0, 0);
		PPEffects.OverrideDOF(false, 0, 0, 0, 0, 1);
		PPEffects.SetBlurOptics(0);

		if (IsCameraNV())
		{
			SetNVPostprocess(NVTypes.NV_GOGGLES);
		}
		else
		{
			SetNVPostprocess(NVTypes.NONE);
		}

		if (m_weaponUsed)
		{
			m_weaponUsed.HideWeaponBarrel(false);
		}
	}

	override void AdjustCameraParameters(float pDt, inout DayZPlayerCameraResult pOutResult)
	{
		pOutResult.m_iDirectBone = m_iBoneIndex;
		pOutResult.m_iDirectBoneMode = 4;
	
		pOutResult.m_fFovAbsolute = HoldBreathFOVEffect(pDt);
		pOutResult.m_bUpdateWhenBlendOut = false;
		pOutResult.m_fDistance = 0;
		pOutResult.m_fUseHeading = 0;
		pOutResult.m_fInsideCamera = 1.0;
		pOutResult.m_fShootFromCamera = m_fShootFromCamera;
		pOutResult.m_fNearPlane = 0.04; //0.07 default
	}

	override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		//! update angles from input 
		float min;
		DayZPlayerImplement player = DayZPlayerImplement.Cast(GetGame().GetPlayer());

		bool is_raised_prone = player && player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_RAISEDPRONE);

		if (is_raised_prone)
			min = CONST_UD_MIN_BACK;
		else
			min = CONST_UD_MIN;
		
		float udAngle = UpdateUDAngle(m_fUpDownAngle, m_fUpDownAngleAdd, min, CONST_UD_MAX, pDt);
		m_CurrentCameraPitch = udAngle;
		m_fLeftRightAngle = UpdateLRAngle(m_fLeftRightAngle, CONST_LR_MIN, CONST_LR_MAX, pDt);

		// get model space transform matrix of the gun's eye vector 
		vector matTM[4];
		HumanItemAccessor hia = m_pPlayer.GetItemAccessor();
		if (GetCurrentSightEntity())
		{
			hia.WeaponGetCameraPointBoneRelative(GetCurrentSightEntity(), m_OpticsCamPos, m_OpticsCamDir, m_iBoneIndex, matTM);
		}

		vector aimChangeYPR;
		float aimChangeX = m_pInput.GetAimChange()[0] * Math.RAD2DEG;
		float aimChangeY = m_pInput.GetAimChange()[1] * Math.RAD2DEG;

		aimChangeYPR[0] = Math.SmoothCD(aimChangeYPR[0], -(m_dynamicsStrength * aimChangeY), m_velocityYaw, m_dynamicsSmoothTime, 1000, pDt);
		aimChangeYPR[1] = Math.SmoothCD(aimChangeYPR[1], -(m_dynamicsStrength * aimChangeX), m_velocityPitch, m_dynamicsSmoothTime, 1000, pDt);
		aimChangeYPR[2] = 0;

		vector dynamics[4];
		Math3D.YawPitchRollMatrix(aimChangeYPR, dynamics);
		dynamics[3] = vector.Zero;

		// If a 3D optic or ironsight, the camera should not follow the hands offset
		// of the gun - a "decouple" effect
		if (!IsCurrentOptic2D())
		{
			vector cameraTM_angles = Math3D.MatrixToAngles(pOutResult.m_CameraTM);

			cameraTM_angles[0] = cameraTM_angles[0] - player.GetAimingModel().m_CurrentHandsOffsetX;
			cameraTM_angles[1] = cameraTM_angles[1] - player.GetAimingModel().m_CurrentHandsOffsetY;
			
			Math3D.YawPitchRollMatrix(cameraTM_angles, pOutResult.m_CameraTM);
		}
		
		// aiming model offsets
		vector aimingMatTM[4];
		hia.WeaponGetAimingModelDirTm(aimingMatTM);
		
		// final offset matrix
		Math3D.MatrixMultiply4(dynamics, aimingMatTM, dynamics);
		Math3D.MatrixMultiply4(dynamics, matTM, matTM);
		Math3D.MatrixMultiply4(matTM, pOutResult.m_CameraTM, pOutResult.m_CameraTM);

		AdjustCameraParameters(pDt, pOutResult);
		UpdateBatteryOptics(GetCurrentSightEntity());
		UpdateCameraNV(PlayerBase.Cast(m_pPlayer));
		
		/*if (m_CameraShake)
		{
			float x, y;
			m_CameraShake.Update(pDt, x, y);
			DayZPlayerImplement.Cast(m_pPlayer).GetAimingModel().SetCamShakeValues(x, y);
		}*/
	}

	void DebugPrintCamera(string val)
	{
		return;
		Print("DebugPrintCamera | "+val);
	}
}

modded class DayZPlayerCameraOptics extends DayZPlayerCameraIronsights
{
	override void AdjustCameraParameters(float pDt, inout DayZPlayerCameraResult pOutResult)
	{
		pOutResult.m_iDirectBone = m_iBoneIndex;
		pOutResult.m_iDirectBoneMode = 4;
	
		pOutResult.m_fFovAbsolute = HoldBreathFOVEffect(pDt);
		pOutResult.m_bUpdateWhenBlendOut = false;
		pOutResult.m_fDistance = 0;
		pOutResult.m_fUseHeading = 0;
		pOutResult.m_fInsideCamera = 1.0;
		pOutResult.m_fShootFromCamera = m_fShootFromCamera;
		pOutResult.m_fNearPlane = 0.06; //0.07 default
	}
	
	override float HoldBreathFOVEffect(float pDt)
	{
		ItemOptics optics = ItemOptics.Cast(GetCurrentSightEntity());
		if (optics)
		{
			float opticsFOV = optics.GetCurrentStepFOV();
			//DebugPrintCamera("HoldBreathFOVEffect | opticsFOV: "+opticsFOV);
			opticsFOV *= PlayerCameraConstants.OPTICS_FOV_MODIFIER;

			if (optics.GetCurrentStepFOV() >= GameConstants.DZPLAYER_CAMERA_FOV_EYEZOOM && (m_pPlayer.IsHoldingBreath() || m_pPlayer.IsEyeZoom()))
			{
				m_fFovAbsolute = Math.SmoothCD(m_fFovAbsolute, opticsFOV * PlayerCameraConstants.HOLDBREATH_FOV_MODIFIER, m_fFovAbsVel, 0.3, 1000, pDt);
			}
			else if (m_isEntering) //sets FOV immediatelly to avoid "floating camera" effect
			{
				m_fFovAbsolute = opticsFOV;
				m_isEntering = false;
			}
			else
			{
				m_fFovAbsolute = Math.SmoothCD(m_fFovAbsolute, opticsFOV, m_fFovAbsVel, 0.2, 1000, pDt);
			}
			return m_fFovAbsolute;
		}
		else
		{
			return 0.6;
		}
	}

	override void SetCameraPP(bool state, DayZPlayerCamera launchedFrom)
	{
		PPEffects.ResetPPMask();
		PPEffects.SetLensEffect(0, 0, 0, 0);
		PPEffects.OverrideDOF(false, 0, 0, 0, 0, 1);
		PPEffects.SetBlurOptics(0);

		if (m_weaponUsed)
		{
			m_weaponUsed.HideWeaponBarrel(state && IsCurrentOptic2D());
		}

		if (!NVGoggles.Cast(m_opticsUsed) && m_opticsUsed && m_opticsUsed.IsNVOptic())
		{
			if (m_opticsUsed.IsWorking())
			{
				SetCameraNV(true);
				SetNVPostprocess(NVTypes.NV_OPTICS_ON);
			}
			else
			{
				SetCameraNV(false);
				SetNVPostprocess(NVTypes.NV_OPTICS_OFF);
			}
		}
		else
		{
			if (IsCameraNV())
			{
				SetNVPostprocess(NVTypes.NV_GOGGLES);
			}
			else
			{
				SetNVPostprocess(NVTypes.NONE);
			}
		}
	}
};

