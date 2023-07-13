// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MenuSystemCharacter.generated.h"


UCLASS(config=Game)
class AMenuSystemCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:
	AMenuSystemCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	//
	UFUNCTION(BlueprintCallable)
	void CreateGameSession();
	UFUNCTION(BlueprintCallable)
	void JoinGameSession();

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful); //Callback function, will be bound to delegate of type FOnCreateSessionCompleteDelegate
	void OnFindSessionsComplete(bool bWasSuccessful); //Callback function
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result); //Callback function
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Smart pointer to the online session interface:
	//TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe>
	IOnlineSessionPtr OnlineSessionInterface;

private:

	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate; // delegate for a session created event
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	TSharedPtr<FOnlineSessionSearch> SessionSearch; // session search results
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
};

