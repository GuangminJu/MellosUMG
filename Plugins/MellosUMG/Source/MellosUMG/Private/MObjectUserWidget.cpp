#include "MObjectUserWidget.h"

UMObjectUserWidget::UMObjectUserWidget(): Object(nullptr)
{
}

void UMObjectUserWidget::PostDuplicate(bool bDuplicateForPIE)
{
	Super::PostDuplicate(bDuplicateForPIE);

	ReloadConfig();
}

void UMObjectUserWidget::OnSetProperty(FProperty* InProperty)
{
	FObjectProperty* ObjectProperty = CastField<FObjectProperty>(InProperty);
	Object = ObjectProperty->GetObjectPropertyValue(GetMemory());
	ensureMsgf(Object->GetClass() == ObjectClass, TEXT("Object class mismatch. Expected: %s, Actual: %s"),
	           *ObjectClass->GetName(), *Object->GetClass()->GetName());
}

void UMObjectUserWidget::CollectProperties()
{
	Properties.Reset();
	for (FProperty* SubProperty : TFieldRange<FProperty>(ObjectClass))
	{
		Properties.Add(SubProperty);
	}

	Functions.Empty();
	for (UFunction* Function : TFieldRange<UFunction>(ObjectClass))
	{
		if (Function->HasAnyFunctionFlags(FUNC_BlueprintCallable) && Function->GetOwnerClass() == ObjectClass)
		{
			Functions.Add(Function);
			
			if (FunctionSettings.Contains(Function->GetName()))
				continue;

			FFunctionSettings FunctionSetting;
			FunctionSetting.Name = Function->GetName();
			FunctionSettings.Add(FunctionSetting);
		}
	}

	TryUpdateDefaultConfigFile();
}

bool UMObjectUserWidget::IsPropertySupported(const FProperty* InProperty) const
{
	const FObjectProperty* InObjectProperty = CastField<FObjectProperty>(InProperty);
	return InObjectProperty->PropertyClass == ObjectClass;
}

TSubclassOf<UMUserWidgetBasicType> UMObjectUserWidget::GetSupportedWidgetClass(const FProperty* InProperty)
{
	for (const TSubclassOf<UMUserWidgetBasicType>& BasicTypeWidget : BasicTypeWidgets)
	{
		UMUserWidgetBasicType* UserWidgetBasic = BasicTypeWidget.GetDefaultObject();
		if (UserWidgetBasic->IsPropertySupported(InProperty))
		{
			return BasicTypeWidget;
		}
	}

	return nullptr;
}

TArray<UMUserWidgetBasicType*> UMObjectUserWidget::GenerateWidget()
{
	if (!Object)
	{
		Object = NewObject<UObject>(this, ObjectClass);
	}

	TArray<UMUserWidgetBasicType*> GeneratedWidgets;

	for (FProperty* SubProperty : Properties)
	{
		if (TSubclassOf<UMUserWidgetBasicType> Class = GetSupportedWidgetClass(SubProperty))
		{
			UMUserWidgetBasicType* Widget = NewObject<UMUserWidgetBasicType>(this, *Class);
			Widget->SetMemory(Object);
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

void UMObjectUserWidget::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
