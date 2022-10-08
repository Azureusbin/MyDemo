// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_SaySomething.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "GraphEditorSettings.h"

void SGraphNodeSaySomething::Construct(const FArguments& InArgs, UK2Node_SaySomething* InNode)
{
	this->GraphNode = InNode;
	this->SetCursor(EMouseCursor::CardinalCross);
	this->UpdateGraphNode();
}

void SGraphNodeSaySomething::CreateInputSideAddButton(TSharedPtr<SVerticalBox> InputBox)
{
	const FText Tmp = FText::FromString(TEXT("Add Word"));
	const TSharedRef<SWidget> AddPinButton = AddPinButtonContent(Tmp, Tmp);

	FMargin AddPinPadding = Settings->GetInputPinPadding();
	AddPinPadding.Top += 6.0f;

	InputBox->AddSlot()
	.AutoHeight()
	.VAlign(VAlign_Center)
	.Padding(AddPinPadding)
	[
		AddPinButton
	];
}

FReply SGraphNodeSaySomething::OnAddPin()
{
	return SGraphNodeK2Base::OnAddPin();
}


/**
 *  ///////////////////////////////////////
 */

void UK2Node_SaySomething::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute );
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
}

void UK2Node_SaySomething::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);
}

void UK2Node_SaySomething::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	const UClass* ActionKey = GetClass();

	if (ActionRegistrar.IsOpenForRegistration((ActionKey)))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}
