// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHole.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class URadialForceComponent;

UCLASS()
class FPSGAME_API ABlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackHole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	// Inner sphere that destroys the overlapping components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* InnerCollision;

	// Outer sphere pulls components (that are physically simulating) towards the center of the actor
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* OuterCollision;

	// Marked with ufunction to bind to overlap event
	UFUNCTION()
	void OverlapInnerSphere(UPrimitiveComponent* OverlappendComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/*UFUNCTION()
	URadialForceComponent* RadialForce;*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
