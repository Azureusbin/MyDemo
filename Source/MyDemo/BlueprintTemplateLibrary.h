// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BlueprintTemplateLibrary.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FDummyStruct
{
	GENERATED_BODY()
	
};

/**
 * 
 */
UCLASS()
class MYDEMO_API UBlueprintTemplateLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/* Example 1 */
	UFUNCTION(BlueprintCallable,CustomThunk,Category=MyDemo,meta=(CustomStructureParam="CustomStruct"))
	static void ShowStructFields(const FDummyStruct& CustomStruct) {/* Do Nothing */}
	static void Generic_ShowStructFields(const void* StructAddr, const FStructProperty* StructProperty);
	
	DECLARE_FUNCTION(execShowStructFields)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.MostRecentPropertyAddress = nullptr;

		/*
		 * 通过调用Stack.StepCompiledIn()函数，就可以更新蓝图虚拟机的栈顶指针；
		 * Stack.MostRecentPropertyAddress和Stack.MostRecentProperty这两个变量，就是当前参数值的内存地址和反射信息。
		 */
		Stack.StepCompiledIn<FStructProperty>(nullptr);
		const void* StructAddr = Stack.MostRecentPropertyAddress;
		const FStructProperty* StructProperty = CastField<FStructProperty>(Stack.MostRecentProperty);

		P_FINISH;

		P_NATIVE_BEGIN;
		Generic_ShowStructFields(StructAddr, StructProperty);
		P_NATIVE_END;
	}

	/* Example 2 */
	UFUNCTION(BlueprintCallable, CustomThunk, Category=MyDemo, meta=(ArrayParm="TagetArray", ArrayTypeDependentParams="TagetArray"))
	static float GetArrayAVG(const TArray<int32>& TagetArray, FName PropertyName) {/* Do Nothing */ return 0;}
	static float Generic_GetArrayAVG(const void* ArrayAddr, const FArrayProperty* ArrayProperty, FName PropertyName);

	DECLARE_FUNCTION(execGetArrayAVG)
	{
		/*Stack.StepCompiledIn<FArrayProperty>(nullptr);
		const void* ArrayAddr = Stack.MostRecentPropertyAddress;
		const FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);*/
		
		// Get Target Array
		Stack.StepCompiledIn<FArrayProperty>(nullptr);
		
		const void* ArrayAddr = Stack.MostRecentPropertyAddress;
		const FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
		
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		// Get Property Name
		P_GET_PROPERTY(FNameProperty, PropertyName);
		
		P_FINISH;

		P_NATIVE_BEGIN;
		*static_cast<float*>(RESULT_PARAM) = Generic_GetArrayAVG(ArrayAddr,ArrayProperty, PropertyName);
		P_NATIVE_END;
	}

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly=true))
	static void Hello_Internal(const UObject* WorldContextObject, const FString& Message)
	{
		UE_LOG(LogTemp, Display, TEXT("Hello world from K2 Node."));
	}

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly=true, WorldContext = "WorldContextObject"))
	static void PrintMessage_Internal(const UObject* WorldContextObject, const FString& Message)
	{
		UKismetSystemLibrary::PrintString(WorldContextObject, Message);
	}
};
