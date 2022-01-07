// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());

}


// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TankPlayerController)
	{
		FHitResult HitResult;
		if (TankPlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility,
			false,
			HitResult))
		{
			FColor TargetColor = FColor::Red;
			if (HitResult.Actor != nullptr) //TODO:and actor is turret
			{
				TargetColor = FColor::Green;
			}

			//DrawDebugSphere(
			//	GetWorld(),
			//	//GetActorLocation() + FVector(0.f, 0.f, 200.f),
			//	HitResult.ImpactPoint,
			//	20.f,
			//	24,
			//	TargetColor,
			//	false,
			//	-1.f);

			ABasePawn::RotateTurret(HitResult.ImpactPoint);
		}
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);

	bAlive = false;
}



void ATank::Move(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Value: %f"), Value);

	//FVector DeltaLocation(0.f);//alternative declaration
	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = Value * Speed * /*delta time*/UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Value: %f"), Value);

	//FVector DeltaLocation(0.f);//alternative declaration
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = Value * TurnRate * /*delta time*/UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(DeltaRotation, true);
}
