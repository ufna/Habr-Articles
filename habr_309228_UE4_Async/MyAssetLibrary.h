// Copyright 2016 ufna. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyAssetLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMyAssetLibrary, Display, All);

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FMyAsyncSpawnActorDelegate, bool, bResult, FStringAssetReference, LoadedReference, AActor*, SpawnedActor);

/**
 * Library that provides sync/asycn load functions for different types of assets
 */
UCLASS()
class UMyAssetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Asynchronously load asset and spawn actor then */
	UFUNCTION(BlueprintCallable, Category = "MyProject|MyAssetLibrary", meta = (WorldContext = "WorldContextObject", DisplayName = "Spawn Actor (Async)"))
	static void AsyncSpawnActor(UObject* WorldContextObject, TAssetSubclassOf<AActor> AssetPtr, FTransform SpawnTransform, const FMyAsyncSpawnActorDelegate& Callback);

	/** Synchronously load material instanse from asset */
	UFUNCTION(BlueprintCallable, Category="MyProject|MyAssetLibrary", meta=(WorldContext = "WorldContextObject", DisplayName="Load Material (Sync)"))
	static UMaterialInstanceConstant* SyncLoadMaterial(UObject* WorldContextObject, TAssetPtr<UMaterialInstanceConstant> Asset);

	/** Synchronously create widget from asset */
	UFUNCTION(BlueprintCallable, Category="MyProject|MyAssetLibrary", meta=(WorldContext="WorldContextObject", DisplayName="Create Widget (Sync)"))
	static UUserWidget* SyncCreateWidget(UObject* WorldContextObject, TAssetSubclassOf<UUserWidget> Asset, APlayerController* OwningPlayer);

protected:
	/** Called when asset loading for actor spawn is finished */
	static void OnAsyncSpawnActorComplete(UObject* WorldContextObject, FStringAssetReference Reference, FTransform SpawnTransform, FMyAsyncSpawnActorDelegate Callback);

};
