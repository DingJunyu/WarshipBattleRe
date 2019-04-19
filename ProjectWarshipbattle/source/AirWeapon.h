#pragma once
/*��s�@�ɏ悹��������������邽�߂̃N���X*/
class AirWeapon
{
public:
	AirWeapon(int WeaponType) :weaponType(WeaponType) {}
	virtual ~AirWeapon();

	//�c�e���₢���킹
	int ReferAmmoRemained() { return ammoRemain; }
	bool Usable() { return ammoRemain != 0 ? true : false; }

	//�e�Ȃǂ𔭎˂��Ďc�e������
    void UseAmmo() { ammoRemain--; }

	//�������i�e�̐��j
	void SetAmmo(int Amount) { ammoRemain = Amount; maxAmmo = Amount; }

private:
	int weaponType;//�p����Œ��ڒ�`����

	int ammoRemain;//�c�e��
	int maxAmmo;//�ő�e��
};

