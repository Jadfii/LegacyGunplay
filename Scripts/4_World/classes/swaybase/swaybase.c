class PlayerSwayStates
{
	static const int DEFAULT = 0;
	static const int CROUCHED = 1;
	static const int PRONE = 2;
	static const int STABLE = 3; // holding breath
	static const int EXHAUSTED = 4; // tired of holding breath :)))
}

modded class PlayerSwayConstants
{
	// This modifier is applied to the base sway before stamina effects
	static const float BASE_SWAY_MODIFIER = 1;

	static const float SWAY_MULTIPLIER_DEFAULT = 1;
	static const float SWAY_MULTIPLIER_CROUCHED = 0.75;
	static const float SWAY_MULTIPLIER_PRONE = 0.5;
	static const float SWAY_MULTIPLIER_STABLE = 0.2; // holding breath
	static const float SWAY_MULTIPLIER_EXHAUSTED = 2.0;
	static const float SWAY_MULTIPLIER_BIPOD = 0.1;

	static const float STAMINA_MULTIPLIER_SCALE = 2;

	static const float SWAY_STATE_TRANSITION_TIME_DEFAULT = 0.35;
	static const float SWAY_STATE_TRANSITION_TIME_EXHAUSTED = 3;
}

class SwayBase
{
	Weapon_Base m_Weapon;
	PlayerBase m_Player;
	protected bool m_DeleteRequested;
	protected bool m_IsClient;

	protected float m_StaminaNormalised;

	protected float m_Amplitude;
	protected float m_AttachmentsModifier;

	protected float m_Time;
	protected float m_ModifiedTime;

	protected float m_TransitionTime;
	protected bool m_IsTransitioningState;

	protected int m_SwayState;
	protected int m_LastSwayState;
	protected float m_SwayStateModifier;
	protected float m_SwayStateVelocity[1];

	void SwayBase(Weapon_Base weapon)
	{
		m_Weapon = weapon;
		m_Player = PlayerBase.Cast(weapon.GetHierarchyRootPlayer());
		m_IsClient = GetGame().IsClient() || !GetGame().IsMultiplayer();
		m_Amplitude = 0.5;

		m_AttachmentsModifier = GetAttachmentsModifier(weapon);

		m_SwayStateModifier = PlayerSwayConstants.SWAY_MULTIPLIER_DEFAULT;

		// Randomly init sway time to set a random pos in the sway curve
		m_ModifiedTime = Math.RandomFloat01() * 10;
	}

	void Destroy()
	{
		m_DeleteRequested = true;
	}

	void Update(out float offset_x, out float offset_y, float pDt)
	{
		if (m_DeleteRequested)
		{
			delete this;
		}
		
		//DbgPrintSwayBase("DbgPrintSwayBase | Player: "+m_Player+" | Update");

		m_StaminaNormalised = GetPlayerStamina();
		UpdateSwayState(pDt);

		float stamina_modifier = 1 + ((1 - m_StaminaNormalised) * PlayerSwayConstants.STAMINA_MULTIPLIER_SCALE);

		vector offset = GetSwayOffset(m_ModifiedTime);
		offset_x = offset[0] * m_AttachmentsModifier * GetBipodModifier() * stamina_modifier * PlayerSwayConstants.BASE_SWAY_MODIFIER;
		offset_y = offset[1] * m_AttachmentsModifier * GetBipodModifier() * stamina_modifier * PlayerSwayConstants.BASE_SWAY_MODIFIER;

		offset_x *= m_SwayStateModifier;
		offset_y *= m_SwayStateModifier;

		/*DbgPrintSwayBase("m_Time: "+m_Time);
		DbgPrintSwayBase("m_StaminaNormalised: "+m_StaminaNormalised);
		DbgPrintSwayBase("stamina_modifier: "+stamina_modifier);
		DbgPrintSwayBase("offset_x: "+offset_x);
		DbgPrintSwayBase("offset_x: "+offset_y);
		DbgPrintSwayBase("m_AttachmentsModifier: "+m_AttachmentsModifier);
		DbgPrintSwayBase("m_SwayStateModifier: "+m_SwayStateModifier);*/

		m_Time += pDt;
		m_ModifiedTime += pDt * m_SwayStateModifier;

		//DbgPrintSwayBase("-------------------");
	}

	int GetSwayState()
	{
		return m_SwayState;
	}

	bool IsExhausted()
	{
		return m_SwayState == PlayerSwayStates.EXHAUSTED || (m_IsTransitioningState && m_LastSwayState == PlayerSwayStates.EXHAUSTED);
	}

