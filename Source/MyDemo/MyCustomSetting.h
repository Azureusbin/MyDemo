// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MyCustomSetting.generated.h"

/**
 * 
 */
UCLASS(Config=MyDemo, DefaultConfig)
class MYDEMO_API UMyCustomSetting : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = Setting1)
	bool UseCustomSetting;

	UPROPERTY(Config, EditAnywhere, Category = Setting2, meta=(UIMin=1,EditConditionHides=true,EditCondition="UseCustomSetting==true"))
	float Health = 100;

	UPROPERTY(Config, EditAnywhere, Category = Setting2, meta=(UIMin=30,UIMax=300,EditConditionHides=true,EditCondition="UseCustomSetting==true"))
	float Ammo = 240;
};
