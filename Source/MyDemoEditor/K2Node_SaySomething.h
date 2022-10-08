// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KismetNodes/SGraphNodeK2Base.h"
#include "K2Node_SaySomething.generated.h"


class UK2Node_SaySomething;

class MYDEMOEDITOR_API SGraphNodeSaySomething : public SGraphNodeK2Base
{
public:
	SLATE_BEGIN_ARGS(SGraphNodeSaySomething){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UK2Node_SaySomething* InNode);
protected:
	virtual void CreateInputSideAddButton(TSharedPtr<SVerticalBox> InputBox) override;
	virtual FReply OnAddPin() override;
};

/**
 * 
 */
UCLASS()
class MYDEMOEDITOR_API UK2Node_SaySomething : public UK2Node
{
	GENERATED_BODY()
public:
	virtual void AllocateDefaultPins() override;
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;

	virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Say Something Node")); }
	virtual FText GetMenuCategory() const override { return FText::FromString(TEXT("MyBlueprintNodes")); }
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString(TEXT("A Say Something Node")); }
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Red; }
	
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override { return SNew(SGraphNodeSaySomething, this); }
};
