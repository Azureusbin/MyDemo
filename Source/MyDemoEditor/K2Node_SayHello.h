// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_SayHello.generated.h"

/**
 * 
 */
UCLASS()
class MYDEMOEDITOR_API UK2Node_SayHello : public UK2Node
{
public:
	// UEdGraphNode interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString(TEXT("A Hello World Node")); }
	virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Say Hello World")); }

	// UK2Node interface
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual FText GetMenuCategory() const override { return FText::FromString(TEXT("MyBlueprintNodes")); }
	
	GENERATED_BODY()
};
