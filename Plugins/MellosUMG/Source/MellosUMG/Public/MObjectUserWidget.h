// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicType/MUserWidgetBasicType.h"
#include "StructUtils/InstancedStruct.h"

#include "MObjectUserWidget.generated.h"


class UFunctionUserWidget;

USTRUCT(Blueprintable)
struct FFunctionSettings
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Config)
	FString Name;

	UPROPERTY(EditAnywhere, Config)
	bool bGenerateUI = false;

	UPROPERTY(EditAnywhere, Config)
	TSubclassOf<UFunctionUserWidget> WidgetClassOverride;

	bool operator==(const FFunctionSettings& RHS) const
	{
		return Name == RHS.Name;
	}

	bool operator==(const FString& InName) const
	{
		return Name == InName;
	}
};

UCLASS(Abstract, DefaultConfig, Config = MObjectUserWidget)
class MELLOSUMG_API UMObjectUserWidget : public UMUserWidgetBasicType
{
	GENERATED_BODY()

	UMObjectUserWidget();
	virtual void PostDuplicate(bool bDuplicateForPIE) override;
	virtual void OnSetProperty(FProperty* InProperty) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UObject> ObjectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SkipSerialization)
	UObject* Object;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void CollectProperties();

	virtual bool IsPropertySupported(const FProperty* InProperty) const override;
	
	TSubclassOf<UMUserWidgetBasicType> GetSupportedWidgetClass(const FProperty* InProperty);

	UFUNCTION(BlueprintCallable, CallInEditor)
	TArray<UUserWidget*> GenerateWidget();

	TArray<FProperty*> Properties;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UMUserWidgetBasicType>> BasicTypeWidgets;
	
	UPROPERTY(EditAnywhere, Config)
	TSubclassOf<UFunctionUserWidget> FunctionWidgetClass;

	TArray<UFunction*> Functions;

	UPROPERTY(EditAnywhere, Config)
	TArray<FFunctionSettings> FunctionSettings;
};