#include "MapEditScene.h"

#include "InGameScene.h"
#include "..\Object\Object.h"
#include "..\Object\Player.h"
#include "..\Object\Monster.h"
#include "..\Object\Tile.h"
#include "..\Core\Core.h"
#include "..\Core\SourceManager.h"
#include "..\Core\InputManager.h"

// 정적 변수 초기화
int CMapEditScene::rect_num_x_ = 0;
int CMapEditScene::rect_num_y_ = 0;
vector<class CTile*> CMapEditScene::rect_tile_vec_;

void CMapEditScene::AddTile(class CTile* _t)
{
	// 포인터 생성으로 인해 추가
	_t->AddRef();

	// Clone은 자동 참조 카운트 추가
	CTile* tmp_tile = _t->Clone();

	// push_back으로 추가
	tmp_tile->AddRef();
	rect_tile_vec_.push_back(tmp_tile);
	SAFE_RELEASE(tmp_tile)
}

void CMapEditScene::ClearTile()
{
	SafeReleaseList(rect_tile_vec_);
}

CMapEditScene::CMapEditScene()
{
}

CMapEditScene::~CMapEditScene()
{
	CCore::Instance()->SetMapEditMode(false);
	SafeReleaseList(rect_tile_vec_);
}

bool CMapEditScene::Init(HWND _hWnd)
{
	if (!CScene::Init(_hWnd))
		return false;

	CCore::Instance()->SetMapEditMode(true);

	// ---------------------------------------------------------------------- : >> 스테이지 생성

	// 레이어 찾기
	CLayer* pt_layer = FindLayer("Background");
	if (pt_layer == nullptr) return false;

	int tile_x_num_ = 50;
	int tile_y_num_ = 50;
	int tile_width_ = TEXTURE_SIZE;
	int tile_height_ = TEXTURE_SIZE;
	string texture_key = WATER1;

	// 월드 사이즈 설정
	world_size_.x = tile_width_ * tile_x_num_;
	world_size_.y = tile_height_ * tile_y_num_;
	/*SetWindowPos(hWnd_, HWND_TOPMOST, 100, 100, 300, 300, SWP_NOMOVE | SWP_NOZORDER);
	ShowWindow(hWnd_, SW_SHOW);
	UpdateWindow(hWnd_);*/

	// 레이어 내 크기, 갯수, 텍스처 종류로 타일 생성
	pt_layer->CreateTile(pose_, tile_x_num_, tile_y_num_, tile_width_, tile_height_, texture_key, _T("water1.bmp"), TEXTURE_PATH);

	// 씬의 카메라 초기화
	hWnd_ = _hWnd;
	RECT rectView;
	GetClientRect(hWnd_, &rectView);
	SetWndSize(rectView.right, rectView.bottom);

	// main 화면 기준 위치, 월드 사이즈 & 씬 사이즈 & 속도 초기화
	camera_->Init(MY_POSE(0.f, 0.f), wnd_size_, world_size_, 500.f);

	// ---------------------------------------------------------------------- : <<


	return true;
}


