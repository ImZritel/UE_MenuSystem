// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Button.h"
#include "Menu.h"
#include "MultiplayerSessionsSubsystem.h"

void UMenu::MenuSetup() {
    AddToViewport();
    SetVisibility(ESlateVisibility::Visible);
    bIsFocusable = true;

    UWorld* World = GetWorld();
    if (World) {
        APlayerController* PlayerController = World->GetFirstPlayerController();
        if(PlayerController) {
            FInputModeUIOnly InputModeData;
            InputModeData.SetWidgetToFocus(TakeWidget());
            InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PlayerController->SetInputMode(InputModeData);
            PlayerController->SetShowMouseCursor(true);
        }
    }
    UGameInstance* GameInstance = GetGameInstance(); // Ptr to game instance subsystem (multiplayer sessions subsystem)
    if (GameInstance) {
        MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
    }
}

bool UMenu::Initialize() {
    if(!Super::Initialize()) {return false;}

    if(HostButton) {
        HostButton->OnClicked.AddDynamic(this, &UMenu::HostButtonClicked); // bind callback function HostButtonClicked to the dynamic multicast delegate (event) OnClicked
    }
    if(JoinButton) {
        JoinButton->OnClicked.AddDynamic(this, &UMenu::JoinButtonClicked); // bind callback function JoinButtonClicked to the dynamic multicast delegate OnClicked
    }
    return true;
}

void UMenu::HostButtonClicked() {
    if(GEngine) {
        GEngine->AddOnScreenDebugMessage(
        -1,
        15.f,
        FColor::Yellow,
        FString(TEXT("Host button clicked."))
        );
    }
    if (MultiplayerSessionsSubsystem) {
        // creating the session:
        MultiplayerSessionsSubsystem->CreateSession(4, FString("Free for all"));
        // travel to the lobby lvl:
        UWorld* World = GetWorld();
        if (World) {
            World->ServerTravel("/Game/ThirdPerson/Maps/Lobby.umap?listen");
        }
    }
}
void UMenu::JoinButtonClicked() {
    if(GEngine) {
        GEngine->AddOnScreenDebugMessage(
        -1,
        15.f,
        FColor::Yellow,
        FString(TEXT("Join button clicked."))
        );
    }
}