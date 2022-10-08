// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_SayHello.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "MyDemo/BlueprintTemplateLibrary.h"

void UK2Node_SayHello::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute );
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
}

void UK2Node_SayHello::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	const UClass* ActionKey = GetClass();

	if (ActionRegistrar.IsOpenForRegistration((ActionKey)))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

void UK2Node_SayHello::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	UEdGraphPin* ExecPin = GetExecPin();
	UEdGraphPin* ThenPin = FindPin(UEdGraphSchema_K2::PN_Then, EGPD_Output);

	if (ExecPin && ThenPin)
	{
		const FName MyFunctionName = GET_FUNCTION_NAME_CHECKED(UBlueprintTemplateLibrary, Hello_Internal);

		UK2Node_CallFunction* CallFunctionNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		CallFunctionNode->FunctionReference.SetExternalMember(MyFunctionName, UBlueprintTemplateLibrary::StaticClass());
		CallFunctionNode->AllocateDefaultPins();

		// Move pins
		CompilerContext.MovePinLinksToIntermediate(*ExecPin, *(CallFunctionNode->GetExecPin()));
		CompilerContext.MovePinLinksToIntermediate(*ThenPin, *(CallFunctionNode->GetThenPin()));
	}

	// Break any links to the expanded node
	BreakAllNodeLinks();
}
