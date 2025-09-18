#pragma once

#include "CoreMinimal.h"
#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"

/** it's hard to fetch all components from a BP, including c++ base class components! use this. */
template<typename BaseT>
void ForAllComponentsOfType(const UBlueprint& BP, TFunctionRef<void (const FString& CompName, const BaseT& Comp)> Handler)
{
	if (const auto BPClass = Cast<UBlueprintGeneratedClass>(BP.GeneratedClass))
	{
		const auto CtorScript = BPClass->SimpleConstructionScript;
		const TArray<USCS_Node*>& CtorNodes = CtorScript->GetAllNodes();
		for(const auto CtorNode : CtorNodes)
		{
			const auto CtorNodeClass = CtorNode->ComponentClass;
			if (CtorNodeClass->IsChildOf(USceneComponent::StaticClass()))
			{
				const auto CtorNodeComponentName = CtorNode->GetVariableName();
				const auto CtorSceneComp = Cast<const USceneComponent>(CtorNode->GetActualComponentTemplate(BPClass));
				Handler(CtorNodeComponentName.ToString(), *CtorSceneComp);
			}
		}
	}

	static TArray<UObject*> SubObjects;
	SubObjects.Empty();
	UObject* const DefaultObject = BP.GeneratedClass->GetDefaultObject();;
	DefaultObject->GetDefaultSubobjects(SubObjects);
	for ( const auto SubObject : SubObjects )
	{
		if ( const auto SceneComponent = Cast<const USceneComponent>(SubObject) )
		{
			Handler(SceneComponent->GetName(), *SceneComponent);
		}
	}
}