#pragma once


#include "../../Base/WinApp/WinApp.h"
#include "../../Base/DXCommon/DirectXCommon.h"
#include "../../CommandManager/CommandManager.h"
#include "../../View/RTVManager/RTVManager.h"
#include "JapaneseGlyphArray.h"

class ImGuiManager {
public:
	static ImGuiManager* GetInstance();
	static void Initialize();
	static void BeginFrame();
	static void EndFrame();
};