// Fill out your copyright notice in the Description page of Project Settings.

#include "MStructUserWidget.h"

void UMStructUserWidget::OnSetProperty(FProperty* InProperty)
{
	FStructProperty* StructProperty = CastField<FStructProperty>(InProperty);
	if (!StructProperty)
	{
		UE_LOG(LogTemp, Warning, TEXT("Property is not a struct property: %s"), *InProperty->GetName());
		return;
	}

	void* StructMemory = InProperty->ContainerPtrToValuePtr<void>(GetMemory());
	InstanceStruct.InitializeAs(StructProperty->Struct, static_cast<uint8*>(StructMemory));
	SetMemory(InstanceStruct.GetMutableMemory());
}

TArray<FProperty*> UMStructUserWidget::GetProperties()
{
	CollectProperties();
	
	return Properties;
}

void UMStructUserWidget::NativePreConstruct()
{
	LoadConfig();

	Super::NativePreConstruct();
}

void UMStructUserWidget::SetInstanceStruct(const FInstancedStruct& InInstanceStruct)
{
	if (InstanceStruct.GetScriptStruct() != InInstanceStruct.GetScriptStruct())
	{
		UE_LOG(LogTemp, Warning, TEXT("Struct type mismatch. Expected: %s, Actual: %s"),
			*InstanceStruct.GetScriptStruct()->GetName(), *InInstanceStruct.GetScriptStruct()->GetName());
		return;
	}
	
	InstanceStruct = InInstanceStruct;
	SetMemory(InstanceStruct.GetMutableMemory());

	for (UUserWidget* Widget : GeneratedWidgets)
	{
		if (UMUserWidgetBasicType* UserWidgetBasic = Cast<UMUserWidgetBasicType>(Widget))
		{
			UserWidgetBasic->SetMemory(InstanceStruct.GetMutableMemory());
		}
		else
		{
			ensureMsgf(false, TEXT("Widget is not of type UMUserWidgetBasicType: %s"), *Widget->GetName());
		}
	}
}

void UMStructUserWidget::CollectProperties()
{
	Properties.Empty();

	for (FProperty* SubProperty : TFieldRange<FProperty>(InstanceStruct.GetScriptStruct()))
	{
		Properties.Add(SubProperty);
	}
}

bool UMStructUserWidget::IsPropertySupported(const FProperty* InProperty) const
{
	const FStructProperty* StructProperty = CastField<FStructProperty>(InProperty);
	if (!StructProperty)
		return false;

	return InstanceStruct.GetScriptStruct() == StructProperty->Struct;
}

TSubclassOf<UMUserWidgetBasicType> UMStructUserWidget::GetSupportedWidgetClass(const FProperty* InProperty)
{
	for (const TSubclassOf<UMUserWidgetBasicType>& BasicType : BasicTypeWidgets)
	{
		if (!BasicType)
			continue;

		UMUserWidgetBasicType* UserWidgetBasic = BasicType.GetDefaultObject();
		if (UserWidgetBasic->IsPropertySupported(InProperty))
		{
			return BasicType;
		}
	}

	return nullptr;
}

TArray<UUserWidget*> UMStructUserWidget::GenerateWidget()
{
	GeneratedWidgets.Empty();

	for (FProperty* SubProperty : Properties)
	{
		if (TSubclassOf<UMUserWidgetBasicType> Class = GetSupportedWidgetClass(SubProperty))
		{
			UMUserWidgetBasicType* Widget = NewObject<UMUserWidgetBasicType>(this, *Class);
			Widget->SetMemory(InstanceStruct.GetMutableMemory());
			Widget->SetProperty(SubProperty);
			GeneratedWidgets.Add(Widget);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No widget class found for property : %s"), *SubProperty->GetName());
		}
	}

	return GeneratedWidgets;
}

#if WITH_EDITOR

void UMStructUserWidget::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UMStructUserWidget, InstanceStruct))
	{
		CollectProperties();
	}
}

#endif
