modded class DayZPlayerImplement extends DayZPlayer
{
	protected bool m_IsForcedWalking = false;
	protected bool m_IsMoving = false;
	protected bool m_IsUsingBipod = false;

	protected float m_MovementSpeedTimeout;

	void DayZPlayerImplement()
	{
		m_IsShootingFromCamera = false;

		m_MoveSettings = GetDayZPlayerType().CommandMoveSettingsW();
		m_MoveSettings.m_fRunSpringTimeout = 0.4;
		/*m_MoveSettings.m_fDirFilterTimeout = 0.3;*/
		m_MoveSettings.m_fLeaningSpeed = 3;
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

	bool IsInventoryMenuOpen()
	{
		if (GetGame().IsClient())
		{
			UIScriptedMenu menu = GetGame().GetUIManager().GetMenu();
			return menu && (menu.GetID() == MENU_INVENTORY || menu.GetID() == MENU_INSPECT);
		}

		return false;
	}

	void ForceWalkMask(bool shouldWalk)
	{
		// If holding a movement key and ADS, we want to force walk
		if (shouldWalk)
		{
			m_IsForcedWalking = true;
			GetUApi().GetInputByName("UAWalkRunTemp").ForceEnable(true);
		}
		else if (m_IsForcedWalking && !IsInventoryMenuOpen())
		{
			m_IsForcedWalking = false;
			GetUApi().GetInputByName("UAWalkRunTemp").ForceEnable(false);
		}
	}

	override bool ModCommandHandlerAfter(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		// Check if should force walking
		// This is for when player is: ADS, leaning

		//bool should_walk = IsADS() || m_MovementState.IsLeaning();
		bool should_walk = IsADS();
		
		ForceWalkMask(should_walk);

		//ApplyMovementInertia(pDt);

		return super.ModCommandHandlerAfter(pDt, pCurrentCommandID, pCurrentCommandFinished);
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

	/*void ApplyMovementInertia(float pDt)
	{
		HumanCommandMove hcm = GetCommand_Move();
		if (!hcm) return;

		float current_movement_speed = hcm.GetCurrentMovementSpeed();
		float current_movement_angle;

		bool isMoving = hcm.GetCurrentInputAngle(current_movement_angle);
		bool isMovingForward = current_movement_angle == 0;

		bool isSprinting = current_movement_speed >= DayZPlayerConstants.MOVEMENTIDX_SPRINT - 0.1;
		bool isSprintingForward = isSprinting && isMovingForward;

		// Finish timer, unlock sprint
		if (m_MovementSpeedTimeout >= 0.3)
		{
			DbgPrintImplement("finish movement speed timer, unlock sprint");

			m_MovementSpeedTimeout = 0;
			GetUApi().GetInputByName("UATurbo").Unlock();
		}

		// If timer is initiated, run the timer by adding delta time
		if (m_MovementSpeedTimeout > 0)
		{
			m_MovementSpeedTimeout += pDt;

			DbgPrintImplement("isSprintingForward: "+isSprintingForward);
			DbgPrintImplement("current_movement_speed: "+current_movement_speed);
			DbgPrintImplement("m_MovementSpeedTimeout: "+m_MovementSpeedTimeout);
		}

		// Init timer if player has started sprinting forward from idle
		// Lock sprint when timer is initiated so that the player is forced to run
		if (isSprintingForward && isMoving != m_IsMoving)
		{
			DbgPrintImplement("init movement speed timer, lock sprint");

			m_MovementSpeedTimeout += pDt;
			GetUApi().GetInputByName("UATurbo").Lock();
		}
		
		m_IsMoving = isMoving;
	}*/
}