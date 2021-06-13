class ActionRetractBipod extends ActionDeployBipod
{
	override string GetText()
	{
		return "Retract Bipod";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		return Class.CastTo(m_Bipod, item.FindAttachmentBySlotName("weaponBipod")) && m_Bipod.IsDeployed();
	}

	override void OnExecuteServer(ActionData action_data)
	{
		if (!m_Bipod) return;
		m_Bipod.DeployBipod(false);
	}
}