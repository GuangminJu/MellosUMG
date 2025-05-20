// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MUserWidgetBasicType.h"
#include "StructUtils/InstancedStruct.h"

#include "MUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MELLOSUMG_API UMUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInstancedStruct InstanceStruct;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void CollectProperties();
	
	TSubclassOf<UMUserWidgetBasicType> GetSupportedWidgetClass(const FFieldClass* FieldClass);

	UFUNCTION(BlueprintCallable, CallInEditor)
	TArray<UMUserWidgetBasicType*> GenerateWidget();

	TArray<FProperty*> Properties;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UMUserWidgetBasicType>> BasicTypeWidgets;
};