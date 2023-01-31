// Basic character which can walk around the level
// and fire against other players in a simple
// multiplayer environment.
//
// Made by Vander R. N. Dias for Hologate test.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapon.h"
#include "PlayerChar.generated.h"

UCLASS()
class HOLOTEST_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Fire weapon from specific position (left or right)
	void FireAtPos(const FVector& Pos, const FRotator& Rot);

	// Server Fire RPC
	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector& Pos, const FRotator& Rot);
	void ServerFire_Implementation(const FVector& Pos, const FRotator& Rot);

	// Server "kill" player RPC
	UFUNCTION(Server, Reliable)
	void ServerKill();
	void ServerKill_Implementation();

	// Server respawn player at location RPC
	UFUNCTION(Server, Reliable)
	void ServerRespawn(const FVector& Pos);
	void ServerRespawn_Implementation(const FVector& Pos);

	// Timed HUD updates
	float Delay;

	// Alive flag
	bool IsAlive;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Forward/Backward movements input
	UFUNCTION()
	void Move(float Value);

	// Strafe left/right input
	UFUNCTION()
	void Strafe(float Value);

	// Jump start
	UFUNCTION()
	void StartJump();

	// Jump end
	UFUNCTION()
	void StopJump();

	// Fire
	UFUNCTION()
	void Fire();

	// Respawn
	UFUNCTION()
	void Respawn();

	// Player Camera
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	// Player Mesh
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* PlayerMesh;

	// Weapon class
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AWeapon> WeaponClass;

	// Weapon offsets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FVector WeaponOffsetLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FVector WeaponOffsetRight;

	// HUD updating
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void OnUpdateHUD(int Energy, int Score);

	// Show/hide respawn message
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void OnShowRespawnMsg(bool Show);
};
