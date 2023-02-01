// Basic character which can walk around the level
// and fire against other players in a simple
// multiplayer environment.
//
// Made by Vander R. N. Dias for Hologate test.


#include "PlayerChar.h"
#include <Holotest/HolotestGameModeBase.h>
#include <Holotest/HolotestPlayerState.h>

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera component and attach to the collider
	USceneComponent* Collider = CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent());
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(Collider);
	CameraComponent->SetRelativeLocation(FVector(20.0f, 0.0f, 0.0f));
	CameraComponent->bUsePawnControlRotation = true;

	// Also attach the player mesh to the camera component
	PlayerMesh = GetMesh();
	PlayerMesh->SetupAttachment(CameraComponent);

	// Starts alive
	IsAlive = true;
}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();
	Delay = 0;
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// We only update HUD from time to time (half a second)
	APlayerController* PController = GetController<APlayerController>();
	if (PController && PController->IsLocalPlayerController())
	{
		AHolotestPlayerState* State = GetPlayerState<AHolotestPlayerState>();
		if (State)
		{
			uint16 Energy = State->GetEnergy();

			if (IsAlive && (Energy == 0 || GetActorLocation().Z < -400))
			{
				// Dead
				IsAlive = false;
				OnShowRespawnMsg(FText::FromString("SPECTATING - PRESS [ENTER] TO RESPAWN"));
				GetCapsuleComponent()->SetPhysicsLinearVelocity(FVector::ZeroVector);
				ServerKill();
			}

			Delay += DeltaTime;
			if (Delay >= 0.5)
			{
				OnUpdateHUD(Energy, State->GetScore());
				Delay = 0;
			}
		}
	}
}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind movement and rotation controls
	PlayerInputComponent->BindAxis("Move", this, &APlayerChar::Move);
	PlayerInputComponent->BindAxis("Strafe", this, &APlayerChar::Strafe);
	PlayerInputComponent->BindAxis("RotateYaw", this, &APlayerChar::AddControllerYawInput);
	PlayerInputComponent->BindAxis("RotatePitch", this, &APlayerChar::AddControllerPitchInput);

	// Bind jump action
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerChar::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerChar::StopJump);

	// Bind fire action
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerChar::Fire);

	// Bind respawn action
	PlayerInputComponent->BindAction("Respawn", IE_Pressed, this, &APlayerChar::Respawn);
}

// Character move
void APlayerChar::Move(float Value)
{
	// Dead players can't move
	if (!IsAlive) return;

	// Move into forward direction
	FRotator Yaw = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

// Character strafe
void APlayerChar::Strafe(float Value)
{
	// Dead players can't move
	if (!IsAlive) return;

	// Find right and move into that direction
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

// Character jump start
void APlayerChar::StartJump()
{
	// Dead players can't move
	if (!IsAlive) return;

	bPressedJump = true;
}

// Character jump end
void APlayerChar::StopJump()
{
	// Dead players can't move
	if (!IsAlive) return;

	bPressedJump = false;
}

// Fire
void APlayerChar::Fire()
{
	// Dead players can't fire
	if (!IsAlive) return;

	// Must setup Weapon in BP
	if (WeaponClass)
	{
		// Get the camera transform
		FVector CameraLocation = GetActorLocation();
		FRotator CameraRotation = GetActorRotation();

		// Transform WeaponOffsetLeft from camera space to world space
		FVector LocationLeft = CameraLocation + FTransform(CameraRotation).TransformVector(WeaponOffsetLeft);
		FVector LocationRight = CameraLocation + FTransform(CameraRotation).TransformVector(WeaponOffsetRight);

		if (!HasAuthority())
		{
			// Fire at Server
			ServerFire(LocationLeft, CameraRotation);
			ServerFire(LocationRight, CameraRotation);
		}
		else
		{
			// Fire at Client
			FireAtPos(LocationLeft, CameraRotation);
			FireAtPos(LocationRight, CameraRotation);
		}
	}
}

// Fire weapon from specific position (left or right)
void APlayerChar::FireAtPos(const FVector& Pos, const FRotator& Rot)
{
	UWorld* World = GetWorld();

	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// Spawn new weapon projectile
		AWeapon* Projectile = World->SpawnActor<AWeapon>(WeaponClass, Pos, Rot, SpawnParams);
		if (Projectile)
		{
			// Fire weapon
			Projectile->SetReplicates(true);
			Projectile->SetReplicateMovement(true);
			FVector LaunchDirection = Rot.Vector();
			Projectile->Fire(LaunchDirection);
		}
	}
}

// Server Fire RPC implementation
void APlayerChar::ServerFire_Implementation(const FVector& Pos, const FRotator& Rot)
{
	FireAtPos(Pos, Rot);
}

// Server kill player RPC
void APlayerChar::ServerKill_Implementation()
{
	// Choose a random place to spectate
	FVector Pos = FVector(FMath::RandRange(-250, 250), 0, FMath::RandRange(450.0f, 550.0f));
	SetActorLocation(Pos);
	GetCapsuleComponent()->SetPhysicsLinearVelocity(FVector::ZeroVector);
}

// Respawn
void APlayerChar::Respawn()
{
	// No respawn if alive
	if (IsAlive) return;

	// Move to some random place above
	FVector Pos = FVector(FMath::RandRange(-300.0f, 300.0f), FMath::RandRange(-350.0f, 350.0f), FMath::RandRange(250.0f, 350.0f));
	SetActorLocation(Pos);

	// Reset energy
	AHolotestPlayerState* State = GetPlayerState<AHolotestPlayerState>();
	State->Reset();

	UE_LOG(LogTemp, Warning, TEXT("CLIENT %s AT %f %f %f"), *GetName(), Pos.X, Pos.Y, Pos.Z);

	if (!HasAuthority())
	{
		ServerRespawn(Pos);
		UE_LOG(LogTemp, Warning, TEXT("SERVER RESPAWNING %s AT %f %f %f"), *GetName(), Pos.X, Pos.Y, Pos.Z);
	}

	// Live again
	IsAlive = true;
	OnShowRespawnMsg(FText::FromString(""));
	UE_LOG(LogTemp, Warning, TEXT("CLIENT %s SHOULD BE ALIVE NOW..."), *GetName());
}

// Server Respawn
void APlayerChar::ServerRespawn_Implementation(const FVector& Pos)
{
	SetActorLocation(Pos);

	// Reset energy
	AHolotestPlayerState* State = GetPlayerState<AHolotestPlayerState>();
	State->Reset();
}
