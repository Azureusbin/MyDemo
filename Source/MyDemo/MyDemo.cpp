// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyDemo.h"

#include "ISettingsModule.h"
#include "MyCustomSetting.h"
#include "MyCustomSetting2.h"
#include "SAdvancedRotationInputBox.h"
#include "Modules/ModuleManager.h"

void FMyDemoGameModule::StartupModule()
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->RegisterSettings("Editor", "MyDemo", "My Custom Setting",
			FText::FromString(TEXT("我的自定义设置")),
			FText::FromString(TEXT("这是自定义设置的描述")),
			GetMutableDefault<UMyCustomSetting>());
	}

	// 访问使用GetDefault
	const UMyCustomSetting* MyCustomSetting = GetDefault<UMyCustomSetting>();
	if (MyCustomSetting)
	{
		FString TrueOrFalse = MyCustomSetting->UseCustomSetting ? L"True" : L"False";
		UE_LOG(LogTemp, Display, TEXT("UseCustomSetting = %s"), *TrueOrFalse);
		
		if (MyCustomSetting->UseCustomSetting)
		{
			UE_LOG(LogTemp, Display, TEXT("Health=%d, Ammo=%d"), MyCustomSetting->Health,MyCustomSetting->Ammo);
		}
	}
	// 修改使用GetMutableDefault
	UMyCustomSetting2* MyCustomSetting2 = GetMutableDefault<UMyCustomSetting2>();
	if (MyCustomSetting2)
	{
		MyCustomSetting2->UseCustomSetting = true;
		MyCustomSetting2->Health = 150;
		MyCustomSetting2->Ammo = 300;
	}
	SNew(SAdvancedRotationInputBox<float>, true);
}

void FMyDemoGameModule::ShutdownModule()
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings("Editor", "MyDemo", "My Custom Setting");
	}
}

bool FMyDemoGameModule::IsGameModule() const
{
	return true;
}

IMPLEMENT_PRIMARY_GAME_MODULE(FMyDemoGameModule, MyDemo, "MyDemo");
