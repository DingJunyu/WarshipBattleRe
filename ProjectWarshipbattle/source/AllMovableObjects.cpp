#include "AllMovableObjects.h"

AllMovableObjects::~AllMovableObjects()
{
}

void AllMovableObjects::Move() {
	oldCoord = coord;
	//2D�Q�[���ł����AY���̈ړ����l����
	if (flyable) {//����͔�ׂ���̂�
		if (!plane) {//��s�@�ł͂Ȃ��ꍇ
			FallingDown();//���R����
			SpeedDownbyAirResistance();//��C��R�ő��x�����Ƃ�
		}
		coord.y += speedOnY;//Y���W�̈ړ�
	}

	if (input != 0) {//�o�͂�����ꍇ�ɑ��x���v�Z����
		/*�����̕���*/
		if (abs(speedOnZ) < abs(maxSpeedOnZ * (input / maxInput))) {
			speedOnZ += input / mass * (1 - abs(speedOnZ / maxSpeedOnZ)) * 0.5;
			/*���x�͍ő呬�x�ɋ߂��ɂȂ�Ɖ����x�����Ƃ�(�d�ʂƑ��x�֌W����)*/
		}
	}

	/*�o�͂��\�Ȃ��̂͑��x������Ȃ��ꍇ�ɑ��x�����Ƃ�*/
	if (!flyable) {
		/*�O�i����ꍇ*/
		if (speedOnZ > 0) {
			if (speedOnZ > maxSpeedOnZ * (input / maxInput)) {//�������x�͍��̍ő呬�x�ȏ�ł����
				speedOnZ -= (speedOnZ / maxSpeedOnZ > 0.5) ? 0.0015 : 0.00075;//���x���������Ɍ���������
				//�X�s�[�h�������̂ق����e���������悤�ɂ��܂����B
				if (speedOnZ < 0)//�O�i�̎��Ɍ�ނɂȂ�Ȃ��悤
					speedOnZ = 0;
			}
		}
		/*��ނ���ꍇ*/
		if (speedOnZ < 0) {
			//�������x�͍��̍ő呬�x�ȏ�ł����
			if (speedOnZ < maxSpeedOnZ * (input / maxInput)) {
				//���x���������Ɍ���������
				speedOnZ += (speedOnZ / maxSpeedOnZ > 0.5) ? 0.0015 : 0.00075;
				//�X�s�[�h�������̂ق����e���������悤�ɂ��܂����B
				if (speedOnZ > 0)
					speedOnZ = 0;
			}
		}
	}

	/*���x������ꍇ�ɓ]�����ł��܂�*/
	if (speedOnZ != 0 && radianChangePerFrame != 0) {
		//���x��������Α����قǓ]���������Ȃ�
		radianOnZ += (radianChangePerFrame/100) * speedOnZ;
	}
	//�I�[�o�[�t���[�΍�:Radian�͈̔͂�-2��~2��
	if (radianOnZ > (MathAndPhysics::PI * 2)) {
		radianOnZ -= (MathAndPhysics::PI * 2);
	}
	if (radianOnZ < - (MathAndPhysics::PI * 2)) {
		radianOnZ += (MathAndPhysics::PI * 2);
	}

	//�ړ��������W���v�Z����
	coord.x += speedOnZ * cos(radianOnZ);
	coord.z += speedOnZ * sin(radianOnZ);

	distanceMoved += speedOnZ;
}

void AllMovableObjects::Unmove() {
	coord = oldCoord;//�O�̍���ɖ߂�
}

void AllMovableObjects::FallingDown() {//�d�͂̉e���ŗ��Ƃ�
	if (coord.y > 0 && flyable && !plane) {
		speedOnY -= MathAndPhysics::GRAVITATIONAL_ACCELERATION;
	}
}

void AllMovableObjects::SetSpeed(double Spe) {
	//�����ʂł͐��������̃X�s�[�h���v��Ȃ�
	if (!flyable) {
		speedOnY = 0;
		speedOnZ = Spe;
	}
	else {//���x���O�̕����ɕ�������
		speedOnY = sin(radianOnY)*Spe;
		speedOnZ = cos(radianOnY)*Spe;
	}
	speedOnZLargerThan0 = speedOnZ > 0 ? true : false;//������ݒu����
}

