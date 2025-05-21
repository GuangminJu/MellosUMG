// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GeneralGameSettings.generated.h"

USTRUCT(Blueprintable)
struct FStringOption
{
	GENERATED_BODY()

	FStringOption(): SelectedIdx(0)
	{
		FScreenResolutionArray Resolutions;
		if (RHIGetAvailableResolutions(Resolutions, true))
		{
			for (const FScreenResolutionRHI& Res : Resolutions)
			{
				FString ResStr = FString::Printf(TEXT("%dx%d"), Res.Width, Res.Height);
				Options.Add(ResStr);
			}
		}
	}

	FStringOption(const TArray<FString>& InOptions, const int32 InSelectedIdx)
		: Options(InOptions),
		  SelectedIdx(InSelectedIdx)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	TArray<FString> Options;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	int32 SelectedIdx;
};

UENUM(BlueprintType)
enum class EDisplayMode : uint8
{
	Fullscreen UMETA(DisplayName = "全屏"),
	Windowed UMETA(DisplayName = "窗口"),
	WindowedFullscreen UMETA(DisplayName = "无边框窗口")
};

UCLASS(DefaultConfig, Config = Game, Blueprintable, BlueprintType)
class MELLOSUMG_API UGeneralGameSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General",
		meta = (DisplayName = "亮度", ClampMin="0", ClampMax = "2"), Config, Setter="SetBrightness")
	float Brightness = 1.f;

	UFUNCTION(BlueprintCallable, Category = "General")
	void SetBrightness(float InBrightness);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General",
		meta = (DisplayName = "对比度", ClampMin="0", ClampMax = "2"), Config)
	float Contrast = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General",
		meta = (DisplayName = "饱和度", ClampMin="0", ClampMax = "2"), Config)
	float Saturation = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General",
		meta = (DisplayName = "音量", ClampMin="0", ClampMax = "1"), Config)
	float Volume = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General",
		meta = (DisplayName = "显示模式", ClampMin="0"), Config)
	EDisplayMode DisplayMode = EDisplayMode::Windowed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General",
		meta = (DisplayName = "分辨率"), Config)
	FStringOption Resolution;
};
