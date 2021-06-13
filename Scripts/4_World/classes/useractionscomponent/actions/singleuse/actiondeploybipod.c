
class ActionDeployBipod extends ActionSingleUseBase
{
	AtlasBipod m_Bipod;

	void ActionDeployBipod()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHBARREL;
	}

	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override typename GetInputType()
	{
		return ContinuousDefaultActionInput;
	}

	override bool HasProgress()
	{
		return false;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "Deploy Bipod";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		return Class.CastTo(m_Bipod, item.FindAttachmentBySlotName("weaponBipod")) && !m_Bipod.IsDeployed();
	}

	override void OnExecuteServer(ActionData action_data)
	{
		if (!m_Bipod) return;
		m_Bipod.DeployBipod(true);
	}
}