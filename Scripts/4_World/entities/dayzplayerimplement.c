modded class DayZPlayerImplement extends DayZPlayer
{
	protected bool m_IsMoving = false;
	protected bool m_IsUsingBipod = false;

	protected float m_MovementSpeedTimeout;

	void DayZPlayerImplement()
	{
		m_IsShootingFromCamera = false;

		/*m_MoveSettings = GetDayZPlayerType().CommandMoveSettingsW();
		m_MoveSettings.m_fRunSpringTimeout = 0.4;
		//m_MoveSettings.m_fDirFilterTimeout = 0.3;
		m_MoveSettings.m_fLeaningSpeed = 3;*/
	}

	bool IsSwayExhausted()
	{
		if (!GetAimingModel() || !GetAimingModel().GetCurrentSway()) return false;

		return GetAimingModel().GetCurrentSway().IsExhausted();
	}

	bool IsADS()
	{
		return m_CameraIronsight || m_CameraOptics;
	}

	bool IsUsingBipod()
	{
		return m_IsUsingBipod;
	}

	// The bipod should only be able to be used if the player is raised & prone & on front while prone
	bool CanUseBipod()
	{
		bool is_on_back = GetCommand_Move() && GetCommand_Move().IsOnBack();
		return IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_RAISEDPRONE) && !is_on_back;
	}

	override void HandleWeapons(float pDt, Entity pInHands, HumanInputController pInputs, out bool pExitIronSights)
	{
		super.HandleWeapons(pDt, pInHands, pInputs, pExitIronSights);

		Weapon_Base weapon;
		if (!Class.CastTo(weapon, pInHands)) return;

		AtlasBipod bipod;
		if (!Class.CastTo(bipod, weapon.GetBipodAttachment())) return;

		// At this point, we have a weapon with bipod attached in our hands.
		
		if (CanUseBipod() && bipod.IsDeployed())
		{
			m_IsUsingBipod = true;
		}
		else if (m_IsUsingBipod)
		{
			m_IsUsingBipod = false;
		}
	}

	override void HandleADS()
	{
		UAInput freelook_input = GetUApi().GetInputByName("UALookAround");
		if (freelook_input)
		{
			// Prevent freelook if ADS
			if (IsADS())
			{
				freelook_input.Lock();
			}
			else if (freelook_input.IsLocked())
			{
				freelook_input.Unlock();
			}
		}

		super.HandleADS();
	}

	override bool IsTryingHoldBreath()
	{
		return !IsSwayExhausted() && super.IsTryingHoldBreath();
	}

	override bool CanJump()
	{
		// Prevent jumping if gun is raised
		if (IsRaised()) return false;

		return super.CanJump();
	}

	void DbgPrintImplement(string val)
	{
		return;
		Print("DayZPlayerImplement | " + val);
	}
}