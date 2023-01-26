// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//


//���� ���̺귯�� �ε��Դϴ�.
#pragma once
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "WindowsCodecs.lib")

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>
#include <string>
#include <cassert>
#include <iostream>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//DirectX ���� ��� �����Դϴ�.
#include <wincodec.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

//Ŀ�ǵ� ���� �����Դϴ�.
constexpr int IDC_BTN_START = 1003;
constexpr int IDC_BTN_END = 1004;
constexpr int IDC_TEXT_WORD = 1005;
constexpr int IDC_BTN_ENTER = 1006;
constexpr int IDC_UPDATE_WORD = 1007;

//�ػ� ������ �Դϴ�.
constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 768;


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.