void CMapEditScene::Input(float _time)
{
	CScene::Input(_time);
	// CCameraManager::Instance()->Input(_time);

	// 마우스 위치가 Assist 씬 위일 경우만 실행
	if (CInputManager::Instance()->GetHwnd() != hWnd_)
	{
		// 레이어 찾기
		CLayer* pt_layer = FindLayer("Assist_scene_mouse_rect");
		if (pt_layer == NULL) return;

		// 레이어에 오브젝트 있으면 삭제
		pt_layer->Clear();

		return;
	}

	// 현재 마우스 위치 get
	MY_POSE mouse_pos = CInputManager::Instance()->GetMousePose();

	// : >> >> 새 마우스 사각형 그리기

	// 준비물: 과거 마우스 다운 위치, 현재 마우스 위치, 씬의 텍스쳐 가로 갯수, 타일 하나 가로 & 세로 길이

	// mouse_down_pose_ 위치에 마우스 사각형 집합 생성
	int rect_num_x = rect_num_x_;
	int rect_num_y = rect_num_y_;



	//--------------------------------------------------------------------------------------------//

	// 만들 마우스 사각형 집합 갯수가 타일 집합 경계 안에 있나?

	//--------------------------------------------------------------------------------------------//

	// 1. mouse_down_pose_ 위치 조정 (전체 타일 집합에 맞게) 
	MY_POSE tmp_mouse_pose = mouse_pos / TEXTURE_SIZE;
	int tmp_mouse_pose_x_idx = floor(tmp_mouse_pose.x);
	int tmp_mouse_pose_y_idx = floor(tmp_mouse_pose.y);

	// 2. rect_num_x_ & rect_num_y_ 조정 (전체 타일 집합 크기에 맞게) 
	// 레이어 찾기
	CLayer* pt_layer2 = FindLayer("Background");
	if (pt_layer2 == nullptr) return;

	// 해당 레이어에서 "stage" 오브젝트를 가져와 타일 집합의 x, y 방향 갯수를 알아낸다.
	/*CStage* pt_stage2 = pt_layer2->FindObj<CStage>("stage");
	if (pt_stage2 == nullptr)
	{
		SAFE_RELEASE(pt_stage2);
		return;
	}*/

	int tileset_x_length = pt_layer2->GetTileXNum();
	int tileset_y_length = pt_layer2->GetTileYNum();
	if (tileset_x_length < tmp_mouse_pose_x_idx + rect_num_x)
	{
		rect_num_x = tileset_x_length - tmp_mouse_pose_x_idx;
	}
	if (tileset_y_length < tmp_mouse_pose_y_idx + rect_num_y)
	{
		rect_num_y = tileset_y_length - tmp_mouse_pose_x_idx;
	}

	if (rect_num_x <= 0 || rect_num_y <= 0)
	{
		// SAFE_RELEASE(pt_stage2);
		return;
	}


	//--------------------------------------------------------------------------------------------//

	// End

	//--------------------------------------------------------------------------------------------//




	// 레이어 찾기
	CLayer* pt_layer = FindLayer("Assist_scene_mouse_rect");
	if (pt_layer == NULL)
	{
		// SAFE_RELEASE(pt_stage2);
		return;
	}

	// 레이어에 오브젝트 있으면 삭제
	pt_layer->Clear();

	// CStage* pt_stage = CObject::CreateObj<CStage>("Assist_scene_mouse_rect", pt_layer);


	//// 마우스 처음 다운된 곳 보다 현재 마우스 위치가 작다면 혹은 화면상 높은 경우의 수 제외
	//if (mouse_pos.x < mouse_down_pose_.x || mouse_pos.y < mouse_down_pose_.y)
	//{
	//	SAFE_RELEASE(pt_stage);
	//	return;
	//}

	// : >> 값 조정
	// 1. mouse_down_pose_ 위치 조정 (전체 타일 집합에 맞게) 
	mouse_pos += camera_->GetPose();
	mouse_pos /= TEXTURE_SIZE;
	int mouse_pose_x_idx = floor(mouse_pos.x);
	int mouse_pose_y_idx = floor(mouse_pos.y);
	mouse_pos.x = mouse_pose_x_idx;
	mouse_pos.y = mouse_pose_y_idx;
	mouse_pos *= TEXTURE_SIZE;
	

	// <<

	// 타일 생성
	// 실패 시, 리턴
	if (!pt_layer->CreateTile(mouse_pos, rect_num_x, rect_num_y, TEXTURE_SIZE, TEXTURE_SIZE, EMPTY_BW_16, _T("empty_bw_16.bmp"), TEXTURE_PATH))
	{
		// SAFE_RELEASE(pt_stage);
		// SAFE_RELEASE(pt_stage2);
		return;
	}



	//--------------------------------------------------------------------------------------------//

	// 마우스 클릭 시 타일 값 붙여넣기

	//--------------------------------------------------------------------------------------------//

	// return;

	// 마우스가 눌려진게 아니면 업데이트할 필요 없음!
	if (!CInputManager::Instance()->GetMouseLeftDown())
	{
		cout << "\nhaha4\n";
		// SAFE_RELEASE(pt_stage);
		// SAFE_RELEASE(pt_stage2);
		return;
	}
	cout << "\nhaha5\n";
	// MapEditScene 타일 업데이트
	int cnt = 0;
	int init_i = tmp_mouse_pose_x_idx + tmp_mouse_pose_y_idx * tileset_x_length;
	for (int i = 0; i < rect_num_y_; i++)
	{
		for (int j = 0; j < rect_num_x_; j++)
		{
			int idx = init_i + tileset_x_length * i;
			idx += j;
			cout << "idx : " << idx << endl;

			// int w = rect_tile_vec_[cnt]->GetWidth();
			// Clone은 자동 참조 카운트 추가
			CTile* tmp_tile = rect_tile_vec_[cnt++]->Clone();
			tmp_tile->SetPose(mouse_pos.x + j * TEXTURE_SIZE, mouse_pos.y + i * TEXTURE_SIZE);

			pt_layer2->AddObj(tmp_tile);
			
			// pt_stage2->ChangeTile(idx, rect_tile_vec_[cnt++]);
			cout << "idx haha : " << idx << endl;
		}
	}
	cout << endl << endl;
	 // <<

	//--------------------------------------------------------------------------------------------//

	// End

	//--------------------------------------------------------------------------------------------//

	// SAFE_RELEASE(pt_stage);
	// SAFE_RELEASE(pt_stage2);
}

void CMapEditScene::Update(float _time)
{
	CScene::Update(_time);
}

void CMapEditScene::LateUpdate(float _time)
{
	CScene::LateUpdate(_time);
}

void CMapEditScene::Collision(float _time)
{
	CScene::Collision(_time);
}

void CMapEditScene::Render(HDC _hdc, float _time)
{
	CScene::Render(_hdc, _time);
}
