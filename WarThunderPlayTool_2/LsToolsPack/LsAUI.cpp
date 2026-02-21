#include "LsAUI.h"

namespace LsAui {

	void LsBotton::CpolyWordPs() {
		collidePlygonWordPs.clear();
		for (LsMath::Vector2d point : collidePlygon) {
			point.x += BdrawPosition.x;
			point.y += BdrawPosition.y;
			collidePlygonWordPs.push_back(point);
		}
	}
	void LsBotton::Botton(bool collider, bool trigger, bool debugMode) {
		CpolyWordPs();
		style1 = false;
		style2 = false;
		style3 = false;

		if (collider) {
			style2 = true;
			if (trigger) {
				style3 = true;
				bottonStyle.style3_lambda();
			}
			else
			{
				bottonStyle.style2_lambda();
			}
		}
		else {
			style1 = true;
			bottonStyle.style1_lambda();
		}

		if (debugMode) {
			setlinecolor(RGB(0, 255, 0));
			std::vector<POINT> polyP;
			for (int i = 0; i < collidePlygonWordPs.size(); i++)
			{
				std::cout << i << std::endl;
				polyP[i] = { (long)collidePlygonWordPs[i].x,(long)collidePlygonWordPs[i].y };
			}
			polygon(polyP.data(), polyP.size());
		}
	}
	void LsBotton::SetBotton(LsMath::Vector2d SETBdrawPosition, std::vector<LsMath::Vector2d> SETcollidePlygon) {
		BdrawPosition = SETBdrawPosition;
		collidePlygon = SETcollidePlygon;
	}

	void LsSlider::setSlider(LsMath::LsRectd _SliderBackRect, COLORREF _lineColor, COLORREF _fillColor, LsMath::Vector2d realVar_Max) {
		SliderBackRect = _SliderBackRect;
		lineColor = _lineColor;
		fillColor = _fillColor;
		BottonOffset = LsMath::Vector2d(SliderBackRect.width / 2, SliderBackRect.height / 2);
		Actual_Offset$Ratio = LsMath::Vector2d(realVar_Max.x / SliderBackRect.width, realVar_Max.y / SliderBackRect.height);
	}
	void LsSlider::Slider(LsMath::Vector2d _mousePosition, bool trigger, bool debugMode,bool Holding) {
		SliderPosition = LsMath::Vector2d(SliderBackRect.x + BottonOffset.x, SliderBackRect.y + BottonOffset.y);

		if (!solidroundRectMode)
		{
			setlinecolor(lineColor);
			setfillcolor(fillColor);
			fillrectangle(SliderBackRect.x, SliderBackRect.y, SliderBackRect.right(), SliderBackRect.bottom());
		}
		else
		{
			setlinecolor(lineColor);
			setfillcolor(fillColor);
			solidroundrect(SliderBackRect.x, SliderBackRect.y, SliderBackRect.right(), SliderBackRect.bottom(),
				solidroundRD.x, solidroundRD.y);
		}
		if(SliderBackRect.contains(_mousePosition))
		{
			if (trigger && Sstatute == "normal" && !Holding) {
				Sstatute = "Holding";
			}
			style1_lambda(SliderPosition);
		}
		if (Sstatute == "saving") {
			Sstatute = "normal";
		}
		if (Sinp1.GetKeyRelese(trigger) && Sstatute == "Holding") {
			Sstatute = "saving";
		}
		if (Sstatute == "Holding") {
			if (interactionMode == LS_SLIDER_VERTICAL) {
				BottonOffset = LsMath::Vector2d(SliderBackRect.width / 2, _mousePosition.y - SliderBackRect.y);
			}
			if (interactionMode == LS_SLIDER_TRANSVERSE) {
				BottonOffset = LsMath::Vector2d(_mousePosition.x - SliderBackRect.x, SliderBackRect.height / 2);
			}
			if (interactionMode == LS_SLIDER_FREEXYMODE) {
				BottonOffset = LsMath::Vector2d(_mousePosition.x - SliderBackRect.x, _mousePosition.y - SliderBackRect.y);
			}
			style1_lambda(SliderPosition);
		}
		else {
			style2_lambda(SliderPosition);
		}
		if (BottonOffset.x > SliderBackRect.width) {
			BottonOffset.x = SliderBackRect.width;
		}
		if (BottonOffset.y > SliderBackRect.height) {
			BottonOffset.y = SliderBackRect.height;
		}
		if (BottonOffset.x < 0) {
			BottonOffset.x = 0;
		}
		if (BottonOffset.y < 0) {
			BottonOffset.y = 0;
		}
		

		ActualVar = LsMath::Vector2d(BottonOffset.x * Actual_Offset$Ratio.x,
			BottonOffset.y * Actual_Offset$Ratio.y);
	}

	void A_UI_Win::A_UI_UpData(LsMath::Vector2d Pv2) {
		UIfunction();
		if (UIRect.contains(LsMath::Vector2d(Pv2.x, Pv2.y))) {
			IsColliding = true;
		}
		else
		{
			IsColliding = false;
		}
	}
}