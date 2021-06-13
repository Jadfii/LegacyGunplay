modded class Weapon_Base extends Weapon
{
	float GetWeaponLength()
	{
		return m_WeaponLength;
	}

	bool HasBipodAttached()
	{
		AtlasBipod bipod;
		return Class.CastTo(bipod, FindAttachmentBySlotName("weaponBipod"));
	}

	bool HasBipodDeployed()
	{
		return GetBipodAttachment() && GetBipodAttachment().IsDeployed();
	}

	AtlasBipod GetBipodAttachment()
	{
		return FindAttachmentBySlotName("weaponBipod");
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionDeployBipod);
		AddAction(ActionRetractBipod);
	}

	override bool LiftWeaponCheck(PlayerBase player)
	{
		// ignore if bipod is deployed
		if (player && player.IsUsingBipod())
		{
			m_LiftWeapon = false;
			return false;
		}

		return super.LiftWeaponCheck(player);
	}

	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		super.OnItemLocationChanged(old_owner, new_owner);

		// Retract bipod if removed from hands
		if (HasBipodDeployed())
		{
			GetBipodAttachment().DeployBipod(false);
		}
	}
}