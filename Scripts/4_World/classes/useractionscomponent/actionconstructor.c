modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);

		actions.Insert(LG_ActionDeployBipod);
		actions.Insert(LG_ActionRetractBipod);
	}
}