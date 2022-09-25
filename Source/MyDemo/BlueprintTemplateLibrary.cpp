// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintTemplateLibrary.h"
#include "Engine/Engine.h"

void UBlueprintTemplateLibrary::Generic_ShowStructFields(const void* StructAddr, const FStructProperty* StructProperty)
{
	const UScriptStruct* Struct = StructProperty->Struct;
	for (TFieldIterator<FProperty> iter(Struct); iter; ++iter)
	{
		FString NewMessageFString = FString::Printf(TEXT("Property:[%s].[%s]"),
			*(Struct->GetDisplayNameText().ToString()),
			*(iter->GetDisplayNameText().ToString()));
		
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Orange, NewMessageFString);
	}
}

float UBlueprintTemplateLibrary::Generic_GetArrayAVG(const void* ArrayAddr, const FArrayProperty* ArrayProperty, FName PropertyName)
{
	const FStructProperty* InnerProperty = CastField<FStructProperty>(ArrayProperty->Inner);
	if (InnerProperty == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Array inner property is NOT a UStruct."))
		return 0.0f;
	}

	const UScriptStruct* Struct = InnerProperty->Struct;
	const FString PropertyNameString = PropertyName.ToString();
	const FNumericProperty* NumProperty = nullptr;
	for (TFieldIterator<FNumericProperty> iter(Struct); iter; ++iter)
	{
		if (Struct->GetAuthoredNameForField(*iter) == PropertyNameString)
		{
			NumProperty = *iter;
			break;
		}
	}

	if (!NumProperty)
	{
		UE_LOG(LogTemp, Warning, TEXT("Struct property [%s] is NOT numberic."), *PropertyName.ToString());
		return 0.0f;
	}

	FScriptArrayHelper ArrayHelper(ArrayProperty, ArrayAddr);
	const int Count = ArrayHelper.Num();
	float Sum = 0.0f;

	if (Count <= 0)
		return Sum;

	if (NumProperty->IsFloatingPoint())
	{
		for (int i=0; i < Count; ++i)
		{
			void* ElementPtr = ArrayHelper.GetRawPtr(i);
			const uint8* ValuePtr = NumProperty->ContainerPtrToValuePtr<uint8>(ElementPtr);
			Sum += NumProperty->GetFloatingPointPropertyValue(ValuePtr);
		}
	}
	else if (NumProperty->IsInteger())
	{
		for (int i=0; i < Count; ++i)
		{
			void* ElementPtr = ArrayHelper.GetRawPtr(i);
			const uint8* ValuePtr = NumProperty->ContainerPtrToValuePtr<uint8>(ElementPtr);
			Sum += NumProperty->GetSignedIntPropertyValue(ValuePtr);
		}
	}

	return Sum / Count;
}
