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
	/* 配置默认状态蓝图节点的引脚 */
	virtual void AllocateDefaultPins() override;
	/* 重新配置引脚 */
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	/* 展开蓝图节点 */
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	/* 注册到蓝图编辑器右键菜单 */
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	/* 对蓝图节点右键，呼出菜单 */
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	
	/* 蓝图节点在右键菜单中的名称 */
	virtual FText GetTooltipText() const override { return FText::FromString(TEXT("Say Something Node")); }
	/* 蓝图节点在右键菜单中的分类 */
	virtual FText GetMenuCategory() const override { return FText::FromString(TEXT("MyBlueprintNodes")); }
	/* 蓝图节点的标题栏 */
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString(TEXT("A Say Something Node")); }
	/* 蓝图节点的标题栏颜色，默认是浅蓝色 */
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(.5f,.6f,.2f); }
	/* 给蓝图创建Slate Widget */
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override { return SNew(SGraphNodeSaySomething, this); }


	void AddPinToNode();
	void RemoveInputPin(UEdGraphPin* Pin);

private:
	UPROPERTY()
	TArray<FName> ArgPinNames;
};
