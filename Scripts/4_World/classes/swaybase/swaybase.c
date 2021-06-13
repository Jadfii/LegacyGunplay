modded class PlayerSwayConstants
{
	static const float SWAY_MULTIPLIER_DEFAULT = 0.6;
	static const float SWAY_MULTIPLIER_STABLE = 0.2; // holding breath
	static const float SWAY_MULTIPLIER_EXHAUSTED = 2.0;
	static const float SWAY_MULTIPLIER_BIPOD = 0.1;
}

class SwayBase
{
	Weapon_Base m_Weapon;
	PlayerBase m_Player;
	protected bool m_DeleteRequested;
	protected bool m_IsClient;

	protected float m_Amplitude;
	protected float m_AttachmentsModifier;
	protected vector m_SwayModifier = "1 1 1";//"max_speed_noise radius_noise overall_speed";

	float m_Time;
	float m_ModifiedTime;
	float m_HoldBreathTime;
	float m_ExhaustedTime;

	float m_SwayStateModifier;
	float m_SwayStateChangeTime;

	void SwayBase(Weapon_Base weapon)
	{
		m_Weapon = weapon;
		m_Player = PlayerBase.Cast(weapon.GetHierarchyRootPlayer());
		m_IsClient = GetGame().IsClient() || !GetGame().IsMultiplayer();
		m_SwayModifier = GetWeaponSwayModifier(weapon);
		m_Amplitude = (1 - 1 / m_SwayModifier[1]) * 1.2;

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
		
		DbgPrintSwayBase("DbgPrintSwayBase | Player: "+m_Player+" | Update");

		//UpdateSwayState(pDt);

		vector offset = GetSwayOffset(m_ModifiedTime);
		offset_x = offset[0] * m_AttachmentsModifier * GetBipodModifier();
		offset_y = offset[1] * m_AttachmentsModifier * GetBipodModifier();

		offset_x *= GetSwayStateModifier();
		offset_y *= GetSwayStateModifier();

		/*DbgPrintSwayBase("offset_x: "+offset_x);
		DbgPrintSwayBase("offset_x: "+offset_y);
		DbgPrintSwayBase("m_SwayModifier: "+m_SwayModifier);
		DbgPrintSwayBase("m_AttachmentsModifier: "+m_AttachmentsModifier);
		DbgPrintSwayBase("m_SwayStateModifier: "+m_SwayStateModifier);*/

		m_Time += pDt;

		float modifier = 1;
		if (m_Player.IsHoldingBreath())
			modifier = PlayerSwayConstants.SWAY_MULTIPLIER_STABLE;
		
		m_ModifiedTime += pDt * modifier;

		DbgPrintSwayBase("-------------------");
	}

	/** 
	 * Update the sway state (should be ran every tick)
	 * This will handle smoothing between sway states
	 * 
	*/
	void UpdateSwayState(float pDt)
	{
		// TODO
		// THE BELOW IS DOGSHIT AND DOES NOT WORK YET
		return;

		float modifier = PlayerSwayConstants.SWAY_MULTIPLIER_DEFAULT;

		// If holding breath, change modifier and start timer
		if (m_Player && m_Player.IsHoldingBreath())
		{
			modifier = PlayerSwayConstants.SWAY_MULTIPLIER_STABLE;
			m_HoldBreathTime += pDt;
		}
		else
		{
			m_HoldBreathTime = 0;
		}

		// If holding breath for too long, cause exhausted event on player and start 
		// timer for exhaustion
		if (m_HoldBreathTime >= (PlayerSwayConstants.SWAY_TIME_IN + PlayerSwayConstants.SWAY_TIME_STABLE + PlayerSwayConstants.SWAY_TIME_EXHAUSTED))
		{
			m_Player.OnHoldBreathExhausted();
			m_ExhaustedTime = 0.01;
		}

		// If exhaustion timer running
		if (m_ExhaustedTime > 0)
		{
			// Increment timer
			m_ExhaustedTime += pDt * 0.5;

			// Reset timer once completed
			if (m_ExhaustedTime >= 1)
			{
				m_ExhaustedTime = 0;
			}
			else
			{
				// Enact on the exhaustion timer by lerping from exhausted state towards default
				modifier = PlayerSwayConstants.SWAY_MULTIPLIER_EXHAUSTED;
				m_SwayStateModifier = Math.Lerp(modifier, PlayerSwayConstants.SWAY_MULTIPLIER_DEFAULT, Easing.EaseInCubic(m_ExhaustedTime));
				return;
			}
		}

		m_SwayStateModifier = modifier;
	}

	float GetSwayStateModifier()
	{
		return m_SwayStateModifier;
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

	vector GetWeaponSwayModifier(Weapon_Base weapon)
	{
		if (weapon) return weapon.GetPropertyModifierObject().m_SwayModifiers;
		else return "1 1 1";
	}

	void DbgPrintSwayBase(string val)
	{
		return;
		Print("DbgPrintSwayBase | " + val);
	}
}