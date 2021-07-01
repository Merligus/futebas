// Fill out your copyright notice in the Description page of Project Settings.


#include "CopaWidget.h"

bool UCopaWidget::Initialize()
{
    bool success = Super::Initialize();
    if (!success)
        return false;

    // if (!ensure(Button_535 != nullptr))
    //     return false;
    // Button_535->OnClicked.AddDynamic(this, &UCopaWidget::ButtonClicked);

    return true;
}

// void UCopaWidget::ButtonClicked()
// {

// }
