// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "UObject/Object.h"
#include "GeneralGameSettings.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStringOptionSelectedOptionChanged, int32, SelectedOption);

USTRUCT(Blueprintable)
struct FStringOption
{
	GENERATED_BODY()

	FStringOption(): SelectedIdx(0)
	{
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

	void SetSelectedOption(const FString& InOption);

	FStringOptionSelectedOptionChanged OnSelectedOptionChanged;
};

UENUM(BlueprintType)
enum class EDisplayMode : uint8
{
	Fullscreen UMETA(DisplayName = "全屏"),
	Windowed UMETA(DisplayName = "窗口"),
	WindowedFullscreen UMETA(DisplayName = "无边框窗口")
};

UCLASS(DefaultConfig, Config = Game, Blueprintable, BlueprintType)
class MELLOSUMG_API UGeneralGameSettings : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UGeneralGameSettings();

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General",
		meta = (DisplayName = "亮度增强", ClampMin="0", ClampMax = "2"), Config, Setter="SetBrightness")
	float Brightness = 0.f;

	UFUNCTION(BlueprintCallable, Category = "General")
	void SetBrightness(float InBrightness);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General",
		meta = (DisplayName = "音量", ClampMin="0", ClampMax = "1"), Config, Setter="SetVolume")
	float Volume = 1.f;

	UFUNCTION(BlueprintCallable, Category = "General")
	void SetVolume(float InVolume);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General",
		meta = (DisplayName = "显示模式", ClampMin="0"), Config, Setter="SetDisplayMode")
	EDisplayMode DisplayMode = EDisplayMode::Windowed;

	UFUNCTION(BlueprintCallable, Category = "General")
	void SetDisplayMode(EDisplayMode InDisplayMode);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General",
		meta = (DisplayName = "分辨率"), Config)
	FStringOption Resolution;

	TArray<FScreenResolutionRHI> ScreenResolutionRHI;
	int32 GetIndex(int32 InWidth, int32 InHeight) const;

public:
	UFUNCTION(BlueprintCallable, Category = "General")
	static void SetSelectedOption(UPARAM(ref) FStringOption& InResolution, const FString& InOption);

	UFUNCTION()
	void SetResolution(int32 SelectedOption);

	static EWindowMode::Type GetWindowMode(EDisplayMode InDisplayMode);
};
