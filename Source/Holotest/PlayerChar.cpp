// Basic character which can walk around the level
// and fire against other players in a simple
// multiplayer environment.
//
// Made by Vander R. N. Dias for Hologate test.


#include "PlayerChar.h"

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Get the camera component and attach to the collider
	USceneComponent* Collider = CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent());
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(Collider);
	CameraComponent->SetRelativeLocation(FVector(20.0f, 0.0f, 0.0f));
	CameraComponent->bUsePawnControlRotation = true;

	// Also attach the player mesh to the camera component
	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	PlayerMesh->SetupAttachment(CameraComponent);
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