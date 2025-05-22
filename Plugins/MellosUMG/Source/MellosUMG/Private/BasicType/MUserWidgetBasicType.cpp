// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicType/MUserWidgetBasicType.h"

#include "Blueprint/WidgetTree.h"

void UMUserWidgetBasicType::NativePreConstruct()
{
	TryInitFromParentWidget();

	Super::NativePreConstruct();
}

void UMUserWidgetBasicType::NativeConstruct()
{
	TryInitFromParentWidget();

	Super::NativeConstruct();
}

bool UMUserWidgetBasicType::TryInitFromParentWidget()
{
	if (Property)
		return false;

	UMUserWidgetBasicType* UmUserWidgetBasic = GetTypedOuter<UMUserWidgetBasicType>();

	if (!UmUserWidgetBasic)
		return false;

	if (UmUserWidgetBasic == this)
		return false;

	ParentMWidget = UmUserWidgetBasic;
	if (!ParentMWidget)
		return false;
	
	SetMemory(ParentMWidget->GetMemory());
	SetPropertyName(PropertyName);
	return true;
}

#if WITH_EDITOR
void UMUserWidgetBasicType::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SetPropertyName(PropertyName);
}
#endif

TArray<FString> UMUserWidgetBasicType::GetPropertyNameOptions() const
{
	TArray<FString> Options;

	if (!ParentMWidget)
	{
		return Options;
	}

	TArray<FProperty*> Properties = ParentMWidget->GetProperties();
	for (FProperty* SubProperty : Properties)
	{
		if (SubProperty && IsPropertySupported(SubProperty))
		{
			Options.Add(SubProperty->GetName());
		}
	}

	return Options;
}

void UMUserWidgetBasicType::SetPropertyName(const FString& InPropertyName)
{
	PropertyName = InPropertyName;

	if (!ParentMWidget)
	{
		return;
	}

	for (FProperty* SubProperty : ParentMWidget->GetProperties())
	{
		if (SubProperty && SubProperty->GetName() == PropertyName)
		{
			SetProperty(SubProperty);
			PropertyDisplayName = SubProperty->GetDisplayNameText().ToString();
			break;
		}
	}
}
