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

USTRUCT(Blueprintable)
struct FPropertySettings
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Config)
	FString Name;

	UPROPERTY(EditAnywhere, Config)
	bool bGenerateUI = true;

	UPROPERTY(EditAnywhere, Config)
	TSubclassOf<UMUserWidgetBasicType> WidgetClassOverride;

	bool operator==(const FPropertySettings& RHS) const
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
	virtual void NativePreConstruct() override;
	virtual void OnSetProperty(FProperty* InProperty) override;
	
#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UObject> ObjectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SkipSerialization, meta=(ExposeOnSpawn), Setter="SetObject")
	UObject* Object;

	UFUNCTION(BlueprintCallable)
	void SetObject(UObject* InObject);

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

	const FFunctionSettings& GetFunctionSettings(const UFunction* InFunction) const;

	UPROPERTY(EditAnywhere, Config)
	TArray<FPropertySettings> PropertySettings;

	const FPropertySettings& GetPropertySettings(const FProperty* InProperty) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UUserWidget*> GeneratedWidgets;
};

inline const FFunctionSettings& UMObjectUserWidget::GetFunctionSettings(const UFunction* InFunction) const
{
	return *FunctionSettings.FindByPredicate(
		[InFunction](const FFunctionSettings& FunctionSetting)
		{
			return FunctionSetting == InFunction->GetName();
		});
}

inline const FPropertySettings& UMObjectUserWidget::GetPropertySettings(const FProperty* InProperty) const
{
	return *PropertySettings.FindByPredicate(
		[InProperty](const FPropertySettings& PropertySetting)
		{
			return PropertySetting == InProperty->GetName();
		});
}
