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
				// Put a cheeky modifier on the attachment length
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

	float GetInertiaModifier(ItemBase attachment)
	{
		float modifier = 1;

		Magazine magazine;
		if (Class.CastTo(magazine, attachment))
		{
			string magazine_type = magazine.GetType();
			magazine_type.ToLower();

			if (magazine_type == "Mag_PP19_64Rnd")
			{
				return modifier;
			}

			if (magazine_type.Contains("drum"))
			{
				modifier *= 1.6;
			}

			if (magazine_type.Contains("couple"))
			{
				modifier *= 1.25;
			}

			int magazine_size = magazine.GetAmmoMax();
			DebugPropertyModifiers("magazine_size: "+magazine_size);

			modifier *= Math.Clamp(magazine_size * 0.035, 1, 5);
		}
		else
		{
			modifier *= PropertyModifiers.GetFloatModifier(attachment, "inertiaMultiplier");
		}

		DebugPropertyModifiers("attachment: "+attachment);
		DebugPropertyModifiers("GetInertiaModifier: "+modifier);

		return modifier;
	}

	static float GetFloatModifier(ItemBase item, string modifier_name)
	{
		if (item.ConfigIsExisting(modifier_name))
			return item.ConfigGetFloat(modifier_name);

		return 1;
	}

	static float GetVectorModifier(ItemBase item, string modifier_name)
	{
		if (item.ConfigIsExisting(modifier_name))
			return item.ConfigGetVector(modifier_name)[0];

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
		m_RecoilModifier = GetVectorModifier(m_OwnerItem, "recoilModifier");
		m_InertiaModifier = GetFloatModifier(m_OwnerItem, "inertiaMultiplier");
		m_SwayModifier = 1;
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
			slot_name.ToLower();

			// here we are checking our list of slots to see:
			// 1) does the slot exist on the weapon?
			// 2) does the weapon have an item on that slot?
			if (slot_name.Contains("buttstock"))
			{
				has_buttstock_slot = true;
				if (Class.CastTo(attachment, m_OwnerItem.GetInventory().FindAttachment(slotId)))
				{
					m_HasButtstockAttachment = true;
				}
			}
			if (slot_name.Contains("handguard"))
			{
				has_handguard_slot = true;
				if (Class.CastTo(attachment, m_OwnerItem.GetInventory().FindAttachment(slotId)))
				{
					m_HasHandguardAttachment = true;
				}
			}
			if ((slot_name.Contains("muzzle") || slot_name.Contains("suppressor")) && Class.CastTo(attachment, m_OwnerItem.GetInventory().FindAttachment(slotId)))
			{
				m_HasMuzzleAttachment = true;
			}

			// find attachment in slot, continue if no item attached
			if (!Class.CastTo(attachment, m_OwnerItem.GetInventory().FindAttachment(slotId))) continue;

			float attachment_inertia_modifier = GetInertiaModifier(attachment);

			float attachment_recoil_modifier = PropertyModifiers.GetVectorModifier(attachment, "recoilModifier");
			if (attachment_recoil_modifier != 1)
			{
				attachment_recoil_modifier *= 1.05;
			}

			//DebugPropertyModifiers("attachment: "+attachment);
			//DebugPropertyModifiers("attachment_recoil_modifier: "+attachment_recoil_modifier);
			
			// apply modifier
			m_RecoilModifier *= attachment_recoil_modifier;
			m_InertiaModifier *= attachment_inertia_modifier;
			//m_SwayModifier *= PropertyModifiers.GetVectorModifier(attachment, "swayModifier");
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