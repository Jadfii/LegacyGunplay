modded class SawedoffIzh43Shotgun extends Izh43Shotgun_Base 
{
	override RecoilBase SpawnRecoilObject()
	{
		return new Izh43SawedOffRecoil(this);
	}
}