// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//


//동적 라이브러리 로딩입니다.
#pragma once
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "WindowsCodecs.lib")
#pragma comment(lib, "dwrite.lib")

#include "../targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

// 랜덤 관련 선언입니다.
#define _CRT_RAND_S

// c++ 헤더 파일입니다.
#include <string>
#include <cassert>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <windows.h>
#include <time.h>

//DirectX 관련 헤더 파일입니다.
#include <wincodec.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

//커맨드 관련 변수입니다.
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

//해상도 사이즈 입니다.
constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 768;

//스테이지 값입니다.
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

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.