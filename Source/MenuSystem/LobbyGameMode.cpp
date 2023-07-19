// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer){
    Super::PostLogin(NewPlayer);

    int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num(); // is needed because GameState (exissting as a variable on the AGameModeBase) is TObjectPtr
    if(GEngine) {
        GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Yellow, FString::Printf(TEXT("%d players in game."), NumberOfPlayers));

        APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
        if(PlayerState) {
            FString PlayerName = PlayerState->GetPlayerName();
            GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Cyan, FString::Printf(TEXT("%s joined the game."), *PlayerName));
        }
    }
    
}

void ALobbyGameMode::Logout(AController* Exiting){
    Super::Logout(Exiting);

    APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>();
    if(PlayerState && GEngine) {
        int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num(); // is needed because GameState (exissting as a variable on the AGameModeBase) is TObjectPtr
        GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Yellow, FString::Printf(TEXT("%d players in game."), NumberOfPlayers));

        FString PlayerName = PlayerState->GetPlayerName();
        GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Cyan, FString::Printf(TEXT("%s has exited the game."), *PlayerName));
    }
}
