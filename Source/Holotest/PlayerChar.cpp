// Basic character which can walk around the level
// and fire against other players in a simple
// multiplayer environment.
//
// Made by Vander R. N. Dias for Hologate test.


#include "PlayerChar.h"
#include <Holotest/HolotestGameModeBase.h>

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

	// Get both weapon sockets from player mesh
	// ### for some reason it was not working, and as I lost
	// ### a good time (this ended up being a one day project),
	// ### I just hammered the coordinates from reference cubes
	// ### inside the BP, sorry.
	//WeaponOffsetLeft = PlayerMesh->GetSocketLocation("BoneSocket_Left");
	//WeaponOffsetRight = PlayerMesh->GetSocketLocation("BoneSocket_Right");
}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
}

// Character move
void APlayerChar::Move(float Value)
{
	// Move into forward direction
	FRotator Yaw = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

// Character strafe
void APlayerChar::Strafe(float Value)
{
	// Find right and move into that direction
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

// Character jump start
void APlayerChar::StartJump()
{
	bPressedJump = true;
}

// Character jump end
void APlayerChar::StopJump()
{
	bPressedJump = false;
}

// Fire
void APlayerChar::Fire()
{
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
