#pragma once
#include"HeaderForIngameDataManagement.h"
/*�֐��̃v���C�x�[�g���͂��ꂩ�炷��*/
class IngameDataManagement
{
public:
	IngameDataManagement() :MainCamera(CameraType::MAIN) {
		TEST_SHOW_ON = false;
		GameOver = false;
		counter = 0;
		shouldIRender = false;
		showLock = false;
	}
	~IngameDataManagement();

	//���C�����[�v
	//�Q�[���{��
	void Update();

	//�Q�[��������������
	//�����̊֐��Ƀt�@�C��������܂�
	void Inif();
	void registerTeam();
	void Free();

	//�J�����p���W�₢���킹
	double ReferPlayerX() { return alliesFleet[0].ReferCoordX(); }
	double ReferPlayerZ() { return alliesFleet[0].ReferCoordZ(); }
	double ReferTargetX(int x) { return enemyFleet[x].ReferCoordX(); }
	double ReferTargetZ(int x) { return enemyFleet[x].ReferCoordZ(); }
	
	//GameController�₢���킹
	int ReferRemainedAlliesNum();
	int ReferRemainedEnemiesNum();
	int ReferRemainedAlliesNumInHanger();
	int ReferRemainedAlliesNumInSky();
	int ReferRemainedEnemiesNumInHanger();
	int ReferRemainedEnemiesNumInSky();

	bool TeamDestroyed();
	bool ReferEndGame() { return GameOver; }

	//���v�f�[�^
	int ShootCount();//�����͂��Ƃł���

	//���ׂẴe�X�g�͂����ōs��
	void TEST();

	//���[�h���
	void DrawLoading();


	void FormationBoard();
	void FreeFormationBoard();

	void DrawStatisticBoard();
private:
	//���̃R���g���[���[
	PictureLoader PL;//�摜���[�_�[
	FrameControl FC;//�t���[���R���g���[���[
	Camera MainCamera;//���C���J����
	Controller CT;//�L�[�{�[�h���͊Ǘ�
	UserInterface UI;//�N���b�N�ł��Ȃ�UI
	EffectTemplate ET;//�G�t�F�N�g�e���v���[�g
	SoundLoader SL;//�������[�_�[
	ClickableUserInterface CUI;//�N���b�N�ł���UI

	/*AI�Ǘ�*/
	FlagShipAI flagShipAI;

	void AIUpdate();
	void LetFlagShipMove();
	void LetEveryOneMove();
	void ControlThisListMove(std::vector<ShipMain> *shipList,
		ArtificialIntelligence *AI);
	void LetEveryOneLockOn();
	void ControlThisListLock(std::vector<ShipMain> *shipList,
		std::vector<ShipMain> enemyList);
	void LetEveryOneShoot();
	void ControlThisListShoot(std::vector<ShipMain> *shipList);

	/*�`�揈���֘A*/
	long long counter;//60�܂ŃJ�E���g����
	bool shouldIRender;//�T���󋵍X�V�X�C�b�`
	bool showLock;

	//�`���֐�
	void DrawAll();
	void TEST_DRAW();
	void DrawSea();
	void DrawShips();
	void DrawShipsShadow();
	void DrawShipsOnMiniMap();
	void DrawPlanes();
	void DrawAmmo();
	void DrawBomb();
	void DrawTorpedo();
	void DrawThisList(std::list<Effect> *effectList);
	void DrawEffectUnderShips();
	void DrawEffectBeyondShips();
	void DrawPointOfImpact();

	//�ړ��֐�
	void MoveAll();
	void MoveShips();
	void MoveEffect(std::list<Effect> *effectList);
	void MoveEffects();
	void MoveAmmo();
	void CheckShipListStatus(std::vector<ShipMain> *shipList);
	void CheckShipsStatus();//�D�̏�Ԃ��X�V����

	//�R���g���[���֐�
	void Control();

	//�e�X�g�֘A
	bool TEST_SHOW_ON;//�f�o�b�O��ʂ��N�p���邩
	void SIMPLE_USER_INTERFACE();//�X�e�[�^�X�𕶎��ɂ��ĕ\������
	bool GameOver;//�Q�[���I���}�[�N

	//�����蔻��Ȃ�
	void CrashDecision();//�����蔻��-�D�ƑD�̊�
	bool PointsToCollisionbox(ShipMain *ship1,ShipMain *ship2);
	void HitDecision();//�Q�[���R���g���[���[�ɓ����鐔��Ԃ�
	void SimpleHitDecision();
	void CheckThisTeamDecision(std::vector<ShipMain> *shipList, Ammo *shell);//���̃`�[���ƒe���X�g�̊Ԃɂ����蔻����s��
	void RemoveDestroyedShip();//���񂾑D������
	void CheckTeamA(std::vector<ShipMain> *teashipListm);//���񂾑D�𒾂މ��o���X�g�ɒǉ�����

	//�e�Ǘ�
	void TestShoot(ShipMain *ship,bool me);

	void LockAndRefresh();
	void CheckThisTeamLock(std::vector<ShipMain> *shipList,
		std::vector<ShipMain> enemyList);

	void CheckSelectedWeapon();
	void InputNewAmmo(ShipMain *SM, FiringData FD,bool me);//�V�����e��list�ɒǉ�
	void DeleteUselessAmmo();//�����؂���̂��폜

	/*�����֐�*/
	void CheckAndPlaySound();

	/*�G�t�F�N�g�֘A*/
	void GetNewEffect();
	void DeleteUselessEffect();
	void CheckAndCleanThisEffectList(std::list<Effect> *effectList);
	void NewEffectForShips(std::vector<ShipMain> shipList);
	void NewExplosion(Coordinate2D<double> Coord);
	void NewRipple(double coordX,double coordZ);//�C�e���C�ɗ����鎞�V�����G�t�F�N�g�𐶐�

	/*�Q�[���R���g���[��*/
	bool win;
	void DeleteUseless();
	void DestroyShips();
	void DestroyThisTeam(std::vector<ShipMain> *shipList);
	void CheckTeamStatus();
	void CheckAlliesStatus();
	void CheckEnemyStatus();
	void TEST_WIN();

	void DrawFormationBoard();
	void InifFormationBoard();

	std::vector<ShipMain> alliesFleet;//���R�͑�
	std::vector<ShipMain> enemyFleet;//�G�R�͑�
	ArtificialIntelligence AI;
	std::list<ShipMain> sinkingShip;//����ł���D

	std::list<Ammo> shellList;//�C�e���X�g
	std::list<Bomb> bombList;//���e���X�g
	std::list<Torpedo> torpedoList;//�������X�g
	std::list<Effect> bubbleList;//���A���X�g
	std::list<Effect> smokeList;//�_���X�g
	std::list<Effect> explosionList;//�������X�g
	std::list<Effect> rippleList;//���A���X�g(�C�e�����n)

	/*���v*/
	void InifStatisticBoardData() {
		shootCount = 0;
		hitCount = 0;
		movedDis = 0;
		damage = 0; damageRecieved = 0;
		killed = 0;
	}
	int *statisticBoard[StatisticBoard::SB_NUM];
	int shootCount;
	int hitCount;
	double hitRate;
	double movedDis;
	int damage;
	int damageRecieved;
	int killed;
	
	/*�Ґ�*/
	int *formationBoard[FormationBoard::FB_NUM];
	std::vector<ShipCard> teamA;
	std::vector<ShipCard> teamB;

	const int maxCountInATeam = 10;
	int teamAChoose[4][2];
	int teamBChoose[4][2];

};