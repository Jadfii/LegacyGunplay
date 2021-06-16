class DynamicCrossHair extends CrossHair
{
	protected vector m_Position;
	protected bool m_Visible = false;

	protected PlayerBase m_Player = NULL;
	protected Weapon_Base m_Weapon = NULL;

	protected float m_RaycastDistance = 50;

	Shape m_Shape_usti;
	Shape m_Shape_konec;
	Shape m_BarrelLine;
	Shape m_AimLine;
	Shape m_AimPoint;

	void GetPlayer()
	{
		Class.CastTo(m_Player, GetGame().GetPlayer());
	}

	void DrawBarrelMemoryPoints(vector begin_point, vector end_point)
	{
		if (m_Shape_usti) Debug.RemoveShape(m_Shape_usti);
		if (m_Shape_konec) Debug.RemoveShape(m_Shape_konec);

		m_Shape_usti = Debug.DrawSphere(end_point, 0.011, Colors.GREEN, ShapeFlags.TRANSP|ShapeFlags.NOOUTLINE|ShapeFlags.NOZBUFFER);
		m_Shape_konec = Debug.DrawSphere(begin_point, 0.011, Colors.GREEN, ShapeFlags.TRANSP|ShapeFlags.NOOUTLINE|ShapeFlags.NOZBUFFER);
	}

	void DrawBarrelAimLine(vector aim_point, vector end_point)
	{
		if (m_BarrelLine) Debug.RemoveShape(m_BarrelLine);
		m_BarrelLine = Debug.DrawLine(end_point, aim_point);
	}

	void DrawAimPoints(vector end_point, vector position)
	{
		if (m_AimLine) Debug.RemoveShape(m_AimLine);
		m_AimLine = Debug.DrawLine(end_point, position, Colors.RED, ShapeFlags.NOZBUFFER);

		if (m_AimPoint) Debug.RemoveShape(m_AimPoint);
		m_AimPoint = Debug.DrawSphere(position, 0.04, COLOR_RED);
	}

	bool GetAimPosition(out vector position)
	{
		vector usti_hlavne_position = m_Weapon.GetSelectionPositionLS("usti hlavne");//usti hlavne
		vector konec_hlavne_position = m_Weapon.GetSelectionPositionLS("konec hlavne");//konec hlavne
		vector usti_pos = m_Weapon.ModelToWorld(usti_hlavne_position);

		vector contact_dir;
		int contact_component;
		
		vector aim_point = usti_pos + vector.Direction(usti_pos, m_Weapon.ModelToWorld(usti_hlavne_position + (vector.Direction(konec_hlavne_position, usti_hlavne_position) * m_RaycastDistance))) * m_RaycastDistance;

		DrawBarrelMemoryPoints(konec_hlavne_position, usti_hlavne_position);
		DrawBarrelAimLine(aim_point, usti_pos);
		
		if (DayZPhysics.RaycastRV(usti_pos, aim_point, position, contact_dir, contact_component, NULL , NULL, m_Player, false, false, ObjIntersectIFire))
		{
			DrawAimPoints(usti_pos, position);
			return true;
		}
		return false;
	}

	void GetPosition()
	{
		m_Visible = false;
		ItemBase itemInHands = m_Player.GetItemInHands();
		if (itemInHands && itemInHands.IsWeapon())
		{
			if (Class.CastTo(m_Weapon, itemInHands))
			{
				m_Visible = GetAimPosition(m_Position);
			}
		}
	}

	void UpdatePosition()
	{
		if(!m_Player) GetPlayer();

		if (m_Player && m_Player.IsPlayerSelected() && !GetGame().IsInventoryOpen())
		{
			GetPosition();
			vector screenSpace = GetGame().GetScreenPos(m_Position);

			int screenW, screenH;
			GetScreenSize(screenW, screenH);

			// probably a better way to go about this, but oh well
			float cam_angle = 0;
			if (DayZPlayerCamera1stPerson.Cast(m_Player.GetCurrentPlayerCamera()))
			{
				cam_angle = DayZPlayerCamera1stPerson.Cast(m_Player.GetCurrentPlayerCamera()).GetLRAngle();
			}
			else if (DayZPlayerCamera3rdPerson.Cast(m_Player.GetCurrentPlayerCamera()))
			{
				cam_angle = DayZPlayerCamera3rdPerson.Cast(m_Player.GetCurrentPlayerCamera()).GetLRAngle();
			}

			DebugPrintDynamicCrossHair("cam_angle: " + cam_angle);

			// if the position is outside the bounds of the screen or the character is freelooking
			// too far, hide the crosshair
			if (screenSpace[0] <= 0 || screenSpace[0] >= screenW || screenSpace[1] <= 0 || screenSpace[1] >= screenH || Math.AbsFloat(cam_angle) >= 90)
			{
				m_Widget.Show(false);
				m_Position = vector.Zero;
				return;
			}

			DebugPrintDynamicCrossHair("m_Position: " + m_Position);

			float sx, sy;
			m_Widget.GetSize(sx, sy);

			// for some reason the HUD scaling uses the height for the width?
			float iconSizeW = screenH * sx;
			float iconSizeH = screenH * sy;

			screenSpace[0] = screenSpace[0] - (iconSizeW / 2);
			screenSpace[1] = screenSpace[1] - (iconSizeH / 2);

			//DebugPrintDynamicCrossHair("screenSpace: " + screenSpace);
			//DebugPrintDynamicCrossHair("iconSizeW: " + iconSizeW + ", iconSizeH: " + iconSizeH);
			//DebugPrintDynamicCrossHair("screenW: " + screenW + ", screenH: " + screenH);
			
			m_Widget.SetPos(screenSpace[0], screenSpace[1]);
			m_Widget.Show(true);
		}
		else
		{
			m_Widget.Show(false);
			m_Position = vector.Zero;
		}
	}

	void DebugPrintDynamicCrossHair(string val)
	{
		return;
		Print("DynamicCrossHair | " + val);
	}
}