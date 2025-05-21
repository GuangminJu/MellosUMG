// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings/GeneralGameSettings.h"

#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

void FStringOption::SetSelectedOption(const FString& InOption)
{
	for (int32 i = 0; i < Options.Num(); ++i)
	{
		if (Options[i] == InOption)
		{
			SelectedIdx = i;
			OnSelectedOptionChanged.Broadcast(SelectedIdx);
			break;
		}
	}
}

UGeneralGameSettings::UGeneralGameSettings()
{
	FScreenResolutionArray Resolutions;
	if (RHIGetAvailableResolutions(Resolutions, true))
	{
		for (const FScreenResolutionRHI& Res : Resolutions)
		{
			FString ResStr = FString::Printf(TEXT("%dx%d"), Res.Width, Res.Height);
			Resolution.Options.Add(ResStr);
			ScreenResolutionRHI.Add(Res);
		}
	}
	
	Resolution.OnSelectedOptionChanged.AddDynamic(this, &UGeneralGameSettings::SetResolution);
}

void UGeneralGameSettings::Tick(float DeltaTime)
{
	if (GEngine && GEngine->GetGameUserSettings())
	{
		FIntPoint ResolutionPoint = GEngine->GetGameUserSettings()->GetScreenResolution();
		int32 Width = ResolutionPoint.X;
		int32 Height = ResolutionPoint.Y;

		int32 Index = GetIndex(Width, Height);
		if (Index != -1)
		{
			// Resolution.SelectedIdx = Index;
		}
	}
}

TStatId UGeneralGameSettings::GetStatId() const
{
	return TStatId();
}

void UGeneralGameSettings::SetBrightness(float InBrightness)
{
	Brightness = InBrightness;

	UKismetSystemLibrary::ExecuteConsoleCommand(
		this, FString::Printf(TEXT("r.ExposureOffset %.2f"), Brightness));
}

void UGeneralGameSettings::SetVolume(float InVolume)
{
	Volume = InVolume;

	UKismetSystemLibrary::ExecuteConsoleCommand(
		this,
		FString::Printf(TEXT("AudioMixer.SetMasterVolumeOverride %.2f"), InVolume));
}

void UGeneralGameSettings::SetDisplayMode(EDisplayMode InDisplayMode)
{
	DisplayMode = InDisplayMode;
	
	EWindowMode::Type WindowMode;

	switch (InDisplayMode)
	{
	case EDisplayMode::Fullscreen:
		WindowMode = EWindowMode::Fullscreen;
		break;
	case EDisplayMode::Windowed:
		WindowMode = EWindowMode::Windowed;
		break;
	case EDisplayMode::WindowedFullscreen:
		WindowMode = EWindowMode::WindowedFullscreen;
		break;
	default:
		WindowMode = EWindowMode::WindowedFullscreen;
		break;
	}

	if (GEngine && GEngine->GetGameUserSettings())
	{
		UGameUserSettings* Settings = GEngine->GetGameUserSettings();
		Settings->SetFullscreenMode(WindowMode);
		Settings->ApplySettings(false);
	}
}

int32 UGeneralGameSettings::GetIndex(int32 InWidth, int32 InHeight) const
{
	for (int32 i = 0; i < ScreenResolutionRHI.Num(); ++i)
	{
		if (ScreenResolutionRHI[i].Height == InHeight && ScreenResolutionRHI[i].Width == InWidth)
		{
			return i;
		}
	}
	return -1;
}

void UGeneralGameSettings::SetSelectedOption(FStringOption& InResolution, const FString& InOption)
{
	InResolution.SetSelectedOption(InOption);
}

void UGeneralGameSettings::SetResolution(int32 SelectedOption)
{
	const FScreenResolutionRHI& ResolutionRHI = ScreenResolutionRHI[SelectedOption];
	if (GEngine && GEngine->GetGameUserSettings())
	{
		UGameUserSettings* Settings = GEngine->GetGameUserSettings();
		Settings->RequestResolutionChange(ResolutionRHI.Width, ResolutionRHI.Height, GetWindowMode(DisplayMode), false);
	}
}

EWindowMode::Type UGeneralGameSettings::GetWindowMode(EDisplayMode InDisplayMode)
{
	switch (InDisplayMode)
	{
	case EDisplayMode::Fullscreen:
		return EWindowMode::Fullscreen;
	case EDisplayMode::Windowed:
		return EWindowMode::Windowed;
	case EDisplayMode::WindowedFullscreen:
		return EWindowMode::WindowedFullscreen;
	default:
		return EWindowMode::WindowedFullscreen;
	}
}
