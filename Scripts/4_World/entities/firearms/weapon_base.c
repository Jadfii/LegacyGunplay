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

	// Override default recoil object
	override RecoilBase SpawnRecoilObject()
	{
		return SpawnPredictedRecoilObject();
	}

	// This method will try and predict a recoil object for those lazy
	// modders than didn't give their weapons one
	protected RecoilBase SpawnPredictedRecoilObject()
	{
		if (IsInherited(Pistol_Base) || IsInherited(Magnum_Base))
			return new Cz75Recoil(this);

		if (IsInherited(BoltActionRifle_InnerMagazine_Base))
			return new Winchester70Recoil(this);
		
		if (IsInherited(BoltActionRifle_Base) || IsInherited(DoubleBarrel_Base))
			return new CZ527Recoil(this);

		string cartridge_type = GetChamberAmmoTypeName(GetCurrentMuzzle());

		if (cartridge_type.Contains("12ga"))
			return new Izh43Recoil(this);

		if (cartridge_type.Contains("762x54") || cartridge_type.Contains("308Win"))
			return new SvdRecoil(this);
		
		if (cartridge_type.Contains("Ammo_380"))
			return new Cz61Recoil(this);
		
		if (cartridge_type.Contains("9x39"))
			return new VSSRecoil(this);
		
		if (cartridge_type.Contains("556x45"))
			return new M4a1Recoil(this);

		if (cartridge_type.Contains("545x39"))
			return new Ak74Recoil(this);

		if (cartridge_type.Contains("762x39"))
			return new AkmRecoil(this);

		return new DefaultRecoil(this);
	}
}