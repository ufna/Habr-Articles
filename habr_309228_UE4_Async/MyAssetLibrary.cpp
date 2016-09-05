// Copyright 2016 ufna. All Rights Reserved.

#include "MyProject.h"
#include "MyAssetLibrary.h"

DEFINE_LOG_CATEGORY(LogMyAssetLibrary)

void UMyAssetLibrary::AsyncSpawnActor(UObject* WorldContextObject, TAssetSubclassOf<AActor> AssetPtr, FTransform SpawnTransform, const FMyAsyncSpawnActorDelegate& Callback)
{
	// Load asset into memory first
	FStreamableManager& AssetLoader = UMyProjectSingleton::Get().AssetLoader;
	FStringAssetReference Reference = AssetPtr.ToStringReference();
	AssetLoader.RequestAsyncLoad(Reference, FStreamableDelegate::CreateStatic(&UMyAssetLibrary::OnAsyncSpawnActorComplete, WorldContextObject, Reference, SpawnTransform, Callback));
}

void UMyAssetLibrary::OnAsyncSpawnActorComplete(UObject* WorldContextObject, FStringAssetReference Reference, FTransform SpawnTransform, FMyAsyncSpawnActorDelegate Callback)
{
	AActor* SpawnedActor = nullptr;

	// Asset now should be loaded into memory, so try to get it
	UClass* ActorClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *(Reference.ToString())));
	if (ActorClass != nullptr)
	{
		// Spawn actor from asset now
		SpawnedActor = WorldContextObject->GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTransform);
	}
	else
	{
		UE_LOG(LogMyAssetLibrary, Warning, TEXT("UMyAssetLibrary::OnAsyncSpawnActorComplete -- Failed to load object: $"), *Reference.ToString());
	}

	Callback.ExecuteIfBound(SpawnedActor != nullptr, Reference, SpawnedActor);
}

UMaterialInstanceConstant* UMyAssetLibrary::SyncLoadMaterial(UObject* WorldContextObject, TAssetPtr<UMaterialInstanceConstant> Asset)
{
	// Check we're trying to load not null asset
	if (Asset.IsNull())
	{
		FString InstigatorName = (WorldContextObject != nullptr) ? WorldContextObject->GetFullName() : TEXT("Unknown");
		UE_LOG(LogMyAssetLibrary, Warning, TEXT("UMyAssetLibrary::SyncLoadMaterial -- Asset ptr is null for: %s"), *InstigatorName);
		return nullptr;
	}

	// Load asset into memory first (sync)
	FStreamableManager& AssetLoader = UMyProjectSingleton::Get().AssetLoader;
	FStringAssetReference Reference = Asset.ToStringReference();
	AssetLoader.SynchronousLoad(Reference);

	// Now load object and check that it has desired class
	UMaterialInstanceConstant* LoadedFrame = (UMaterialInstanceConstant*)(StaticLoadObject(UMaterialInstanceConstant::StaticClass(), WorldContextObject, *(Reference.ToString())));
	if (LoadedFrame)
	{
		// Be sure that it won't be killed by GC on this frame
		LoadedFrame->SetFlags(RF_StrongRefOnFrame);
	}
	else
	{
		UE_LOG(LogMyAssetLibrary, Warning, TEXT("UMyAssetLibrary::SyncLoadMaterial -- Failed to load object: $"), *Asset.ToString());
	}

	return LoadedFrame;
}

UUserWidget* UMyAssetLibrary::SyncCreateWidget(UObject* WorldContextObject, TAssetSubclassOf<UUserWidget> Asset, APlayerController* OwningPlayer)
{
	// Check we're trying to load not null asset
	if (Asset.IsNull())
	{
		FString InstigatorName = (WorldContextObject != nullptr) ? WorldContextObject->GetFullName() : TEXT("Unknown");
		UE_LOG(LogMyAssetLibrary, Warning, TEXT("UMyAssetLibrary::SyncCreateWidget -- Asset ptr is null for: %s"), *InstigatorName);
		return nullptr;
	}

	// Load asset into memory first (sync)
	FStreamableManager& AssetLoader = UMyProjectSingleton::Get().AssetLoader;
	FStringAssetReference Reference = Asset.ToStringReference();
	AssetLoader.SynchronousLoad(Reference);

	// Now load object and check that it has desired class
	UClass* WidgetType = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *(Reference.ToString())));
	if (WidgetType == nullptr)
	{
		return nullptr;
	}
	
	// Create widget from loaded object
	UUserWidget* UserWidget = nullptr;
	if (OwningPlayer == nullptr)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
		UserWidget = CreateWidget<UUserWidget>(World, WidgetType);
	}
	else
	{
		UserWidget = CreateWidget<UUserWidget>(OwningPlayer, WidgetType);
	}
	
	// Be sure that it won't be killed by GC on this frame
	if (UserWidget)
	{
		UserWidget->SetFlags(RF_StrongRefOnFrame);
	}
	
	return UserWidget;
}
