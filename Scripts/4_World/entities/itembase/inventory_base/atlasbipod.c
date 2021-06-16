/* Logic for handling when bipod is used in DayZPlayerImplement.HandleWeapons */
modded class AtlasBipod extends Inventory_Base
{
	protected string m_DefaultAnimPhase = "bipod";
	protected bool m_Deployed; // is bipod deployed

	void AtlasBipod()
	{
		RegisterNetSyncVariableBool("m_Deployed");
	}

	override void OnVariablesSynchronized()
	{
		DoBipodAnimation(m_Deployed);
	}

	override bool CanPutAsAttachment(EntityAI parent)
	{
		// Since super only accounts for rail handguard attachments
		return ((!IsRuined() && !parent.IsRuined() && parent.IsInherited(Mosin9130_Base)) || super.CanPutAsAttachment(parent));
	}

	override bool CanDetachAttachment(EntityAI attachment)
	{
		return !IsDeployed();
	}

	bool IsDeployed()
	{
		return m_Deployed;
	}

	void SetDeployed(bool deployed)
	{
		m_Deployed = deployed;
		SetSynchDirty();
	}

	void DeployBipod(bool deploy)
	{
		SetDeployed(deploy);
		DoBipodAnimation(deploy);
	}

	void DoBipodAnimation(bool deploy)
	{
		if (deploy)
		{
			SetAnimationPhase(m_DefaultAnimPhase, 1);
		}
		else
		{
			SetAnimationPhase(m_DefaultAnimPhase, 0);
		}
	}
}