// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//


//���� ���̺귯�� �ε��Դϴ�.
#pragma once
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "WindowsCodecs.lib")
#pragma comment(lib, "dwrite.lib")

#include "../targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

// ���� ���� �����Դϴ�.
#define _CRT_RAND_S

// c++ ��� �����Դϴ�.
#include <string>
#include <cassert>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <windows.h>
#include <time.h>

//DirectX ���� ��� �����Դϴ�.
#include <wincodec.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

//Ŀ�ǵ� ���� �����Դϴ�.
constexpr int IDC_BTN_START = 1003;
constexpr int IDC_BTN_END = 1004;
constexpr int IDC_GAME_CARD = 1010;
constexpr int IDC_CARD_CLOVER = 1011;
constexpr int IDC_CARD_DIAMOND = 1012;
constexpr int IDC_CARD_HEART = 1013;
constexpr int IDC_CARD_SPADE = 1014;
constexpr int IDC_GAME_YUT = 1020;
constexpr int IDC_GAME_OMOK = 1030;
constexpr int IDC_GAME_SPIDER = 1040;

//�ػ� ������ �Դϴ�.
constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 768;

//�������� ���Դϴ�.
constexpr unsigned int START_GAME = 0;
constexpr unsigned int SELECT_GAME = 1;
constexpr unsigned int CARD_GAME = 100;
constexpr unsigned int CARD_GAME_END = 101;
constexpr unsigned int YUT_GAME = 200;
constexpr unsigned int YUT_GAME_END = 201;
constexpr unsigned int OMOK_GAME = 300;
constexpr unsigned int OMOK_GAME_END = 301;
constexpr unsigned int SPIDER_GAME = 400;
constexpr unsigned int SPIDER_GAME_END = 401;

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.