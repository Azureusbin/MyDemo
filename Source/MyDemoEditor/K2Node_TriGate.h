// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KismetCompilerMisc.h"
#include "K2Node_TriGate.generated.h"

namespace TriGatePN
{
	inline FName Exec(TEXT("执行"));
	inline FName Input(TEXT("输入值"));
	inline FName Positive(TEXT("正数"));
	inline FName Zero(TEXT("零"));
	inline FName Negative(TEXT("负数"));
}

class FKCHandler_TriGate final : public FNodeHandlingFunctor
{
public:
	explicit FKCHandler_TriGate(FKismetCompilerContext& CompilerContext);
	
	virtual void Compile(FKismetFunctionContext& Context, UEdGraphNode* Node) override;
	virtual void RegisterNet(FKismetFunctionContext& Context, UEdGraphPin* Pin) override;
	virtual void RegisterNets(FKismetFunctionContext& Context, UEdGraphNode* Node) override;

protected:
	// 为每个Node开辟一个临时bool变量，用来存储输入参数的比较结果
	TMap<UEdGraphNode*, FBPTerminal*> BoolTermMap;
	FBPTerminal* LiteralZeroTerm;
};

/**
 * 
 */
UCLASS()
class MYDEMOEDITOR_API UK2Node_TriGate : public UK2Node
{
public:
	GENERATED_BODY()
	
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FNodeHandlingFunctor* CreateNodeHandler(FKismetCompilerContext& CompilerContext) const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
};
