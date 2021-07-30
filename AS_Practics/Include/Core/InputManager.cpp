#include "InputManager.h"

DEFINE_SINGLETON(CInputManager)

CInputManager::CInputManager()
{
}

CInputManager::~CInputManager()
{
}

void CInputManager::SetMousePose(LPARAM _lParam)
{
	mouse_pose_ = MY_POSE(GET_X_LPARAM(_lParam), GET_Y_LPARAM(_lParam));
}

bool CInputManager::Init(HWND _hWnd)
{
	KeyBoardStateReset();
	MouseStateReset();

	hWnd_ = _hWnd;

	return true;
}

void CInputManager::KeyBoardInput(float _time)
{
	KeyBoardStateReset();

	// ���� ������Ʈ
	key_a_ = (GetKeyState(0x41) & 0x8000);
	key_w_ = (GetKeyState(0x57) & 0x8000);
	key_d_ = (GetKeyState(0x44) & 0x8000);
	key_s_ = (GetKeyState(0x53) & 0x8000);

	key_1_ = (GetKeyState(0x31) & 0x8000);
	key_2_ = (GetKeyState(0x32) & 0x8000);
	key_3_ = (GetKeyState(0x33) & 0x8000);
}

void CInputManager::KeyBoardStateReset()
{
	// ���� �ʱ�ȭ
	key_a_ = false;
	key_w_ = false;
	key_d_ = false;
	key_s_ = false;

	key_1_ = false;
	key_2_ = false;
	key_3_ = false;
}

void CInputManager::MouseInput(float _time)
{
	MouseStateReset();

	// ���� ������Ʈ
	mouse_left_down_ = (GetKeyState(VK_LBUTTON) & 0x8000);
	mouse_right_down_ = (GetKeyState(VK_RBUTTON) & 0x8000);

	POINT tmp_pt;
	GetCursorPos(&tmp_pt);
	ScreenToClient(hWnd_, &tmp_pt);

	mouse_pose_ = tmp_pt;
}

void CInputManager::MouseInput(MOUSE_STATE _state,LPARAM _lParam)
{
	// ���� ������Ʈ
	switch (_state)
	{
	case LBUTTON_UP:
		mouse_left_up_ = true;
		break;
	case LBUTTON_DOWN:
		mouse_left_down_ = true;
		break;
	case RBUTTON_UP:
		mouse_right_up_ = true;
		break;
	case RBUTTON_DOWN:
		mouse_right_down_ = true;
		break;
	default:
		break;
	}

	SetMousePose(_lParam);
}

void CInputManager::MouseStateReset()
{
	// ���� �ʱ�ȭ
	/*if (mouse_left_up_)
		mouse_left_down_ = false;
	mouse_left_up_ = false;
	if (mouse_right_up_)
		mouse_right_down_ = false;
	mouse_right_up_ = false;*/

	mouse_left_down_ = false;
	mouse_left_up_ = false;
	mouse_right_down_ = false;
	mouse_right_up_ = false;

	
	mouse_pose_ = MY_POSE{ 0.f, 0.f };
}

void CInputManager::Update(float _time)
{
}