#include <windows.h>
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00
#include <graphics.h>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <conio.h>
#include <sstream>
#include <memory>
#include <commctrl.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <json/json.h>
#include <ShellScalingApi.h>		// 引用头文件
#pragma comment(lib, "Shcore.lib")	// 链接库文件
//#pragma comment(lib, "comctl32.lib")

#include "LsToolsPack/LsToolsAll.hpp"
#include <codecvt>

LsDrawEX::DrawData MainWindow(
	GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
	{ 0,0,0 },
	false
);

// 读取JSON文件
bool readJsonFile(const std::string& filename, Json::Value& root) {
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "无法打开文件: " << filename << std::endl;
		return false;
	}

	Json::CharReaderBuilder readerBuilder;
	std::string parseErrors;

	// 使用二进制模式读取，避免换行符问题
	bool success = Json::parseFromStream(readerBuilder, file, &root, &parseErrors);
	file.close();

	if (!success) {
		std::cerr << "解析JSON失败: " << parseErrors << std::endl;
		return false;
	}

	return true;
}

// 保存JSON文件
bool saveJsonFile(const std::string& filename, const Json::Value& root) {
	std::ofstream file(filename);
	if (!file.is_open()) {
		std::cerr << "无法创建文件: " << filename << std::endl;
		return false;
	}

	Json::StreamWriterBuilder writerBuilder;
	writerBuilder["indentation"] = "  "; // 缩进2个空格
	writerBuilder["commentStyle"] = "None"; // 不保存注释

	try {
		std::unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());
		writer->write(root, &file);
		file.close();

		std::cout << "文件已保存: " << filename << std::endl;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "保存文件时发生错误: " << e.what() << std::endl;
		file.close();
		return false;
	}
}

std::tm getTime_Chrono() {
	// 获取当前时间点
	auto now = std::chrono::system_clock::now();

	// 转换为time_t类型
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);

	// 转换为本地时间结构体
	std::tm local_time;
	localtime_s(&local_time, &now_time);

	return local_time;
}

std::string GetExePath()
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	/*
	strrchr:函数功能：查找一个字符c在另一个字符串str中末次出现的位置（也就是从str的右侧开始查找字符c首次出现的位置），
	并返回这个位置的地址。如果未能找到指定字符，那么函数将返回NULL。
	使用这个地址返回从最后一个字符c到str末尾的字符串。
	*/
	(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串//
	std::string path = szFilePath;
	return path;
}
std::string BasePath = GetExePath();

POINT mouseV2 = { 0,0 };
bool isWindowActive = false;

std::string MainWindowName = "_warthunderPlayTool_2_";
bool closeWindow = false;

LsDrawEX::LsRGB timeTextColor;
LsMath::Vector2d timeTextPosition(MainWindow.WindowW / 2, 0);

