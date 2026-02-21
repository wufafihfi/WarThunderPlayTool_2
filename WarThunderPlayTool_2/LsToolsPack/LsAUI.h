#ifndef LSAUI
#define LSAUI

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <future>
#include <graphics.h>

#include "LsMath.h"
#include "LsInput.h"

#define LS_UI_SIZE 0x001
#define LS_UI_CLOSE 0x002
#define LS_UI_CHILD 0x003
#define LS_UI_TITLE 0x004
#define LS_UI_TITLEBAR 0x005
#define LS_UI_SCREENSIDEHAIDE 0x006

#define LS_SLIDER_TRANSVERSE 0x007
#define LS_SLIDER_VERTICAL 0x008
#define LS_SLIDER_FREEXYMODE 0x009

namespace LsAui {

    //平滑运动 计算速度
    template <typename T>
    LsMath::Vector2<T> smoothMovementV2(LsMath::Vector2<T> s_v2, T move_time) {
        return LsMath::Vector2<T>(s_v2.x / move_time, s_v2.y / move_time);
    }
    template <typename T>
    T smoothMovement(T s, T move_time) {
        return (s / move_time);
    }

    class Style {
    public:
        std::function<void()> style1_lambda;
        std::function<void()> style2_lambda;
        std::function<void()> style3_lambda;
    };

    class LsBotton {
    public:
        LsMath::Vector2d BdrawPosition = { 0,0 };

        std::vector<LsMath::Vector2d> collidePlygon;
        std::vector<LsMath::Vector2d> collidePlygonWordPs;

        Style bottonStyle;

        bool style1 = false;
        bool style2 = false;
        bool style3 = false;

        void CpolyWordPs();

        void Botton(bool collider, bool trigger, bool debugMode);

        void SetBotton(LsMath::Vector2d SETBdrawPosition, std::vector<LsMath::Vector2d> SETcollidePlygon);
    };

    class LsSlider {
    public:
        LsMath::LsRectd SliderBackRect;

        LsMath::Vector2d BottonOffset;
        LsMath::Vector2d ActualVar;
        LsMath::Vector2d Actual_Offset$Ratio;
        LsMath::Vector2d solidroundRD;
        LsMath::Vector2d SliderPosition;

        std::function<void(LsMath::Vector2d &SliderPosition)> style1_lambda;
        std::function<void(LsMath::Vector2d &SliderPosition)> style2_lambda;

        COLORREF lineColor = RGB(0, 255, 255);
        COLORREF fillColor = RGB(255,0,255);

        bool solidroundRectMode = false;

        int interactionMode = LS_SLIDER_TRANSVERSE;
        
        std::string Sstatute = "normal";

        LsInput::FHInput Sinp1;

        void setSlider(LsMath::LsRectd _SliderBackRect, COLORREF _lineColor, COLORREF _fillColor, LsMath::Vector2d realVar_Max);

        void Slider(LsMath::Vector2d _mousePosition, bool trigger, bool debugMode, bool Holding);
    };

	class A_UI_Win {
	public:
		LsMath::LsRectd UIRect;
        LsMath::LsRectd UItitleBar;

        LsMath::LsRectd adsorbedRect;

		LsMath::Vector2d UIspeed;
		LsMath::Vector2d UIacceleration;
        LsMath::Vector2d solidroundRD;

        std::string Uistatute;
		std::string UiName;

        bool IsColliding = false;

        std::function<void()> UIfunction;

        A_UI_Win(LsMath::LsRectd _UIRect):UIRect(_UIRect) {
            UItitleBar = LsMath::LsRectd(UIRect.x, UIRect.y, UIRect.width, 10);
        }

        void A_UI_UpData(LsMath::Vector2d Pv2);
	};
}

#endif