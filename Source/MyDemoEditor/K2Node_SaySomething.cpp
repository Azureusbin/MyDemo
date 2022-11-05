// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_SaySomething.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "GraphEditorSettings.h"
#include "K2Node_CallFunction.h"
#include "K2Node_MakeArray.h"
#include "KismetCompiler.h"
#include "ToolMenu.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "MyDemo/BlueprintTemplateLibrary.h"

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
	CastChecked<UK2Node_SaySomething>(GraphNode)->AddPinToNode();
	this->UpdateGraphNode();
	
	return FReply::Handled();
}


/**
 *  ///////////////////////////////////////
 */

void UK2Node_SaySomething::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute );
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
}

void UK2Node_SaySomething::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
{
	Super::ReallocatePinsDuringReconstruction(OldPins);

	for (UEdGraphPin* Pin : OldPins)
	{
		// 过滤不需要的端口
		if (UEdGraphSchema_K2::IsExecPin(*Pin) || Pin->Direction == EGPD_Output)
			continue;
		
		UE_LOG(LogTemp, Display, TEXT("Allocate pin: [%s]"), *Pin->GetName());
		UEdGraphPin* TempPin =  CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, Pin->PinName);
		TempPin->DefaultValue = Pin->DefaultValue;
	}
}

void UK2Node_SaySomething::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	UEdGraphPin* ExecPin = GetExecPin();
	UEdGraphPin* ThenPin = FindPin(UEdGraphSchema_K2::PN_Then ,EGPD_Output);

	if (ExecPin && ThenPin)
	{
		// Create a CallFunction Node
		const FName MyFunctionName = GET_FUNCTION_NAME_CHECKED(UBlueprintTemplateLibrary, PrintMessage_Internal);

		UK2Node_CallFunction* CallFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		CallFuncNode->FunctionReference.SetExternalMember(MyFunctionName, UBlueprintTemplateLibrary::StaticClass());
		CallFuncNode->AllocateDefaultPins();

		// Move exec pin
		CompilerContext.MovePinLinksToIntermediate(*ExecPin, *(CallFuncNode->GetExecPin()));
		CompilerContext.MovePinLinksToIntermediate(*ThenPin, *(CallFuncNode->GetThenPin()));

		// Connect a "Make Array" node to compile all args
		UK2Node_MakeArray* MakeArrayNode = CompilerContext.SpawnIntermediateNode<UK2Node_MakeArray>(this, SourceGraph);
		MakeArrayNode->AllocateDefaultPins();

		// Connect Make Array output to function arg
		UEdGraphPin* ArrayOut = MakeArrayNode->GetOutputPin();
		UEdGraphPin* FuncArgPin = CallFuncNode->FindPinChecked(TEXT("Messages"));
		ArrayOut->MakeLinkTo(FuncArgPin);

		// This will set the "Make Array" node's type, only works if one pin is connected.
		MakeArrayNode->PinConnectionListChanged(ArrayOut);

		// Connect all arg pin to Make Array input
		for (int32 i=0; i<ArgPinNames.Num(); ++i)
		{
			// Make Array node has one input by default
			if (i>0)
				MakeArrayNode->AddInputPin();

			// Fine the input pin on the "Make Array" node by index
			const FString PinName = FString::Printf(TEXT("[%d]"), i);
			UEdGraphPin* ArrayInputPin = MakeArrayNode->FindPinChecked(PinName);

			// Move input word to array
			UEdGraphPin* MyInputPin = FindPinChecked(ArgPinNames[i], EGPD_Input);
			CompilerContext.MovePinLinksToIntermediate(*MyInputPin, *ArrayInputPin);
		}
	}

	// Break any links to the expanded node
	BreakAllNodeLinks();
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

void UK2Node_SaySomething::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	if (!Context->bIsDebugging)
	{
		{
			FToolMenuSection& Section = Menu->AddSection("UK2Node_SaySomething", FText::FromString(TEXT("Say Some Thing")));
			if (Context->Pin != NULL)
			{
				if ( Context->Pin->Direction == EGPD_Input )
				{
					Section.AddMenuEntry(
							TEXT("RemovePin"),
							FText::FromString(TEXT("Remove word")),
							FText::FromString(TEXT("Remove word from input")),
							FSlateIcon(),
							FUIAction(
								FExecuteAction::CreateUObject(const_cast<UK2Node_SaySomething*>(this), &UK2Node_SaySomething::RemoveInputPin, const_cast<UEdGraphPin*>(Context->Pin))
							)
						);
				}
			}
			else
			{
				Section.AddMenuEntry(
							TEXT("AddPin"),
							FText::FromString(TEXT("Add word")),
							FText::FromString(TEXT("Add word to input")),
							FSlateIcon(),
							FUIAction(
								FExecuteAction::CreateUObject(const_cast<UK2Node_SaySomething*>(this), &UK2Node_SaySomething::AddPinToNode)
							)
						);
			}
		}
	}
}

void UK2Node_SaySomething::AddPinToNode()
{
	const FScopedTransaction Transaction(NSLOCTEXT("Kismet", "AddArgumentPin", "Add Argument Pin"));
	
	const TMap<FString,FStringFormatArg> FormatArgs = {
		{TEXT("Count"), ArgPinNames.Num()}
	};
	const FName NewPinName(FString::Format(TEXT("Word {Count}"), FormatArgs));
	ArgPinNames.Add(NewPinName);

	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, NewPinName);
	Modify(true);
	
	FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
	GetGraph()->NotifyGraphChanged();
}

void UK2Node_SaySomething::RemoveInputPin(UEdGraphPin* Pin)
{
	FScopedTransaction Transaction(FText::FromString(TEXT("Remove Input Pin")));

	ArgPinNames.Remove(Pin->GetFName());

	RemovePin(Pin);
	Modify(true);
	
	FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
	GetGraph()->NotifyGraphChanged();
}