	void UpdateSwayState(float pDt)
	{
		int state = -1;

		if (!m_Player)
		{
			state = PlayerSwayStates.DEFAULT;
		}
		else if (m_Player.IsHoldingBreath())
		{
			state = PlayerSwayStates.STABLE;
		}
		else if (!m_Player.CanStartConsumingStamina(EStaminaConsumers.HOLD_BREATH))
		{
			state = PlayerSwayStates.EXHAUSTED;
		}
		else if (m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_RAISEDCROUCH))
		{
			state = PlayerSwayStates.CROUCHED;
		}
		else if (m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_RAISEDPRONE))
		{
			state = PlayerSwayStates.PRONE;
		}
		else
		{
			state = PlayerSwayStates.DEFAULT;
		}

		// State has changed, cause state change
		// We don't want to cause another state change if we're already transitioning
		if (state != m_SwayState && !m_IsTransitioningState)
		{
			m_SwayState = state;
			OnStartSwayStateChange(state);
		}

		TransitionSwayStateModifier(pDt);
	}

	void OnStartSwayStateChange(int state)
	{
		DbgPrintSwayBase("OnStartSwayStateChange");
		m_IsTransitioningState = true;
	}

	/* This should be called when the transition between states has finished */
	void OnFinishSwayStateChange()
	{
		DbgPrintSwayBase("OnFinishSwayStateChange");
		m_IsTransitioningState = false;
		m_LastSwayState = m_SwayState;

		if (m_SwayState == PlayerSwayStates.EXHAUSTED)
		{
			//m_Player.OnHoldBreathExhausted();
		}
	}

	void TransitionSwayStateModifier(float pDt)
	{
		if (m_SwayState == m_LastSwayState && m_TransitionTime <= 0) return;

		//DbgPrintSwayBase("m_LastSwayState: "+m_LastSwayState+" | m_SwayState: "+m_SwayState);

		float last_sway_state_transition_time = GetSwayStateTransitionTime(m_LastSwayState);

		float last_sway_state_modifier = GetSwayStateModifier(m_LastSwayState);
		float current_sway_state_modifier = GetSwayStateModifier(m_SwayState);

		DbgPrintSwayBase("last_sway_state_modifier: "+last_sway_state_modifier+" | current_sway_state_modifier: "+current_sway_state_modifier);
		DbgPrintSwayBase("last_sway_state_transition_time: "+last_sway_state_transition_time);

		// Timer has finished, reset and wait for next tick
		if (m_TransitionTime >= last_sway_state_transition_time)
		{
			OnFinishSwayStateChange();
			m_TransitionTime = 0;
			return;
		}

		// Modifier is at current state
		if (m_SwayStateModifier == current_sway_state_modifier)
		{
			m_TransitionTime = 0;
			return;
		}

		float time = Math.InverseLerp(0, last_sway_state_transition_time, m_TransitionTime);
		if (m_LastSwayState != PlayerSwayStates.EXHAUSTED) time = Easing.EaseInOutCubic(time);

		// Lerp modifier between modifier of last state and modifier of new state
		m_SwayStateModifier = Math.Lerp(last_sway_state_modifier, current_sway_state_modifier, Math.Clamp(time, 0, 1));

		m_TransitionTime += pDt;

		DbgPrintSwayBase("m_SwayStateModifier: "+m_SwayStateModifier+" | m_TransitionTime: "+m_TransitionTime);
	}

	float GetSwayStateTransitionTime(int state)
	{
		switch (state)
		{
			case PlayerSwayStates.EXHAUSTED:
				return PlayerSwayConstants.SWAY_STATE_TRANSITION_TIME_EXHAUSTED;
			default:
				return PlayerSwayConstants.SWAY_STATE_TRANSITION_TIME_DEFAULT;
		}

		return PlayerSwayConstants.SWAY_STATE_TRANSITION_TIME_DEFAULT;
	}

	bool IsTransitioningState()
	{
		return m_IsTransitioningState;
	}

	float GetSwayStateModifier(int state)
	{
		float modifier = PlayerSwayConstants.SWAY_MULTIPLIER_DEFAULT;
		
		switch (state)
		{
			case PlayerSwayStates.STABLE:
				modifier = PlayerSwayConstants.SWAY_MULTIPLIER_STABLE;
				break;
			case PlayerSwayStates.EXHAUSTED:
				modifier = PlayerSwayConstants.SWAY_MULTIPLIER_EXHAUSTED;
				break;
			case PlayerSwayStates.CROUCHED:
				modifier = PlayerSwayConstants.SWAY_MULTIPLIER_CROUCHED;
				break;
			case PlayerSwayStates.PRONE:
				modifier = PlayerSwayConstants.SWAY_MULTIPLIER_PRONE;
				break;
			default:
				modifier = PlayerSwayConstants.SWAY_MULTIPLIER_DEFAULT;
				break;
		}

		return modifier;
	}

	private vector GetSwayOffset(float time)
	{
		return Vector(GetX(time), GetY(time), 0);
	}

	/** 
	 * Sway follows a Lissajous curve in the ratio 4:3
	 * It is not random. That's how 0.62 was
	 * m_Amplitude sets the size of the curve
	 * 
	*/
	private float GetX(float time)
	{
		return Math.Sin(time * 2.2) * m_Amplitude;
	}

	private float GetY(float time)
	{
		return Math.Sin(time * 1.65) * m_Amplitude;
	}

	float GetBipodModifier()
	{
		float modifier = 1;

		if (m_Player && m_Player.IsUsingBipod())
		{
			modifier = PlayerSwayConstants.SWAY_MULTIPLIER_BIPOD;
		}

		return modifier;
	}

	float GetAttachmentsModifier(Weapon_Base weapon)
	{
		float modifier = 1;
		
		if (weapon)
		{
			modifier = weapon.GetPropertyModifierObject().m_SwayModifier;
		}

		return modifier;
	}

	float GetPlayerStamina()
	{
		float stamina_cap = m_Player.GetStaminaHandler().GetStaminaCap();
		float stamina = m_Player.GetStaminaHandler().GetSyncedStamina();
		if (!GetGame().IsMultiplayer()) // offline mode
			stamina = m_Player.GetStaminaHandler().GetStamina();

		return Math.InverseLerp(0, stamina_cap, stamina);
	}

	void DbgPrintSwayBase(string val)
	{
		return;
		Print("DbgPrintSwayBase | " + val);
	}
}