modded class PlayerBase extends ManBase
{
	ref Param2<string, bool> m_UserStatusParamMessage;

	override void Init()
	{
		super.Init();

		m_UserStatusParamMessage = new Param2<string, bool>("", false);
	}

	void Message(string text, string style)
	{
		super.Message(text, style);

		if (!GetGame().IsMultiplayer())
		{
			ChatMessageEventParams chat_params = new ChatMessageEventParams(CCDirect, "", text, style);
			if (GetGame().GetMission())
			{
				GetGame().GetMission().OnEvent(ChatMessageEventTypeID, chat_params);
			}
		}
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		switch (rpc_type)
		{
			case ERPCsModded.RPC_USER_STATUS_MESSAGE:
				if (!GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT) 
				{
					break;
				}

				if (ctx.Read(m_UserStatusParamMessage))
				{
					string statustext = m_UserStatusParamMessage.param1;
					bool important = m_UserStatusParamMessage.param2;

					if (important) MessageImportant(statustext);
					else MessageStatus(statustext);
				}
				break;
		}
	}

	override void HideClothing(ItemOptics optic, bool state)
	{
		super.HideClothing(optic, state);
		float default_fov = 30 * Math.DEG2RAD;
		array<int> clothingArray = new array<int>;

		// I don't want to NOT call super, so we're just going to re-enable
		// some clothing pieces if they shouldn't be invisible
		if (state && DayZPlayerCameraOptics.Cast(m_CurrentCamera) && GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT && optic.GetCurrentStepFOV() > default_fov)
		{
			clothingArray.Insert(InventorySlots.BODY);
			SetInvisibleRecursive(false, this, clothingArray);
		}
	}
}