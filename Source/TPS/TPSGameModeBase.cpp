#include "TPSGameModeBase.h"
#include "Player/PController.h"

ATPSGameModeBase::ATPSGameModeBase()
{
	PlayerControllerClass = APController::StaticClass();
	DefaultPawnClass = ConstructorHelpers::FClassFinder<APawn>(TEXT("/Game/Blueprints/BP_Player")).Class;
	//HUDClass = APlayerHUD::StaticClass();
}