BYTE windowAlphaVar = 255;
int main() {
	HWND targetWindow = FindWindowA(NULL, MainWindowName.c_str());
	if (targetWindow != NULL) {
		MessageBoxW(NULL, _T("错误码:10086\nBZD_EXEERRO_REOPEN\n程序正在运行,无法重复打开此程序"), _T("错误"), MB_OK | MB_ICONERROR);
		return 10086;
	}
	//SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
	MainWindow.WindowW = GetSystemMetrics(SM_CXSCREEN);
	MainWindow.WindowH = GetSystemMetrics(SM_CYSCREEN);
	HWND main_hwnd = LsDrawEX::initDraw(MainWindow);
	LsWS::setWindowTransparent_Tool(main_hwnd, MainWindow.bkColor.getRGB_EasyX(), 200);
	setbkmode(TRANSPARENT);
	SetWindowTextA(main_hwnd, MainWindowName.c_str());

	timeTextColor.r = 255;
	timeTextColor.g = 255;
	timeTextColor.b = 255;

	LsAui::LsSlider color_R_slider;
	color_R_slider.setSlider(LsMath::LsRectd(0, 0, 200, 10), RGB(200, 200, 200), RGB(200, 200, 200), LsMath::Vector2d(255, 0));
	color_R_slider.solidroundRectMode = true;
	color_R_slider.solidroundRD = LsMath::Vector2d(10, 10);
	color_R_slider.style1_lambda = [&](LsMath::Vector2d& SliderPosition) {
		setfillcolor(RGB(255, 255, 255));
		setlinecolor(RGB(255, 255, 255));
		fillcircle(SliderPosition.x, SliderPosition.y
			, 10);
		};
	color_R_slider.style2_lambda = [&](LsMath::Vector2d& SliderPosition) {
		setfillcolor(RGB(100, 100, 100));
		setlinecolor(RGB(100, 100, 100));
		fillcircle(SliderPosition.x, SliderPosition.y
			, 10);
		};

	LsAui::LsSlider color_G_slider;
	color_G_slider.setSlider(LsMath::LsRectd(0, 0, 200, 10), RGB(200, 200, 200), RGB(200, 200, 200), LsMath::Vector2d(255, 0));
	color_G_slider.solidroundRectMode = true;
	color_G_slider.solidroundRD = LsMath::Vector2d(10, 10);
	color_G_slider.style1_lambda = [&](LsMath::Vector2d& SliderPosition) {
		setfillcolor(RGB(255, 255, 255));
		setlinecolor(RGB(255, 255, 255));
		fillcircle(SliderPosition.x, SliderPosition.y
			, 10);
		};
	color_G_slider.style2_lambda = [&](LsMath::Vector2d& SliderPosition) {
		setfillcolor(RGB(100, 100, 100));
		setlinecolor(RGB(100, 100, 100));
		fillcircle(SliderPosition.x, SliderPosition.y
			, 10);
		};

	LsAui::LsSlider color_B_slider;
	color_B_slider.setSlider(LsMath::LsRectd(0, 0, 200, 10), RGB(200, 200, 200), RGB(200, 200, 200), LsMath::Vector2d(255, 0));
	color_B_slider.solidroundRectMode = true;
	color_B_slider.solidroundRD = LsMath::Vector2d(10, 10);
	color_B_slider.style1_lambda = [&](LsMath::Vector2d& SliderPosition) {
		setfillcolor(RGB(255, 255, 255));
		setlinecolor(RGB(255, 255, 255));
		fillcircle(SliderPosition.x, SliderPosition.y
			, 10);
		};
	color_B_slider.style2_lambda = [&](LsMath::Vector2d& SliderPosition) {
		setfillcolor(RGB(100, 100, 100));
		setlinecolor(RGB(100, 100, 100));
		fillcircle(SliderPosition.x, SliderPosition.y
			, 10);
		};

	Json::Value configData;
	if (!readJsonFile(BasePath + "\\config.json", configData)) {
		MessageBoxW(NULL, _T("错误码:114\nBZD_CONFIGERRO_READFAILURE\n配置文件读取失败"), _T("错误"), MB_OK | MB_ICONERROR);
		return 114;
	}

	timeTextColor.r = configData["savedRGB"]["r"].asDouble();
	timeTextColor.g = configData["savedRGB"]["g"].asDouble();
	timeTextColor.b = configData["savedRGB"]["b"].asDouble();
	color_R_slider.BottonOffset.x = timeTextColor.r / color_R_slider.Actual_Offset$Ratio.x;
	color_G_slider.BottonOffset.x = timeTextColor.g / color_G_slider.Actual_Offset$Ratio.x;
	color_B_slider.BottonOffset.x = timeTextColor.b / color_B_slider.Actual_Offset$Ratio.x;


	LsAui::LsSlider timeTextPosition_slider;
	timeTextPosition_slider.setSlider(LsMath::LsRectd(0, 0, 180, 180), RGB(200, 200, 200), RGB(200, 200, 200), LsMath::Vector2d(MainWindow.WindowW, MainWindow.WindowH));
	timeTextPosition_slider.solidroundRectMode = true;
	timeTextPosition_slider.solidroundRD = LsMath::Vector2d(20, 20);
	timeTextPosition_slider.interactionMode = LS_SLIDER_FREEXYMODE;
	timeTextPosition_slider.style1_lambda = [&](LsMath::Vector2d& SliderPosition) {
		setfillcolor(RGB(255, 255, 255));
		setlinecolor(RGB(255, 255, 255));
		fillcircle(SliderPosition.x, SliderPosition.y
			, 10);
		};
	timeTextPosition_slider.style2_lambda = [&](LsMath::Vector2d& SliderPosition) {
		setfillcolor(RGB(100, 100, 100));
		setlinecolor(RGB(100, 100, 100));
		fillcircle(SliderPosition.x, SliderPosition.y
			, 10);
		};

	timeTextPosition.x = configData["saveTimeTPosition"]["x"].asDouble();
	timeTextPosition.y = configData["saveTimeTPosition"]["y"].asDouble();
	timeTextPosition_slider.BottonOffset.x = timeTextPosition.x / timeTextPosition_slider.Actual_Offset$Ratio.x;
	timeTextPosition_slider.BottonOffset.y = timeTextPosition.y / timeTextPosition_slider.Actual_Offset$Ratio.y;

	LsAui::LsSlider WindowAlpa_slider;
	WindowAlpa_slider.setSlider(LsMath::LsRectd(0, 0, 200, 10), RGB(200, 200, 200), RGB(200, 200, 200), LsMath::Vector2d(255, 0));
	WindowAlpa_slider.solidroundRectMode = true;
	WindowAlpa_slider.solidroundRD = LsMath::Vector2d(10, 10);
	WindowAlpa_slider.style1_lambda = [&](LsMath::Vector2d& SliderPosition) {
		setfillcolor(RGB(255, 255, 255));
		setlinecolor(RGB(255, 255, 255));
		fillcircle(SliderPosition.x, SliderPosition.y
			, 10);
		};
	WindowAlpa_slider.style2_lambda = [&](LsMath::Vector2d& SliderPosition) {
		setfillcolor(RGB(100, 100, 100));
		setlinecolor(RGB(100, 100, 100));
		fillcircle(SliderPosition.x, SliderPosition.y
			, 10);
		};

	windowAlphaVar = configData["savedWinAlpha"].asDouble();
	WindowAlpa_slider.BottonOffset.x = windowAlphaVar / WindowAlpa_slider.Actual_Offset$Ratio.x;


	LsAui::A_UI_Win myUi1(LsMath::LsRectd(MainWindow.WindowW - 20, MainWindow.WindowH / 2 - 50, 40, 40));
	myUi1.adsorbedRect = LsMath::LsRectd(100, 100, MainWindow.WindowW - 200, MainWindow.WindowH - 200);
	myUi1.solidroundRD = LsMath::Vector2d(15, 15);
	myUi1.Uistatute = "small";
	myUi1.UIfunction = [&]() {
		if (myUi1.IsColliding || myUi1.Uistatute == "followMouseL" || myUi1.Uistatute == "followMouse") {
			if (GetWindowLong(main_hwnd, GWL_EXSTYLE) & WS_EX_TRANSPARENT)
			{
				SetWindowLong(main_hwnd, GWL_EXSTYLE, GetWindowLong(main_hwnd, GWL_EXSTYLE) & ~WS_EX_TRANSPARENT);
			}
		}
		else
		{
			if (!(GetWindowLong(main_hwnd, GWL_EXSTYLE) & WS_EX_TRANSPARENT))
			{
				SetWindowLong(main_hwnd, GWL_EXSTYLE, GetWindowLong(main_hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
			}
		}

		static LsInput::FHInput Inp1;
		static LsMath::Vector2d targetHW(100, 100);
		static LsMath::Vector2d uiRv2(0, 0);
		static LsMath::Vector2d UiPositionSave(0, 0);
		static bool Saved = false;
		static LsMath::Vector2d uiPv2(0, 0);
		static LsMath::Vector2d targetXY(0, 0);
		if (isWindowActive) {
			if (myUi1.IsColliding) {
				if (GetAsyncKeyState(VK_LBUTTON))
				{
					if (myUi1.Uistatute == "small") {

					}
					if (myUi1.Uistatute != "large" && myUi1.Uistatute != "followMouseL") {
						myUi1.Uistatute = "followMouse";
					}
					else {
						LsMath::LsRectd titileBarCollection = myUi1.UItitleBar;
						//titileBarCollection.width = titileBarCollection.width - myUi1.UItitleBar.height;
						if (titileBarCollection.contains(mouseV2.x, mouseV2.y)
							&& color_R_slider.Sstatute != "Holding"
							&& color_G_slider.Sstatute != "Holding"
							&& color_B_slider.Sstatute != "Holding"
							&& timeTextPosition_slider.Sstatute != "Holding"
							&& WindowAlpa_slider.Sstatute != "Holding") {
							if (!Saved)
							{
								Saved = true;
								UiPositionSave = LsMath::Vector2d(mouseV2.x, mouseV2.y) - myUi1.UItitleBar.center();
							}
							myUi1.Uistatute = "followMouseL";
						}
					}
				}
			}
			else
			{

			}
		}
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			if (myUi1.Uistatute == "followMouse" || myUi1.Uistatute == "followMouseL")
			{
				targetXY = LsMath::Vector2d(mouseV2.x, mouseV2.y);
			}
		}
		if (Inp1.GetKeyRelese(GetAsyncKeyState(VK_LBUTTON)))
		{
			//if (myUi1.Uistatute == "followMouseL")
			//{
			Saved = false;
			//}
		}

		if (myUi1.IsColliding) {
			if (myUi1.Uistatute == "small") {
				myUi1.Uistatute = "toBig";
			}
		}
		else
		{
			if (myUi1.Uistatute == "Big") {
				myUi1.Uistatute = "toSmall";
			}
		}

		if (myUi1.Uistatute == "large") {
			if (!myUi1.adsorbedRect.contains(targetXY)) {
				myUi1.Uistatute = "toSmall";
			}
			if (myUi1.UIRect.y < 0) {
				static LsMath::Vector2d SttargetXY = LsMath::Vector2d(myUi1.UIRect.x, 0);
				static LsMath::Vector2d StuiPv2 = LsAui::smoothMovementV2(LsMath::Vector2d(
					SttargetXY.x - myUi1.UIRect.x,
					SttargetXY.y - myUi1.UIRect.y
				), 10.0);
				myUi1.UIRect = myUi1.UIRect.translated(StuiPv2.x, StuiPv2.y);
			}
		}

		if (myUi1.Uistatute == "toBig") {
			targetHW = LsMath::Vector2d(100, 100);
		}
		if (myUi1.Uistatute == "small") {
			if (myUi1.UIRect.center().x < myUi1.adsorbedRect.x) {
				targetXY = LsMath::Vector2d(0, myUi1.UIRect.center().y);
			}
			if (myUi1.UIRect.center().x > myUi1.adsorbedRect.right()) {
				targetXY = LsMath::Vector2d(MainWindow.WindowW, myUi1.UIRect.center().y);
			}
			if (myUi1.UIRect.center().y < myUi1.adsorbedRect.y) {
				targetXY = LsMath::Vector2d(myUi1.UIRect.center().x, 0);
			}
			if (myUi1.UIRect.center().y > myUi1.adsorbedRect.bottom()) {
				targetXY = LsMath::Vector2d(myUi1.UIRect.center().x, MainWindow.WindowH);
			}
		}
		if (myUi1.Uistatute == "toSmall") {
			targetHW = LsMath::Vector2d(40, 40);
			if (myUi1.UIRect.center().x < myUi1.adsorbedRect.x) {
				targetXY = LsMath::Vector2d(0, myUi1.UIRect.center().y);
			}
			if (myUi1.UIRect.center().x > myUi1.adsorbedRect.right()) {
				targetXY = LsMath::Vector2d(MainWindow.WindowW, myUi1.UIRect.center().y);
			}
			if (myUi1.UIRect.center().y < myUi1.adsorbedRect.y) {
				targetXY = LsMath::Vector2d(myUi1.UIRect.center().x, 0);
			}
			if (myUi1.UIRect.center().y > myUi1.adsorbedRect.bottom()) {
				targetXY = LsMath::Vector2d(myUi1.UIRect.center().x, MainWindow.WindowH);
			}
		}
		if (myUi1.Uistatute == "toLarge") {
			targetHW = LsMath::Vector2d(500, 300);
		}
		if (myUi1.Uistatute == "toBig" || myUi1.Uistatute == "toSmall" || myUi1.Uistatute == "toLarge") {
			uiRv2 = LsAui::smoothMovementV2(LsMath::Vector2d(
				targetHW.x - myUi1.UIRect.width,
				targetHW.y - myUi1.UIRect.height
			), 10.0);
			myUi1.UIRect = myUi1.UIRect.expanded(uiRv2.x, uiRv2.y);
			if (uiRv2.length() <= 0.01) {
				if (myUi1.Uistatute == "toBig") {
					myUi1.Uistatute = "Big";
					uiRv2 = LsMath::Vector2d(0, 0);
				}
				if (myUi1.Uistatute == "toSmall") {
					myUi1.Uistatute = "small";
					uiRv2 = LsMath::Vector2d(0, 0);
				}
				if (myUi1.Uistatute == "toLarge") {
					myUi1.Uistatute = "large";
					uiRv2 = LsMath::Vector2d(0, 0);
				}
			}
		}
		if (myUi1.Uistatute == "followMouse" || myUi1.Uistatute == "followMouseL" || myUi1.Uistatute == "toSmall" || myUi1.Uistatute == "small") {
			if (myUi1.Uistatute == "followMouse" || myUi1.Uistatute == "toSmall" || myUi1.Uistatute == "small") {
				uiPv2 = LsAui::smoothMovementV2(LsMath::Vector2d(
					targetXY.x - myUi1.UIRect.center().x,
					targetXY.y - myUi1.UIRect.center().y
				), 10.0);
			}
			if (myUi1.Uistatute == "followMouseL") {
				uiPv2 = LsAui::smoothMovementV2(LsMath::Vector2d(
					targetXY.x - (myUi1.UItitleBar.center().x + UiPositionSave.x),
					targetXY.y - (myUi1.UItitleBar.center().y + UiPositionSave.y)
				), 10.0);
			}
			myUi1.UIRect = myUi1.UIRect.translated(uiPv2.x, uiPv2.y);
			if (uiPv2.length() <= 0.05 && myUi1.Uistatute != "small") {
				if (myUi1.Uistatute != "followMouseL")
				{
					myUi1.Uistatute = "toLarge";
				}
				else {
					if (!myUi1.adsorbedRect.contains(targetXY) && myUi1.Uistatute != "followMouseL") {
						myUi1.Uistatute = "toSmall";
					}
					else
					{
						myUi1.Uistatute = "large";
					}
				}
			}
		}

		if (myUi1.Uistatute == "toLarge") {
			if (!myUi1.adsorbedRect.contains(myUi1.UIRect.center())) {
				myUi1.Uistatute = "toSmall";
			}
		}
		//std::cout << myUi1.Uistatute << "  " << uiRv2 .x << "  " << targetHW.x - myUi1.UIRect.width << std::endl;
		//std::cout << myUi1.IsColliding << std::endl;

		//LsMath::Vector2d uiRv2 = LsAui::smoothMovementV2(LsMath::Vector2d(1000 - myUi1.UIRect.x - myUi1.UIRect.width/2 + 30, 200 - myUi1.UIRect.y - myUi1.UIRect.height / 2 - 10), 10.0);
		//UIRect = myUi1.UIRect.translated(uiRv2);
		//std::cout << uiRv2 << myUi1.UIRect << std::endl;

		setfillcolor(RGB(0, 150, 150));
		setlinecolor(RGB(0, 150, 150));
		solidroundrect(myUi1.UIRect.x, myUi1.UIRect.y, myUi1.UIRect.right(), myUi1.UIRect.bottom(),
			myUi1.solidroundRD.x, myUi1.solidroundRD.y);

		if (myUi1.Uistatute == "large" || myUi1.Uistatute == "followMouseL") {
			myUi1.UItitleBar = LsMath::LsRectd(myUi1.UIRect.x, myUi1.UIRect.y, myUi1.UIRect.width, 20);
			setfillcolor(RGB(255, 255, 255));
			setlinecolor(RGB(255, 255, 255));
			fillrectangle(myUi1.UItitleBar.x, myUi1.UItitleBar.y, myUi1.UItitleBar.right(), myUi1.UItitleBar.bottom());

			int BBc = 4;
			TCHAR text[100];
			settextstyle(5 * BBc, 2 * BBc, _T("微软雅黑"), 0, 0, 100, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0);
			settextcolor(RGB(10, 10, 10));
			RECT ctr1 = { myUi1.UItitleBar.x, myUi1.UItitleBar.y, myUi1.UItitleBar.right(), myUi1.UItitleBar.bottom() };
			drawtext(_T("设置"), &ctr1, DT_VCENTER | DT_SINGLELINE | DT_VCENTER | DT_SINGLELINE);

			BBc = 5;
			settextstyle(5 * BBc, 2 * BBc, _T("微软雅黑"), 0, 0, 100, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0);
			color_R_slider.SliderBackRect.x = myUi1.UIRect.x + 10;
			color_R_slider.SliderBackRect.y = myUi1.UIRect.y + 50;
			settextcolor(RGB(255, 0, 0));
			_stprintf_s(text, _T("R %0.1f"), color_R_slider.ActualVar.x);
			ctr1 = { (long)color_R_slider.SliderBackRect.right(), (long)color_R_slider.SliderBackRect.y - 10,(long)color_R_slider.SliderBackRect.right() + 100, (long)color_R_slider.SliderBackRect.bottom() + 10 };
			drawtext(text, &ctr1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			color_R_slider.Slider(LsMath::Vector2d(mouseV2.x, mouseV2.y), (isWindowActive && (GetAsyncKeyState(VK_LBUTTON)))
				, false,
				myUi1.Uistatute == "followMouseL" || color_G_slider.Sstatute == "Holding" || color_B_slider.Sstatute == "Holding" || timeTextPosition_slider.Sstatute == "Holding" || WindowAlpa_slider.Sstatute == "Holding"
			);

			color_G_slider.SliderBackRect.x = myUi1.UIRect.x + 10;
			color_G_slider.SliderBackRect.y = myUi1.UIRect.y + 80;
			settextcolor(RGB(0, 255, 0));
			_stprintf_s(text, _T("G %0.1f"), color_G_slider.ActualVar.x);
			ctr1 = { (long)color_G_slider.SliderBackRect.right(), (long)color_G_slider.SliderBackRect.y - 10,(long)color_G_slider.SliderBackRect.right() + 100, (long)color_G_slider.SliderBackRect.bottom() + 10 };
			drawtext(text, &ctr1, DT_VCENTER | DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			color_G_slider.Slider(LsMath::Vector2d(mouseV2.x, mouseV2.y), (isWindowActive && (GetAsyncKeyState(VK_LBUTTON)))
				, false,
				myUi1.Uistatute == "followMouseL" || color_R_slider.Sstatute == "Holding" || color_B_slider.Sstatute == "Holding" || timeTextPosition_slider.Sstatute == "Holding" || WindowAlpa_slider.Sstatute == "Holding"
			);

			color_B_slider.SliderBackRect.x = myUi1.UIRect.x + 10;
			color_B_slider.SliderBackRect.y = myUi1.UIRect.y + 110;
			settextcolor(RGB(0, 0, 255));
			_stprintf_s(text, _T("B %0.1f"), color_B_slider.ActualVar.x);
			ctr1 = { (long)color_B_slider.SliderBackRect.right(), (long)color_B_slider.SliderBackRect.y - 10,(long)color_B_slider.SliderBackRect.right() + 100, (long)color_B_slider.SliderBackRect.bottom() + 10 };
			drawtext(text, &ctr1, DT_VCENTER | DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			color_B_slider.Slider(LsMath::Vector2d(mouseV2.x, mouseV2.y), (isWindowActive && (GetAsyncKeyState(VK_LBUTTON)))
				, false,
				myUi1.Uistatute == "followMouseL" || color_G_slider.Sstatute == "Holding" || color_R_slider.Sstatute == "Holding" || timeTextPosition_slider.Sstatute == "Holding" || WindowAlpa_slider.Sstatute == "Holding"
			);

			settextcolor(RGB(0, 255, 0));
			ctr1 = { (long)myUi1.UIRect.x + 10, (long)myUi1.UIRect.y + 160,(long)myUi1.UIRect.x + 300, (long)myUi1.UIRect.y + 200 };
			drawtext(_T("RGB值都为0则不显示数据"), &ctr1, DT_VCENTER | DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			ctr1 = { (long)myUi1.UIRect.x + 10, (long)myUi1.UIRect.y + 180,(long)myUi1.UIRect.x + 300, (long)myUi1.UIRect.y + 240 };
			drawtext(_T("将窗口拖动到屏幕边缘以缩小化"), &ctr1, DT_VCENTER | DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			timeTextColor.r = color_R_slider.ActualVar.x;
			timeTextColor.g = color_G_slider.ActualVar.x;
			timeTextColor.b = color_B_slider.ActualVar.x;

			std::cout << timeTextColor.g << color_G_slider.ActualVar << std::endl;

			static double showTimeSave = 0;
			if (color_R_slider.Sstatute == "saving" || color_G_slider.Sstatute == "saving" || color_B_slider.Sstatute == "saving") {
				configData["savedRGB"]["r"] = Json::Value(timeTextColor.r);
				configData["savedRGB"]["g"] = Json::Value(timeTextColor.g);
				configData["savedRGB"]["b"] = Json::Value(timeTextColor.b);
				saveJsonFile(BasePath + "\\config.json", configData);
				showTimeSave = clock();
			}

			timeTextPosition_slider.SliderBackRect.x = myUi1.UIRect.x + 310;
			timeTextPosition_slider.SliderBackRect.y = myUi1.UIRect.y + 50;
			timeTextPosition_slider.Slider(LsMath::Vector2d(mouseV2.x, mouseV2.y), (isWindowActive && (GetAsyncKeyState(VK_LBUTTON)))
				, false,
				myUi1.Uistatute == "followMouseL" || color_G_slider.Sstatute == "Holding" || color_R_slider.Sstatute == "Holding" || color_B_slider.Sstatute == "Holding" || WindowAlpa_slider.Sstatute == "Holding"
			);
			timeTextPosition.x = timeTextPosition_slider.ActualVar.x;
			timeTextPosition.y = timeTextPosition_slider.ActualVar.y;

			settextcolor(RGB(255, 255, 0));
			ctr1 = { (long)timeTextPosition_slider.SliderBackRect.x,(long)timeTextPosition_slider.SliderBackRect.y - 30,(long)timeTextPosition_slider.SliderBackRect.right(),(long)timeTextPosition_slider.SliderBackRect.y - 10 };
			drawtext(_T("数据显示的位置"), &ctr1, DT_VCENTER | DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			BBc = 4;
			settextstyle(5 * BBc, 2 * BBc, _T("微软雅黑"), 0, 0, 100, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0);
			settextcolor(RGB(10, 10, 10));
			_stprintf_s(text, _T("战雷游玩工具 2026/2/6 v1.0 EasyX:%s"), GetEasyXVer());
			ctr1 = { (long)myUi1.UIRect.x, (long)myUi1.UIRect.bottom() - 20,(long)myUi1.UIRect.right(), (long)myUi1.UIRect.bottom() };
			drawtext(text, &ctr1, DT_VCENTER | DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			if (timeTextPosition_slider.Sstatute == "saving") {
				configData["saveTimeTPosition"]["x"] = timeTextPosition.x;
				configData["saveTimeTPosition"]["y"] = timeTextPosition.y;
				saveJsonFile(BasePath + "\\config.json", configData);
				showTimeSave = clock();
			}

			WindowAlpa_slider.SliderBackRect.x = myUi1.UIRect.x + 10;
			WindowAlpa_slider.SliderBackRect.y = myUi1.UIRect.y + 140;
			BBc = 5;
			settextstyle(5 * BBc, 2 * BBc, _T("微软雅黑"), 0, 0, 100, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0);
			settextcolor(RGB(255, 255, 255));
			_stprintf_s(text, _T("A %0.1f"), WindowAlpa_slider.ActualVar.x);
			ctr1 = { (long)WindowAlpa_slider.SliderBackRect.right(), (long)WindowAlpa_slider.SliderBackRect.y - 10,(long)WindowAlpa_slider.SliderBackRect.right() + 100, (long)WindowAlpa_slider.SliderBackRect.bottom() + 10 };
			drawtext(text, &ctr1, DT_VCENTER | DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			WindowAlpa_slider.Slider(LsMath::Vector2d(mouseV2.x, mouseV2.y), (isWindowActive && (GetAsyncKeyState(VK_LBUTTON)))
				, false,
				myUi1.Uistatute == "followMouseL" || color_G_slider.Sstatute == "Holding" || color_R_slider.Sstatute == "Holding" || color_B_slider.Sstatute == "Holding" || timeTextPosition_slider.Sstatute == "Holding"
			);

			windowAlphaVar = WindowAlpa_slider.ActualVar.x;
			if (windowAlphaVar < 100) {
				windowAlphaVar = 100;
				WindowAlpa_slider.ActualVar.x = 100;
			}
			SetLayeredWindowAttributes(main_hwnd, 0, windowAlphaVar, LWA_ALPHA | LWA_COLORKEY);

			if (WindowAlpa_slider.Sstatute == "saving") {
				configData["savedWinAlpha"] = Json::Value(windowAlphaVar);
				saveJsonFile(BasePath + "\\config.json", configData);
				showTimeSave = clock();
			}


			if (!(clock() - showTimeSave > 1000))
			{
				BBc = 5;
				settextstyle(5 * BBc, 2 * BBc, _T("微软雅黑"), 0, 0, 100, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0);
				settextcolor(RGB(10, 10, 10));
				settextcolor(RGB(0, 255, 0));
				ctr1 = { (long)myUi1.UIRect.x + 10, (long)myUi1.UIRect.y + 240,(long)myUi1.UIRect.right(), (long)myUi1.UIRect.y + 280 };
				drawtext(_T("已保存!"), &ctr1, DT_VCENTER | DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

			LsMath::LsRectd closeBRect(myUi1.UItitleBar.right() - myUi1.UItitleBar.height, myUi1.UItitleBar.y, myUi1.UItitleBar.height, myUi1.UItitleBar.height);
			static LsInput::FHInput Inp2;
			if (myUi1.Uistatute != "followMouseL")
			{
				if (closeBRect.contains(LsMath::Vector2d(mouseV2.x, mouseV2.y)))
				{
					myUi1.Uistatute = "large";
					setfillcolor(RGB(255, 10, 10));
					setlinecolor(RGB(255, 10, 10));
					fillrectangle(closeBRect.x, closeBRect.y, closeBRect.right(), closeBRect.bottom());
					setlinecolor(RGB(10, 10, 10));
					line(closeBRect.x, closeBRect.y, closeBRect.right(), closeBRect.bottom());
					line(closeBRect.x, closeBRect.bottom(), closeBRect.right(), closeBRect.y);

					if (isWindowActive && GetAsyncKeyState(VK_LBUTTON) && !(color_G_slider.Sstatute == "Holding" || color_R_slider.Sstatute == "Holding" || color_B_slider.Sstatute == "Holding" || timeTextPosition_slider.Sstatute == "Holding")) {
						closeWindow = true;
					}
				}
				else
				{
					setfillcolor(RGB(100, 100, 100));
					setlinecolor(RGB(100, 100, 100));
					fillrectangle(closeBRect.x, closeBRect.y, closeBRect.right(), closeBRect.bottom());
					setlinecolor(RGB(10, 10, 10));
					line(closeBRect.x, closeBRect.y, closeBRect.right(), closeBRect.bottom());
					line(closeBRect.x, closeBRect.bottom(), closeBRect.right(), closeBRect.y);
				}
			}
			std::cout << myUi1.Uistatute << std::endl;
		}
		};

	double mapWH = 157;
	double mapRealDistance = 275;
	mapWH = configData["mapData"]["mapWH_px"].asDouble();
	mapRealDistance = configData["mapData"]["mapDistance_m"].asDouble();

	double K_M_MR = mapRealDistance / mapWH;

	double circleR = 5;
	circleR = configData["circle_R"].asDouble();
	LsMath::Vector2d saveP1(0, 0);
	LsMath::Vector2d saveP2(0, 0);
	int saveCount = 0;
	double V_length = 0;

	double RealDistance_Data = 0;

	int readTimeS = 0;
	int signTime = 0;
	std::string targetStr = u8"读";
	std::string readSign = " ";

	LsDrawEX::FPScontorl MainFPS;
	MainFPS.init_FPS(120);
	while (!closeWindow) {
		LsDrawEX::GetMouse(main_hwnd, mouseV2);
		isWindowActive = LsDrawEX::isWindowActive(main_hwnd);
		MainFPS.time_Begin();

		if(clock() - readTimeS >= 5000)
		{
			readTimeS = clock();
			if (!readJsonFile(BasePath + "\\config.json", configData)) {
				MessageBoxW(NULL, _T("错误码:114\nBZD_CONFIGERRO_READFAILURE\n配置文件读取失败"), _T("错误"), MB_OK | MB_ICONERROR);
				return 114;
			}
			circleR = configData["circle_R"].asDouble();
			mapWH = configData["mapData"]["mapWH_px"].asDouble();
			mapRealDistance = configData["mapData"]["mapDistance_m"].asDouble();
			K_M_MR = mapRealDistance / mapWH;
			setfillcolor(RGB(timeTextColor.r, timeTextColor.g, timeTextColor.b));
			setlinecolor(RGB(timeTextColor.r, timeTextColor.g, timeTextColor.b));
			fillcircle(saveP1.x, saveP1.y, circleR);

			signTime = 100;
		}
		if (signTime > 0) {
			readSign = targetStr;
			signTime--;
		}
		else
		{
			if (targetStr == u8"存") {
				targetStr = u8"读";
			}
			readSign = " ";
		}

		if (GetAsyncKeyState(0x4D) & 0x8000) 
		{
			setfillcolor(RGB(timeTextColor.r, timeTextColor.g, timeTextColor.b));
			setlinecolor(RGB(timeTextColor.r, timeTextColor.g, timeTextColor.b));
			fillcircle(saveP1.x, saveP1.y, circleR);
			fillcircle(saveP2.x, saveP2.y, circleR);
			line(saveP1.x, saveP1.y, saveP2.x, saveP2.y);
			
			static LsInput::FHInput mouseKeyRinp;
			if (mouseKeyRinp.GetKeyPress(GetAsyncKeyState(VK_RBUTTON) & 0x8000)) {
				switch (saveCount)
				{
				case 0:
					saveP1.x = mouseV2.x;
					saveP1.y = mouseV2.y;
					saveCount++;
					break;

				case 1:
					saveP2.x = mouseV2.x;
					saveP2.y = mouseV2.y;
					saveCount++;
					break;

				default:
					break;
				}
			}
			if (saveCount >= 2) {
				saveCount = 0;
			}
		}
		V_length = (saveP1 - saveP2).length();
		if ((GetAsyncKeyState(0x4D) & 0x8000) && (GetAsyncKeyState(VK_LCONTROL) & 0x8000))
		{
			readSign = u8"校";
		}
		static LsInput::FHInput mouseKeyRinp_1;
		if (mouseKeyRinp_1.GetKeyRelese((GetAsyncKeyState(0x4D) & 0x8000) && (GetAsyncKeyState(VK_LCONTROL) & 0x8000)))
		{
			mapWH = V_length;
			K_M_MR = mapRealDistance / mapWH;
			configData["mapData"]["mapWH_px"] = Json::Value(mapWH);
			saveJsonFile(BasePath + "\\config.json", configData);
			
			signTime = 100;
			targetStr = u8"存";
			readTimeS = clock();
		}
		RealDistance_Data = V_length * K_M_MR;

		int BBc = configData["TextSize"].asInt();
		settextstyle(5 * BBc, 2 * BBc, _T("微软雅黑"), 0, 0, 100, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0);
		settextcolor(RGB(timeTextColor.r, timeTextColor.g, timeTextColor.b));
		TCHAR text[100];
		//数据
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &readSign[0], (int)readSign.size(), NULL, 0);
		std::wstring readSignW(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &readSign[0], (int)readSign.size(), &readSignW[0], size_needed);
		_stprintf_s(text, 100, _T("距离:%0.1f %s"), RealDistance_Data, readSignW.c_str());
		RECT ctr1 = { timeTextPosition.x - 15 * BBc,timeTextPosition.y - 10 * BBc,timeTextPosition.x + 15 * BBc,timeTextPosition.y + 10 * BBc };
		drawtext(text, &ctr1, DT_VCENTER | DT_SINGLELINE);

		myUi1.A_UI_UpData(LsMath::Vector2d(mouseV2.x, mouseV2.y));

		LsDrawEX::drawFlush();
		MainFPS.time_Sleep();
	}
	return 0;
}