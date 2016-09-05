// Copyright 2016 ufna. All Rights Reserved.

#pragma once

#include "MyAssetTypes.generated.h"

//////////////////////////////////////////////////////////////////////////
// DataTable rows

/**
 * Example #1. Table for dynamic actor creation (not defined in advance)
 */
USTRUCT(Blueprintable)
struct FMyActorTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AssetId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<AActor> ActorClass;

	FMyActorTableRow() :
		AssetId(TEXT("")),
		ActorClass(nullptr)
	{
	}
};

/**
 * Example #2. When you just need to load predefined widget (but prevent all references to be loaded on parent class creation)
 */
USTRUCT(Blueprintable)
struct FMyWidgetTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<UUserWidget> WidgetClass;
	
	FMyWidgetTableRow() :
		WidgetClass(nullptr)
	{
	}
};

/**
 * Example #3. Create your own tables just with blueprints
 */
USTRUCT(Blueprintable)
struct FMyMaterialInstanceAsset
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetPtr<UMaterialInstanceConstant> MaterialInstance;

	FMyMaterialInstanceAsset() :
		MaterialInstance(nullptr)
	{
	}
};
