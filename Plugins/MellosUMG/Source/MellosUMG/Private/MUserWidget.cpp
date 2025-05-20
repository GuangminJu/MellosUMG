// Fill out your copyright notice in the Description page of Project Settings.

#include "MUserWidget.h"

void UMUserWidget::CollectProperties()
{
	Properties.Empty();

	for (FProperty* Property : TFieldRange<FProperty>(InstanceStruct.GetScriptStruct()))
	{
		Properties.Add(Property);
	}
}

TSubclassOf<UMUserWidgetBasicType> UMUserWidget::GetSupportedWidgetClass(const FFieldClass* FieldClass)
{
	for (const TSubclassOf<UMUserWidgetBasicType>& BasicType : BasicTypeWidgets)
	{
		if (!BasicType)
			continue;

		UMUserWidgetBasicType* UserWidgetBasic = BasicType.GetDefaultObject();
		if (UserWidgetBasic->GetSupportedFieldClass() == FieldClass)
		{
			return BasicType;
		}
	}

	return nullptr;
}

TArray<UMUserWidgetBasicType*> UMUserWidget::GenerateWidget()
{
	TArray<UMUserWidgetBasicType*> GeneratedWidgets;

	for (FProperty* Property : Properties)
	{
		FFieldClass* FieldClass = Property->GetClass();

		if (TSubclassOf<UMUserWidgetBasicType> Class = GetSupportedWidgetClass(FieldClass))
		{
			UMUserWidgetBasicType* Widget = NewObject<UMUserWidgetBasicType>(this, *Class);
			Widget->SetProperty(Property);
			Widget->SetMemory(InstanceStruct.GetMutableMemory());
			GeneratedWidgets.Add(Widget);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No widget class found for property type: %s"), *FieldClass->GetName());
		}
	}

	return GeneratedWidgets;
}

#if WITH_EDITOR

void UMUserWidget::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UMUserWidget, InstanceStruct))
	{
		CollectProperties();
	}
}

#endif
