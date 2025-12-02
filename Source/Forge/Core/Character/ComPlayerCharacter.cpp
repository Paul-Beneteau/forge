#include "ComPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Forge/Core/GAS/ComAbilitySystemComponent.h"
#include "Forge/Core/GAS/ComCombatAttributeSet.h"
#include "Forge/Core/GAS/ComDamageModifierAttributeSet.h"
#include "Forge/Item/Components/ItmEquipmentComponent.h"
#include "Forge/Item/Components/ItmInventoryComponent.h"
#include "Forge/Item/Managers/ItmInventoryManager.h"
#include "Forge/Item/UI/ItmInventoryRootWidget.h"
#include "Forge/MapGenerator/MapGenerator.h"
#include "Kismet/GameplayStatics.h"

AComPlayerCharacter::AComPlayerCharacter()
{	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// Don't want arm to rotate when character does
	SpringArmComp->SetUsingAbsoluteRotation(true); 
	SpringArmComp->TargetArmLength = 800.f;
	SpringArmComp->SetRelativeRotation(FRotator(-55.f, 0.f, 0.f));
	// Don't want to pull camera in when it collides with level
	SpringArmComp->bDoCollisionTest = false;

	// Camera does not rotate relative to arm
	CameraComp->bUsePawnControlRotation = false; 
	
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	// Increase update frequency for GAS components
	SetNetUpdateFrequency(100.0f);
	AbilitySystemComp = CreateDefaultSubobject<UComAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	CombatAttributeSet = CreateDefaultSubobject<UComCombatAttributeSet>(TEXT("CombatAttributeSet"));
	AbilitySystemComp->AddAttributeSetSubobject<UComCombatAttributeSet>(CombatAttributeSet);
	DamageAttributeSet = CreateDefaultSubobject<UComDamageModifierAttributeSet>(TEXT("DamageAttributeSet"));
	AbilitySystemComp->AddAttributeSetSubobject<UComDamageModifierAttributeSet>(DamageAttributeSet);

	CombatAttributeSet->OnHealthChanged.AddDynamic(this, &AComPlayerCharacter::HandleHealthChanged);

	EquipmentComp = CreateDefaultSubobject<UItmEquipmentComponent>(TEXT("EquipmentComp"));
	InventoryComp = CreateDefaultSubobject<UItmInventoryComponent>(TEXT("InventoryComp"));
}

// Binds Input actions from PlayerConfig DataSet with their corresponding callbacks
void AComPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerConfig && PlayerConfig->IsValid());
	
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		InputSubsystem->AddMappingContext(PlayerConfig->DefaultInputContext, 0);
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Bind movement input
		EnhancedInputComponent->BindAction(PlayerConfig->Move, ETriggerEvent::Started, this, &AComPlayerCharacter::OnInputStarted);
		EnhancedInputComponent->BindAction(PlayerConfig->Move, ETriggerEvent::Triggered, this, &AComPlayerCharacter::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(PlayerConfig->Move, ETriggerEvent::Completed, this, &AComPlayerCharacter::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(PlayerConfig->Move, ETriggerEvent::Canceled, this, &AComPlayerCharacter::OnSetDestinationReleased);

		// Bind Abilities input. Saves handle to modify dynamically input ability binding
		for (FComAbilityInput AbilityInput : PlayerConfig->InitialAbilities)
		{			
			if (UInputAction* InputAction = AbilityInput.InputAction.Get(nullptr))
			{
				uint32 Handle = EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started,this,
					&AComPlayerCharacter::OnActivateAbilityStarted, AbilityInput.Ability).GetHandle();
			
				InputHandleMap.Add(InputAction, Handle);				
				InputAbilityMap.Add(InputAction, AbilityInput.Ability);
			}			
		}		
	}
}

void AComPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Initialize GAS
	AbilitySystemComp->InitAbilityActorInfo(this, this);
	
	for (FComAbilityInput AbilityInput : PlayerConfig->InitialAbilities)
	{
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(AbilityInput.Ability));
	}

	AbilitySystemComp->ApplyGameplayEffectToSelf(InitialGameplayEffect->GetDefaultObject<UGameplayEffect>(), 1.0f, AbilitySystemComp->MakeEffectContext());

	CombatAttributeSet->InitPeriodicAttributes();	
}

UAbilitySystemComponent* AComPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void AComPlayerCharacter::OnActivateAbilityStarted(const TSubclassOf<UGameplayAbility> AbilityClass)
{
	AbilitySystemComp->TryActivateAbilityByClass(AbilityClass);
}

