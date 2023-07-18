// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

void UMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch) {
    NumPublicConnections = NumberOfPublicConnections;
    MatchType = TypeOfMatch;

    AddToViewport();
    SetVisibility(ESlateVisibility::Visible);
    bIsFocusable = true;

    UWorld* World = GetWorld();
    if (World) {
        APlayerController* PlayerController = World->GetFirstPlayerController();
        if(PlayerController) {
            FInputModeUIOnly InputModeData; // allows to focus on UI
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

    // Binding callback function to the delegate defined on MultiplayerSessionnsSubsystem:
    if(MultiplayerSessionsSubsystem) {
        MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &UMenu::OnCreateSession);
        MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &UMenu::OnFindSessions); // AddUObject instead of AddDynamic for non UOBJECT
        MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &UMenu::OnJoinSession);
        MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &UMenu::OnDestroySession);
        MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &UMenu::OnStartSession);
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
    if (MultiplayerSessionsSubsystem) {
        // creating the session:
        MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);
    }
}
void UMenu::JoinButtonClicked() {
    if (MultiplayerSessionsSubsystem) {
        MultiplayerSessionsSubsystem->FindSessions(10000);
    }
}

void UMenu::MenuTearDown() {
    RemoveFromParent(); // remove widget
    UWorld* World = GetWorld();
    if (World) {
        APlayerController* PlayerController = World->GetFirstPlayerController();
        if (PlayerController) {
            FInputModeGameOnly InputModeData; // to focus on the game instead of UI
            PlayerController->SetInputMode(InputModeData);
            PlayerController->SetShowMouseCursor(false);
        }
    }
}

void UMenu::NativeDestruct() {
    MenuTearDown();
    Super::NativeDestruct();
}

void UMenu::OnCreateSession(bool bWasSuccessful) {
    if(bWasSuccessful) {
        if(GEngine) {
            GEngine->AddOnScreenDebugMessage(
            -1,
            15.f,
            FColor::Green,
            FString(TEXT("Session created successfully!"))
            );
        }
        // travel to the lobby lvl:
        UWorld* World = GetWorld();
        if (World) {
            World->ServerTravel(FString("/Game/ThirdPerson/Maps/Lobby?listen"));
        }
    } else {
        if(GEngine) {
            GEngine->AddOnScreenDebugMessage(
            -1,
            15.f,
            FColor::Green,
            FString(TEXT("Failed to create session!"))
            );
        }
    }
}

void UMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful){
    if (!MultiplayerSessionsSubsystem) { return; }
    
    for (const auto& Result : SessionResults) {
        FString SettingsValue;
        Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
        if (SettingsValue == MatchType) {
            MultiplayerSessionsSubsystem->JoinSession(Result);
            return;
        }
    }
}

void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result){
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem) {
        IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
        if(SessionInterface.IsValid()) {
            FString Address;
            SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);
            
            APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
            if(PlayerController) {
                PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
            }
        }
    }
}

void UMenu::OnDestroySession(bool bWasSuccessful){
}

void UMenu::OnStartSession(bool bWasSuccessful){
}
