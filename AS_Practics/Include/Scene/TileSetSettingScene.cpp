#include "TileSetSettingScene.h"

#include "AssistScene.h"

#include "InGameScene.h"
#include "MapEditScene.h"
#include "SceneManager.h"

#include "..\Object\Object.h"
#include "..\Object\Player.h"
#include "..\Object\Monster.h"
#include "..\Object\Tile.h"

#include "..\Core\Core.h"
#include "..\Core\SourceManager.h"
#include "..\Core\TfManager.h"
#include "..\Core\InputManager.h"
#include "..\Core\Texture.h"


CTileSetSettingScene::CTileSetSettingScene()
{

}
CTileSetSettingScene::~CTileSetSettingScene()
{

}

bool CTileSetSettingScene::Init(HWND _hWnd)
{

	if (!CMapToolScene::Init(_hWnd))
		return false;

	// AssistScene에서 텍스쳐 테그를 가져온다.
	CAssistScene* pt_assist = static_cast<CAssistScene*>(CSceneManager::Instance()->pt_assist_scene_);
	
	// CTileSetSettingScene에 깔린 텍스쳐 테그 설정
	texture_tag_ = pt_assist->GetTextureTag();

	// FLOOR 레이어를 해당 텍스쳐로 깐다.
	ChangeBackTileSheet(_hWnd, texture_tag_);

	// UI 레이어를 깐다
	vector<CTile*> vec_tile;

	CTile* pt_tile = CObject::CreateObj<CTile>("O_32", NULL);
	pt_tile->SetTexture(O_32);
	pt_tile->AddRef();
	vec_tile.push_back(pt_tile);
	SAFE_RELEASE(pt_tile);

	pt_tile = CObject::CreateObj<CTile>("X_32", NULL);
	pt_tile->SetTexture(X_32);
	pt_tile->AddRef();
	vec_tile.push_back(pt_tile);
	SAFE_RELEASE(pt_tile);

	pt_tile = CObject::CreateObj<CTile>("T_32", NULL);
	pt_tile->SetTexture(T_32);
	pt_tile->AddRef();
	vec_tile.push_back(pt_tile);
	SAFE_RELEASE(pt_tile);

	// texture_tag_ 텍스쳐의 정보를 참조하여 UI_LAYER에 O/X/T 표시한다.
	PaintMap2(vec_tile, 1, 1, UI_LAYER, world_size_, texture_tag_);

	SafeReleaseList(vec_tile);
	return true;
}

void CTileSetSettingScene::Input(float _time)
{
	CMapToolScene::Input(_time);

	// 안누르다가 눌렀다면
	if (!CInputManager::Instance()->GetPastMouseLeftDown())
	{
		if (CInputManager::Instance()->GetMouseLeftDown())
		{
			mouse_down_pose_ = CInputManager::Instance()->GetMousePose();

			// 마우스 다운 위치 수정 by camera
			MY_POSE mouse_down_pose_cam_ = mouse_down_pose_;
			mouse_down_pose_cam_ += camera_->GetPose();

			// mouse_down_pose_ 위치 조정 (전체 타일 집합에 맞게) 
			mouse_down_pose_cam_ /= TEXTURE_SIZE;
			int mouse_down_pose_x_idx = floor(mouse_down_pose_cam_.x);
			int mouse_down_pose_y_idx = floor(mouse_down_pose_cam_.y);

			// 예외 처리
			if (mouse_down_pose_x_idx >= (world_size_.x / TEXTURE_SIZE) || mouse_down_pose_x_idx < 0)
				return;
			if (mouse_down_pose_y_idx >= (world_size_.y / TEXTURE_SIZE) || mouse_down_pose_y_idx < 0)
				return;


			// 현재 씬에 깔려있는 텍스쳐를 가져온다.
			CTexture* pt_texture = CSourceManager::Instance()->FindTexture(texture_tag_);

			// 바꿀 위치 찾기
			TILE_OPTION tile_option = pt_texture->GetOptionVec(mouse_down_pose_x_idx, mouse_down_pose_y_idx);

			// 어떤 것으로 바꿀지 선정 (옵션 & 텍스쳐)
			string texture_str;
			switch (tile_option)
			{
			case TILE_NONE:
				tile_option = TILE_NOMOVE;
				texture_str = X_32;
				break;
			case TILE_NOMOVE:
				tile_option = TILE_TRANSPORT;
				texture_str = T_32;
				break;
			case TILE_TRANSPORT:
				tile_option = TILE_NONE;
				texture_str = O_32;
				break;
			}

			// 텍스쳐 바꾸기
			CLayer* pt_layer = FindLayer(UI_LAYER);
			CObject* pt_obj = pt_layer->GetObj(mouse_down_pose_x_idx + mouse_down_pose_y_idx * (world_size_.x / TEXTURE_SIZE));
			pt_obj->SetTexture(texture_str);

			// 정한 타일 옵션으로 해당 위치 특성 변경
			pt_texture->SetOptionVec(tile_option, mouse_down_pose_x_idx, mouse_down_pose_y_idx);

			SAFE_RELEASE(pt_obj);
			SAFE_RELEASE(pt_texture);
		}
	}

	

}

void CTileSetSettingScene::Update(float _time)
{
	CMapToolScene::Update(_time);
}

void CTileSetSettingScene::LateUpdate(float _time)
{
	CMapToolScene::LateUpdate(_time);
}

void CTileSetSettingScene::Collision(float _time)
{
	CMapToolScene::Collision(_time);
}

void CTileSetSettingScene::Render(HDC _hdc, float _time)
{
	CMapToolScene::Render(_hdc, _time);
}
