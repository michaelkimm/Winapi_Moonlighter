#pragma once

#ifndef CLAYER_H_
#define CLAYER_H_

#include "..\Game.h"
#include "Scene.h"

class CLayer
{
private:
	// 씬 클래스에서 CLayer를 생성할 것이기 때문에 프렌드 클래스 생성
	friend class CScene;

	int tile_x_num_;
	int tile_y_num_;
	int tile_width_;
	int tile_height_;

public:
	void SetTileXNum(int _x_num) { tile_x_num_ = _x_num; }
	void SetTileYNum(int _y_num) { tile_y_num_ = _y_num; }
	void SetTileWidth(int _width) { tile_width_ = _width; }
	void SetTileHeight(int _height) { tile_height_ = _height; }

	int GetTileXNum() const { return tile_x_num_; }
	int GetTileYNum() const { return tile_y_num_; }
	int GetTileWidth() const { return tile_width_; }
	int GetTileHeight() const { return tile_height_; }

private:
	class CScene* pt_scene_;
	string	str_tag_;
	int		z_order_;
	vector<class CObject*> obj_list_;

private:
	CLayer();
	~CLayer();

public:
	void SetScene(class CScene* _scene) { pt_scene_ = _scene; }
	void SetTag(const string& _str_tag) { str_tag_ = _str_tag; }
	void SetZOrder(int _z_order) { z_order_ = _z_order; }

	CScene* GetScene() { return pt_scene_; }
	int		GetZOrder() { return z_order_; }
	string	GetTag() { return str_tag_; }

public:
	void AddObj(class CObject* obj);
	CObject* GetObj(int _idx) const;
	void Clear();

public:
	bool Init();
	void Input(float _time);
	void Update(float _time);
	void LateUpdate(float _time);
	void Collision(float _time);
	void Render(HDC _hdc, float _time);

public:
	template <typename T>
	T* FindObj(const string& _str_tag)
	{
		typename list<class CObject*>::iterator iter;
		typename list<class CObject*>::iterator iter_end = obj_list_.end();

		for (iter = obj_list_.begin(); iter != iter_end; iter++)
		{
			if ((*iter)->GetTag() == _str_tag)
			{
				(*iter)->AddRef();
				return static_cast<T*>(*iter);
			}
		}
		return NULL;
	}

	bool CreateTile(const MY_POSE& _start_pose, int _num_x, int _num_y, int _size_x, int _size_y,
		const string& _texture_key, const wchar_t* _file_name, const string& _root_str);
	bool CreateTileSpriteSheet(const MY_POSE& _start_pose, int _size_x, int _size_y,
		const string& _texture_key, const wchar_t* _file_name, const string& _root_str);
};

#endif