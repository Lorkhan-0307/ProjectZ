// Fill out your copyright notice in the Description page of Project Settings.


#include "Room/ZObjectBase.h"

// Sets default values
AZObjectBase::AZObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZObjectBase::OnBeginCursorOver(UPrimitiveComponent* TouchedComponent)
{
	
}

void AZObjectBase::OnEndCursorOver(UPrimitiveComponent* TouchedComponent)
{
	
}
