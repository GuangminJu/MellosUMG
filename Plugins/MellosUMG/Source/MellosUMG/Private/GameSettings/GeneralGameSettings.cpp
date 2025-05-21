// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings/GeneralGameSettings.h"

#include "Kismet/KismetSystemLibrary.h"

void UGeneralGameSettings::SetBrightness(float InBrightness)
{
	Brightness = InBrightness;

	UKismetSystemLibrary::ExecuteConsoleCommand(
		this, FString::Printf(TEXT("r.ExposureOffset %.2f"), Brightness));
}
