modded class AK_RailHndgrd extends Inventory_Base
{
	override bool CanDetachAttachment(EntityAI parent)
	{
		if (Weapon_Base.Cast(parent) && parent.FindAttachmentBySlotName("weaponBipod"))
		{
			return false;
		}

		return super.CanDetachAttachment(parent);
	}
}