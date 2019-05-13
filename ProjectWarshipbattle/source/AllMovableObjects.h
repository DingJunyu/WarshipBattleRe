#pragma once
#include<cmath>
#include"DefinedData.h"
#include"Camera.h"
#include"DxLib.h"
/*インターフェイスについて:
一部分のインターフェイスはprivateに移したい
*/

class AllMovableObjects
{
public:
	//飛べるか、飛行機か、点に見えるか、あたり判定使うか
	AllMovableObjects(bool Flyable, bool Plane, bool Point,bool Crashable) :
		flyable(Flyable),
		plane(Plane), alive(true), point(Point), crashable(Crashable) {
		alive = true;
		inList = false;
		radianChangePerFrame = 0;
		airResistanceActive = false;
		airResistance = 0.00001;
		speedOnZ = 0;
		input = 0;
		distanceMoved = 0;
	}
	virtual ~AllMovableObjects();

	//データ問い合わせ関数
	double ReferCoordX() { return coord.x; }/*x座標*/
	double ReferCoordY() { return coord.y; }//y座標
	double ReferCoordZ() { return coord.z; }//z座標
	Coordinate<double> ReferCoord() { return coord; }//3D座標
	Coordinate2D<double> ReferCoord2D_d() { return Coordinate2D<double> 
	{(double)coord.x, (double)coord.z}; }//平面座標(double)
	Coordinate2D<int> ReferCoord2D() { return Coordinate2D<int>
	{(int)coord.x,(int)coord.z}; }/*平面座標(int)*/

	double ReferSpeedOnZ() { return speedOnZ; }//平面速度
	double ReferSpeedOnY() { return speedOnY; }//垂直速度
	double ReferAirResistance() { return airResistance; }//空気抵抗
	double ReferRadianOnZ() { return radianOnZ; }//平面角度
	double ReferRadianOnY() { return radianOnY; }//垂直角度
	double ReferLength() { return length; }//長さ(画像)
	double ReferWidth() { return width; }//幅(画像)
	double ReferMultiple() { return multiple; }//拡大率

	double ReferDistanceMoved() { return distanceMoved; }

	//状態問い合わせ関数
	bool ReferAlive() { return alive; }//生きる状況確認
	bool ReferInList() { return inList; }
	int *ReferGraphHandle() { return pictureHandle; }//画像ハンドル

	//移動関数
	void Move();//移動
	void Unmove();//前の状態に戻る
	void FallingDown();//落下
	void SpeedDownbyAirResistance();//空気抵抗の影響で速度が落とす

	//描画関数
	void SetPictureHandle(int *p) { pictureHandle = p; }
	void SetShadowHandle(int *p) { shadowHandle = p; }
	void Draw(Camera CM);//カメラの中心にある時使う描画関数
	void DrawShadow(Camera CM);//影を描画する
	void DrawSub(Camera CM);//他のところにある時に使う描画関数
	void DrawSubShadow(Camera CM);//影を描画する
	void DrawCard(Coordinate2D<double> coord);

	//状態設置関数
	//ステータス設定関数
	//トン単位
	void SetMass(double Mass) { mass = Mass; }//重量を設置する
	//ニュートン単位ですが、現実とちょっと違う
	void SetOutput(double Output) { input = Output; }//出力を設置する
	void SetMaxOutput(double MaxOutput) { maxInput = MaxOutput; }//最大出力値を設置する
	//新しい座標を設定する
	void NewCoordX(double X) { coord.x = X; }
	void NewCoordZ(double Z) { coord.z = Z; }
	void NewCoordY(double Y) { coord.y = Y; }
	void SetCoord(Coordinate<double> NC) { coord = NC; }
	//新しい角度を設定する
	void SetRadianOnZ(double ROZ) { radianOnZ = ROZ; }
	void SetRadianChangePerFrame(double RCPF) { radianChangePerFrame = RCPF; }
	void SetRadianOnY(double ROY) { radianOnY = ROY; }
	//サイズを設定する
	void SetLength(double L) { length = L; }
	void SetWidth(double W) { width = W; }
	void SetMultiple(double M) { multiple = M; }

	void Killed() { alive = false; SetSpeed(0); }//状態を死亡に設置する
	void PutInList() { inList = true; }
	void SetSpeed(double Spe);//速度を設置
	void SetMaxSpeedOnZ(double Speed) { maxSpeedOnZ = Speed; }//最大速度を設置
	void UnsetPlane() { plane = false; }//飛行機が死んだら普通の飛ぶものに変える
	

private:
	/*XZは水平座標、Yは高さ*/
	Coordinate<double> coord;
	Coordinate<double> oldCoord;
	double distanceMoved;
	/*スピードは水平と垂直に分ける*/
	double speedOnZ;
	double speedOnY;
	double maxSpeedOnZ;//船と飛行機の最大速度
	bool speedOnZLargerThan0;//正数と負数の判定
	bool airResistanceActive;//空気抵抗を使うか
	double airResistance;//空気抵抗
	double radianOnZ;//行く方向(水平)
	double radianChangePerFrame;
	double radianOnY;//行く方向(垂直)
	bool alive;//生きる状態
	bool inList;

	/*出力値と重量*/
	double input;
	double maxInput;
	double mass;

	/*継承先で値を与える*/
	bool flyable;//これは飛べるものか（重力影響の有無）
	bool plane;//これは飛べるものだが、重力を克服して飛ぶことができる

	/*長方形を利用して簡単な当たり判定を行う*/
	/*継承先で値を与える*/
	bool point;//点と判断する時は長さなどを使わない
	/*サイズ*/
	double length;
	double width;

	//当たり判定が必要かどうか
	bool crashable;
	
	//画像のハンドル
	int *pictureHandle;
	int *shadowHandle;
	double multiple;
};

