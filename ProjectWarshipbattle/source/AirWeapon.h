#pragma once
/*飛行機に乗せた武器を実装するためのクラス*/
class AirWeapon
{
public:
	AirWeapon(int WeaponType) :weaponType(WeaponType) {}
	virtual ~AirWeapon();

	//残弾数問い合わせ
	int ReferAmmoRemained() { return ammoRemain; }
	bool Usable() { return ammoRemain != 0 ? true : false; }

	//弾などを発射して残弾が減る
    void UseAmmo() { ammoRemain--; }

	//初期化（弾の数）
	void SetAmmo(int Amount) { ammoRemain = Amount; maxAmmo = Amount; }

private:
	int weaponType;//継承先で直接定義する

	int ammoRemain;//残弾数
	int maxAmmo;//最大弾数
};

