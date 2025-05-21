#include "MObjectUserWidget.h"

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
		Object = ObjectClass.GetDefaultObject();
	}
	
	TArray<UMUserWidgetBasicType*> GeneratedWidgets;

	for (FProperty* SubProperty : Properties)
	{
		if (TSubclassOf<UMUserWidgetBasicType> Class = GetSupportedWidgetClass(SubProperty))
		{
			UMUserWidgetBasicType* Widget = NewObject<UMUserWidgetBasicType>(this, *Class);
			Widget->SetProperty(SubProperty);
			Widget->SetMemory(Object);
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
