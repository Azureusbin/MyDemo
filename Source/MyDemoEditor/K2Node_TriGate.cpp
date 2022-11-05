// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_TriGate.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EdGraphUtilities.h"
#include "KismetCompiledFunctionContext.h"
#include "Kismet/KismetMathLibrary.h"

FKCHandler_TriGate::FKCHandler_TriGate(FKismetCompilerContext& CompilerContext)
	: FNodeHandlingFunctor(CompilerContext)
{
}

void FKCHandler_TriGate::Compile(FKismetFunctionContext& Context, UEdGraphNode* Node)
{
	const UK2Node_TriGate* MyNode = CastChecked<UK2Node_TriGate>(Node);
	UEdGraphPin* InputPin = Context.FindRequiredPinByName(MyNode, TriGatePN::Input, EGPD_Input);
	UEdGraphPin* PinToTry = FEdGraphUtilities::GetNetFromPin(InputPin);
	FBPTerminal** pInputTerm = Context.NetMap.Find(PinToTry);
	if (pInputTerm == nullptr)
	{
		Context.MessageLog.Error(TEXT("FKCHandler_TriGate: 无法找到输入端口"), InputPin);
		return;
	}
	FBPTerminal* InputTerm = *pInputTerm;

	UEdGraphPin* PositivePin = MyNode->FindPin(TriGatePN::Positive, EGPD_Output);
	UEdGraphPin* ZeroPin = MyNode->FindPin(TriGatePN::Zero, EGPD_Output);
	UEdGraphPin* NegativePin = MyNode->FindPin(TriGatePN::Negative, EGPD_Output);

	FBPTerminal* BoolTerm = BoolTermMap.FindRef(MyNode);
	
	const UClass* MathLibraryClass = UKismetMathLibrary::StaticClass();
	UFunction* GreaterFuncPtr = FindUField<UFunction>(MathLibraryClass, "Greater_IntInt");
	UFunction* EqualFuncPtr = FindUField<UFunction>(MathLibraryClass, "EqualEqual_IntInt");

	// Statement 1: 计算表达式 BoolTerm = Integer > 0
	FBlueprintCompiledStatement& CallGreaterZero = Context.AppendStatementForNode(Node);
	CallGreaterZero.Type = KCST_CallFunction;
	CallGreaterZero.FunctionToCall = GreaterFuncPtr;
	CallGreaterZero.LHS = BoolTerm;
	CallGreaterZero.RHS.Add(InputTerm);
	CallGreaterZero.RHS.Add(LiteralZeroTerm);

	// Statement 2: if (BoolTerm)
	FBlueprintCompiledStatement& IfPositive = Context.AppendStatementForNode(Node);
	IfPositive.Type = KCST_GotoIfNot;
	IfPositive.LHS = BoolTerm;

	// Statement 3: 执行 Positive Pin
	FBlueprintCompiledStatement& ExecPositive = Context.AppendStatementForNode(Node);
	ExecPositive.Type = KCST_UnconditionalGoto;
	Context.GotoFixupRequestMap.Add(&ExecPositive, PositivePin);

	// Statement 4: 计算表达式 BoolTerm = Integer == 0
	FBlueprintCompiledStatement& CallEqualZero = Context.AppendStatementForNode(Node);
	CallEqualZero.Type = KCST_CallFunction;
	CallEqualZero.FunctionToCall = EqualFuncPtr;
	CallEqualZero.LHS = BoolTerm;
	CallEqualZero.RHS.Add(InputTerm);
	CallEqualZero.RHS.Add(LiteralZeroTerm);
	CallEqualZero.bIsJumpTarget = true;  // 设为跳转的目标
	
	IfPositive.TargetLabel = &CallEqualZero;

	// Statement 5: GotoIfNot(BoolTerm)
	FBlueprintCompiledStatement& IfZero = Context.AppendStatementForNode(Node);
	IfZero.Type = KCST_GotoIfNot;
	IfZero.LHS = BoolTerm;
	Context.GotoFixupRequestMap.Add(&IfZero, NegativePin);

	// Statement 6: 执行 Zero Pin
	FBlueprintCompiledStatement& ExecZero = Context.AppendStatementForNode(Node);
	ExecZero.Type = KCST_UnconditionalGoto;
	Context.GotoFixupRequestMap.Add(&ExecZero, ZeroPin);
}

void FKCHandler_TriGate::RegisterNet(FKismetFunctionContext& Context, UEdGraphPin* Pin)
{
	FNodeHandlingFunctor::RegisterNet(Context, Pin);
}

void FKCHandler_TriGate::RegisterNets(FKismetFunctionContext& Context, UEdGraphNode* Node)
{
	FNodeHandlingFunctor::RegisterNets(Context, Node);

	// 存储比较结果的Bool
	FBPTerminal* BoolTerm = Context.CreateLocalTerminal();
	BoolTerm->Type.PinCategory = UEdGraphSchema_K2::PC_Boolean;
	BoolTerm->Source = Node;
	BoolTerm->Name = Context.NetNameMap->MakeValidName(Node) + TEXT("_CMPRESULT");
	BoolTermMap.Add(Node, BoolTerm);

	// 字面量 “0”
	LiteralZeroTerm = Context.CreateLocalTerminal(TS_Literal);
	LiteralZeroTerm->Type.PinCategory = UEdGraphSchema_K2::PC_Int;
	LiteralZeroTerm->Name = TEXT("2");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UK2Node_TriGate::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, TriGatePN::Exec);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int, TriGatePN::Input);
	
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, TriGatePN::Positive);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, TriGatePN::Zero);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, TriGatePN::Negative);
}

FText UK2Node_TriGate::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("TriGate"));
}

FText UK2Node_TriGate::GetTooltipText() const
{
	return FText::FromString(TEXT("Tri Gate"));
}

FLinearColor UK2Node_TriGate::GetNodeTitleColor() const
{
	return FLinearColor(0.2,0.9,0.18,1);
}

/* 代替ExpandNode作为节点编译的依据 */
FNodeHandlingFunctor* UK2Node_TriGate::CreateNodeHandler(FKismetCompilerContext& CompilerContext) const
{
	return new FKCHandler_TriGate(CompilerContext);
}

void UK2Node_TriGate::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(ActionKey);
		check(Spawner!=nullptr);
		ActionRegistrar.AddBlueprintAction(Spawner);
	}
}

FText UK2Node_TriGate::GetMenuCategory() const
{
	return FText::FromString(TEXT("MyBlueprintNodes"));
}
