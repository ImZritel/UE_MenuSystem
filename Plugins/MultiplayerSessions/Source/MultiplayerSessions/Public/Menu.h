// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Menu.generated.h"

//
// Class encapsulates functionality for handling the Menu widget.
// Usage: create a widget blueprint, change parent class of it to this class.
//
UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup();
protected:
	virtual bool Initialize() override;
private:
	UPROPERTY(meta=(BindWidget))
	class UButton* HostButton; // Name of button and a variable name in code should be strictly the same
	UPROPERTY(meta=(BindWidget))
	class UButton* JoinButton;
	// callbacks on button click:
	UFUNCTION()
	void HostButtonClicked();
	UFUNCTION()
	void JoinButtonClicked();
	
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem; // Subsystem designed to handle all online session functionality
};
