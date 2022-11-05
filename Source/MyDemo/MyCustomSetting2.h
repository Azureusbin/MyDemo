// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MyCustomSetting2.generated.h"


UCLASS()
class MYDEMO_API UMyCustomSetting2 : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	// 等效方法1的ContainerName参数
	virtual FName GetContainerName() const override
	{
		return "Project";
	}
	// 等效方法1的CategoryName参数
	virtual FName GetCategoryName() const override
	{
		return "MyDemo";
	}
	// 等效方法1的SectionName参数
	virtual FName GetSectionName() const override
	{
		return "My Custom Setting";
	}
	// 等效方法1的DisplayName参数
	virtual FText GetSectionText() const override
	{
		return FText::FromString(TEXT("我的自定义设置"));
	}
	// 等效方法1的Description参数
	virtual FText GetSectionDescription() const override
	{
		return FText::FromString(TEXT("这是自定义设置的描述"));
	}
	
	UPROPERTY(EditAnywhere, Category = Setting1)
	bool UseCustomSetting;

	UPROPERTY(EditAnywhere, Category = Setting2, meta=(UIMin=1,EditConditionHides=true,EditCondition="UseCustomSetting==true"))
	float Health = 100;

	UPROPERTY(EditAnywhere, Category = Setting2, meta=(UIMin=30,UIMax=300,EditConditionHides=true,EditCondition="UseCustomSetting==true"))
	float Ammo = 240;
};
