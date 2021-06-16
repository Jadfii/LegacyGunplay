modded class PropertyModifiers
{
	bool m_HasButtstockAttachment;
	bool m_HasHandguardAttachment;
	bool m_HasMuzzleAttachment;

	float m_RecoilModifier = 1;
	float m_InertiaModifier = 1;
	float m_SwayModifier = 1;

	float m_BarrelLength = 1;
	float m_Weight = 1;

	ref array<string> m_ButtstockSlots = {
		"weaponButtstockAK",
		"weaponButtstockFal",
		"weaponButtstockM4",
		"weaponButtstockMP5",
		"weaponButtstockSaiga"
	};
	ref array<string> m_HandguardSlots = {
		"weaponHandguardAK",
		"weaponHandguardM4",
		"weaponHandguardMP5"
	};
	ref array<string> m_MuzzleSlots = {
		"weaponMuzzle",
		"suppressorImpro",
		"pistolMuzzle",
		"weaponMuzzleMosin",
		"weaponMuzzleM4",
		"weaponMuzzleAK",
		"weaponMuzzleMP5"
	};

	float GetBarrelLength()
	{
		float length = 1;

		Weapon_Base weapon;

		if (Class.CastTo(weapon, m_OwnerItem))
		{
			length = weapon.GetWeaponLength();

			ItemBase attachment;
			if (ItemBase.CastTo(attachment, weapon.FindAttachmentBySlotName("weaponBayonet")) || ItemBase.CastTo(attachment, weapon.FindAttachmentBySlotName("weaponBayonetAK")) || ItemBase.CastTo(attachment, weapon.FindAttachmentBySlotName("weaponBayonetMosin")) || ItemBase.CastTo(attachment, weapon.FindAttachmentBySlotName("weaponBayonetSKS")) || ItemBase.CastTo(attachment, weapon.GetAttachedSuppressor()))
			{
				length += attachment.m_ItemModelLength;
			}
		}

		return length;
	}

	// returns weight of item in kg
	float GetWeight()
	{
		float modifier = 1;
		// GetWeight returns weight in grams
		if (m_OwnerItem)
			modifier = m_OwnerItem.GetWeight() / 1000;
		return modifier;
	}

	static float GetFloatModifier(ItemBase item, string modifier_name)
	{
		if (item.ConfigIsExisting(modifier_name))
			return item.ConfigGetFloat(modifier_name);

		return 1;
	}

	// This method is horrid, but it calculates our attachment modifiers.
	// Too lazy to make the logic better. It works
	override void UpdateModifiers()
	{
		super.UpdateModifiers();
		ItemBase attachment;

		m_BarrelLength = GetBarrelLength();
		m_Weight = GetWeight();

		// Reset modifiers
		m_RecoilModifiers = Vector(1, 1, 1);
		m_RecoilModifier = GetFloatModifier(m_OwnerItem, "recoilMultiplier");
		m_InertiaModifier = GetFloatModifier(m_OwnerItem, "inertiaMultiplier");
		m_SwayModifier = GetFloatModifier(m_OwnerItem, "swayMultiplier");
		m_HasButtstockAttachment = false;
		m_HasHandguardAttachment = false;
		m_HasMuzzleAttachment = false;

		bool has_buttstock_slot = false;
		bool has_handguard_slot = false;

		// loop through all attachment slots
		for (int i = 0; i < m_OwnerItem.GetInventory().GetAttachmentSlotsCount(); i++)
		{
			int slotId = m_OwnerItem.GetInventory().GetAttachmentSlotId(i);
			string slot_name = InventorySlots.GetSlotName(slotId);

			// here we are checking our list of slots to see:
			// 1) does the slot exist on the weapon?
			// 2) does the weapon have an item on that slot?
			if (m_ButtstockSlots && m_ButtstockSlots.Count() > 0 && m_ButtstockSlots.Find(slot_name) > -1)
			{
				has_buttstock_slot = true;
				if (Class.CastTo(attachment, m_OwnerItem.GetInventory().FindAttachment(slotId)))
				{
					m_HasButtstockAttachment = true;
				}
			}
			if (m_HandguardSlots && m_HandguardSlots.Count() > 0 && m_HandguardSlots.Find(slot_name) > -1)
			{
				has_handguard_slot = true;
				if (Class.CastTo(attachment, m_OwnerItem.GetInventory().FindAttachment(slotId)))
				{
					m_HasHandguardAttachment = true;
				}
			}
			if (m_MuzzleSlots && m_MuzzleSlots.Count() > 0 && m_MuzzleSlots.Find(slot_name) > -1 && Class.CastTo(attachment, m_OwnerItem.GetInventory().FindAttachment(slotId)))
			{
				m_HasMuzzleAttachment = true;
			}

			// find attachment in slot, continue if no item attached
			if (!Class.CastTo(attachment, m_OwnerItem.GetInventory().FindAttachment(slotId))) continue;

			// apply modifier
			m_RecoilModifier *= PropertyModifiers.GetFloatModifier(attachment, "recoilMultiplier");
			m_InertiaModifier *= PropertyModifiers.GetFloatModifier(attachment, "inertiaMultiplier");
			m_SwayModifier *= PropertyModifiers.GetFloatModifier(attachment, "swayMultiplier");
		}

		// true if attachment was found, or no slot was found
		// false if attach not found, but has slot
		m_HasButtstockAttachment = m_HasButtstockAttachment || !has_buttstock_slot;
		m_HasHandguardAttachment = m_HasHandguardAttachment || !has_handguard_slot;

		if (!m_HasButtstockAttachment)
		{
			m_InertiaModifier *= 3;
			m_RecoilModifier *= 2;
		}
		if (!m_HasHandguardAttachment)
		{
			m_InertiaModifier *= 2;
			m_RecoilModifier *= 1.3;
		}

		// apply modifier(s) for weapon type
		if (m_OwnerItem.IsInherited(Pistol_Base))
		{
			m_InertiaModifier *= 0.4;
		}
		else if (m_OwnerItem.IsInherited(Rifle_Base))
		{
			m_InertiaModifier *= 1;
		}
	}

	void DebugPropertyModifiers(string val)
	{
		return;
		Print("PropertyModifiers | "+val);
	}
}