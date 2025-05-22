// Copyright Epic Games, Inc. All Rights Reserved.

#include "MellosUMGBPLibrary.h"
#include "MellosUMG.h"
#include "Blueprint/UserWidget.h"

UMellosUMGBPLibrary::UMellosUMGBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

AGameMode* UMellosUMGBPLibrary::GetGameModeFromObject(const UObject* WorldContextObject)
{
	return WorldContextObject ? WorldContextObject->GetWorld() ? WorldContextObject->GetWorld()->GetAuthGameMode<AGameMode>() : nullptr : nullptr;
}

UUserWidget* UMellosUMGBPLibrary::CreateWidgetFromObject(const UObject* WorldContextObject,
	TSubclassOf<UUserWidget> WidgetClass)
{
	return CreateWidget<UUserWidget>(WorldContextObject->GetWorld(), WidgetClass);
}