void AllMovableObjects::SpeedDownbyAirResistance() {
	if (speedOnZ == 0)
		return;
	if (airResistance > MathAndPhysics::MIN_AIR_RESISTANCE)
		airResistance *= 0.99;//��C��R�̉e�����ǂ�ǂ񌸂炷
	else
		airResistance = MathAndPhysics::MIN_AIR_RESISTANCE;//���E�ɒ�������ŏ���C��R�ɂȂ�

	if (speedOnZLargerThan0 && speedOnZ > 0) {
		speedOnZ -= airResistance;//���x�����炷
		if (speedOnZ < 0)//�������ς��Ȃ�
			speedOnZ = 0;
	}
	if (!speedOnZLargerThan0 && speedOnZ < 0) {
		speedOnZ += airResistance * 0.1;//���x�����炷
		if (speedOnZ > 0)//�������ς��Ȃ�
			speedOnZ = 0;
	}
}

/*�`��ɂ��āF
������K�v�͂Ȃ��ł�*/

//���@�`�����͂��̊֐����g��
//���@�͉�ʂ̒��S�ɂ��邽�߁A�����̓J�����̒��S���W���g��
//����X�V�ɂ��āFDraw��DrawShadow�ɕ�����A�e�̕`��Ɩ{�̂̕`�悪�ʁX�ɍs���܂��B
void AllMovableObjects::Draw(Camera CM) {
	double xOnScreen;
	double zOnScreen;
	//�D����e�ւ̋���
	int shadowDistanceOnX = 2;
	int shadowDistanceOnZ = 2;
	//���S���W�����
	xOnScreen = Screen::SCREEN_X / 2;
	zOnScreen = Screen::SCREEN_Z / 2;

	//�{�̂�`��
	DrawRotaGraph3((int)xOnScreen, (int)zOnScreen, (int)(length / 2), 
		(int)(width) / 2, multiple, multiple,
		radianOnZ, *pictureHandle, TRUE, FALSE);
}

void AllMovableObjects::DrawShadow(Camera CM) {
	double xOnScreen;
	double zOnScreen;
	//�D����e�ւ̋���
	int shadowDistanceOnX = 2;
	int shadowDistanceOnZ = 2;
	//���S���W�����
	xOnScreen = Screen::SCREEN_X / 2;
	zOnScreen = Screen::SCREEN_Z / 2;

	//�e��`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);//�����x��������
	DrawRotaGraph3((int)xOnScreen + shadowDistanceOnX,
		(int)zOnScreen + shadowDistanceOnZ, (int)(length / 2),
		(int)(width) / 2, multiple, multiple,
		radianOnZ, *shadowHandle, TRUE, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);//�`�惂�[�h�����Ƃɖ߂�
}

//���@�ȊO�̂͂��̊֐����g��
//���@�ȊO�ł͎��@�ɑ΂��č��W�𗘗p���ĕ`��
void AllMovableObjects::DrawSub(Camera CM) {
	/*�V���h�[�̋����v�Z�͂܂�*/
	int shadowDistanceOnX = 2;
	int shadowDistanceOnZ = 2;

	/*�����̍��W�ƃJ�������W�����킹�Čv�Z����*/
	double coordX = coord.x - CM.ReferRealCameraX();
	double coordZ = coord.z - CM.ReferRealCameraZ();

	//�{�̂�`��
	/*��ʂɓ��鎞�����`����s��*/
	if (coordX > 0 - 100 && coordX < Screen::SCREEN_X + 100 && coordZ>0 - 100 &&
		coordZ < Screen::SCREEN_Z + 100) {
		DrawRotaGraph3((int)(coordX),
			(int)(coordZ),
			(int)(length / 2), (int)(width / 2),
			multiple, multiple, radianOnZ,
			*pictureHandle, TRUE, FALSE);
	}
}

void AllMovableObjects::DrawSubShadow(Camera CM) {
	/*�V���h�[�̋����v�Z�͂܂�*/
	int shadowDistanceOnX = 2;
	int shadowDistanceOnZ = 2;

	/*���W�ƃJ�������W�����킹�Čv�Z����*/
	double coordX = coord.x - CM.ReferRealCameraX() + shadowDistanceOnX;
	double coordZ = coord.z - CM.ReferRealCameraZ() + shadowDistanceOnZ;

	//�J�����̍��W������ĉf�����W���v�Z����
	//�e��`��
	/*��ʂɓ��鎞�����`����s��*/
	if (coordX > 0 - 100 && coordX < Screen::SCREEN_X + 100 && coordZ>0 - 100 &&
		coordZ < Screen::SCREEN_Z + 100) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);//�����x��������
		DrawRotaGraph3((int)(coordX),
			(int)(coordZ),
			(int)(length / 2), (int)(width / 2),
			multiple, multiple, radianOnZ,
			*shadowHandle, TRUE, FALSE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}//�`�惂�[�h�����Ƃɖ߂�
}