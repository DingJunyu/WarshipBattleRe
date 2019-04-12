#pragma once
class AirWeapon
{
public:
	AirWeapon(int WeaponType) :weaponType(WeaponType) {}
	virtual ~AirWeapon();

	//c’e”–â‚¢‡‚í‚¹
	int ReferAmmoRemained() { return ammoRemain; }
	bool Usable() { return ammoRemain != 0 ? true : false; }

	//’e‚È‚Ç‚ğ”­Ë‚µ‚Äc’e‚ªŒ¸‚é
    void UseAmmo() { ammoRemain--; }

	//‰Šú‰»i’e‚Ì”j
	void SetAmmo(int Amount) { ammoRemain = Amount; maxAmmo = Amount; }

private:
	int weaponType;//Œp³æ‚Å’¼Ú’è‹`‚·‚é

	int ammoRemain;
	int maxAmmo;
};