// Remove the current ability bound to the input action and binds the new ability
void AComPlayerCharacter::SetInputActionAbility(UInputAction* InputAction, TSubclassOf<UGameplayAbility> Ability)
{
	check(InputAction && Ability);
	
	if (UEnhancedInputComponent* EnhancedInputComponent { Cast<UEnhancedInputComponent>(InputComponent) })
	{
		// Iterate to find if the ability added was already bound with another input to prevent an ability to be used by
		// 2 input actions. It is not efficient to iterate with a TMap but as there is only a few input it's fine.
		const UInputAction* AbilityCurrentInputAction { nullptr };			

		for (const TPair<TObjectPtr<UInputAction>, TSubclassOf<UGameplayAbility>>& Pair : InputAbilityMap)
		{
			if (Pair.Value == Ability)
			{
				AbilityCurrentInputAction = Pair.Key;
			}
		}

		// Remove current input action bound to the ability if the ability was already used by an input
		if (AbilityCurrentInputAction)
		{
			if (uint32* InputActionHandle = InputHandleMap.Find(AbilityCurrentInputAction))
			{
				EnhancedInputComponent->RemoveBindingByHandle(*InputActionHandle);

				InputHandleMap.Remove(AbilityCurrentInputAction);
				InputAbilityMap.Remove(AbilityCurrentInputAction);
			}
		}

		if (uint32* InputActionHandle = InputHandleMap.Find(InputAction))
		{
			// Remove current ability bound to the input action
			EnhancedInputComponent->RemoveBindingByHandle(*InputActionHandle);
		
			InputHandleMap.Remove(InputAction);
			InputAbilityMap.Remove(InputAction);
		}
		
		// Add new binding and save the handle and ability
		uint32 NewHandle { EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started,
	this, &AComPlayerCharacter::OnActivateAbilityStarted, Ability).GetHandle() };

		InputHandleMap.Add(InputAction, NewHandle);
		InputAbilityMap.Add(InputAction, Ability);
	}
}

void AComPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InventoryManager = NewObject<UItmInventoryManager>(this);
	InventoryManager->Initialize(GetController<APlayerController>(), InventoryComp, EquipmentComp, PlayerConfig->InventoryWidgetClass);
}

void AComPlayerCharacter::OnInputStarted()
{
	GetController()->StopMovement();

	// Tell there is an outside click to the inventory. Used to put item into the world.
	if (UItmInventoryRootWidget* InventoryWidget = InventoryManager->GetInventoryRoot())
		InventoryWidget->HandleClickOutside();
}

// Move toward destination while input is triggered
void AComPlayerCharacter::OnSetDestinationTriggered()
{
	SetDestinationTriggerDuration += GetWorld()->GetDeltaSeconds();

	if (APlayerController* PlayerController { Cast<APlayerController>(GetController()) })
	{
		// Get destination location at the cursor position
		FHitResult Hit;
		if (PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit))
		{
			// Move towards destination
			FVector WorldDirection { (Hit.Location - GetActorLocation()).GetSafeNormal() };
			AddMovementInput(WorldDirection, 1.0, false);
		}
	}
}

// Move to destination if this was a short click
void AComPlayerCharacter::OnSetDestinationReleased()
{
	// If this is a short click
	if (SetDestinationTriggerDuration <= ClickToDestinationThreshold)
	{
		// Get destination location at the cursor position 
		if (APlayerController* PlayerController { Cast<APlayerController>(GetController()) })
		{
			FHitResult Hit;
			if (PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit))
			{
				// Move to destination
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Hit.Location);
			}
		}		
	}

	SetDestinationTriggerDuration = 0.f;
}

void AComPlayerCharacter::HandleHealthChanged(AActor* EffectInstigator, float OldValue, float NewValue)
{
	if (NewValue <= 0.f && OldValue >= 0.f)
		Die();
}

void AComPlayerCharacter::Die()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		DisableInput(PlayerController);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (PlayerConfig->DeathMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(PlayerConfig->DeathMontage);

			// Enable ragdoll for 1s so the character lay on the ground because death montage ends at a weird place.
			// TODO: fix death anim montage
			FOnMontageBlendingOutStarted BlendingOutDelegate;
			BlendingOutDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
			{
				GetMesh()->bPauseAnims = true;

				// Enable ragdoll
				GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				GetMesh()->SetSimulatePhysics(true);

				FTimerHandle TimerHandle;
				auto DisableRagdoll = [this](){ GetMesh()->SetSimulatePhysics(false); };
				GetWorldTimerManager().SetTimer(TimerHandle, DisableRagdoll, 1.f, false);
				
			});
			
			AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, PlayerConfig->DeathMontage);
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AComPlayerCharacter: DeathMontage has not been set"));
	}

	// Respawn after delay
	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AComPlayerCharacter::Respawn, PlayerConfig->RespawnDelay, false);
}

void AComPlayerCharacter::Respawn()
{
	if (AMapGenerator* MapGenerator = Cast<AMapGenerator>(UGameplayStatics::GetActorOfClass(GetWorld(), AMapGenerator::StaticClass())))
		MapGenerator->TeleportPlayerToStart();
	else
		UE_LOG(LogTemp, Error, TEXT("AComPlayerCharacter: Can't set respawn location because there is no map generator"));

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->bPauseAnims = false;

	// Reset mesh after ragdoll
	// TODO: parametrize values
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f),FRotator(0.0f, -90.0f, 0.0f));
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		EnableInput(PlayerController);

	if (AbilitySystemComp && InitialGameplayEffect)
			AbilitySystemComp->ApplyGameplayEffectToSelf(InitialGameplayEffect.GetDefaultObject(), 1.0f, AbilitySystemComp->MakeEffectContext());

	OnPlayerRespawn.Broadcast();
}