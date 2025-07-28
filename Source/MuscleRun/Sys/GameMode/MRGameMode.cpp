#include "Sys/GameMode/MRGameMode.h"
#include "Sys/GameState/MRGameState.h"
#include "GameFramework/Character.h" // ���ø� ���� ����
#include "Kismet/GameplayStatics.h"

AMRGameMode::AMRGameMode()
{
	// ����� �⺻ GameState Ŭ������ �����մϴ�.
	GameStateClass = AMRGameState::StaticClass();
}

void AMRGameMode::BeginPlay()
{
	Super::BeginPlay();

	// ������ ���۵Ǹ�, StartGame �Լ��� ȣ���Ͽ� ���� �帧�� �����մϴ�.
	StartGame();

	// TODO: ĳ������ OnDeath ��������Ʈ�� PlayerDied �Լ��� ���ε��ϴ� ������ �ʿ��մϴ�.
	// ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	// if (PlayerCharacter) { PlayerCharacter->OnDeath.AddDynamic(this, &AMRGameMode::PlayerDied); }
}

void AMRGameMode::AddScore(int32 ScoreToAdd)
{
	// 1. '������'�� GameState�� �����ɴϴ�.
	if (AMRGameState* GS = GetGameState<AMRGameState>())
	{
		// 2. GameState�� ���� ���� �Լ��� ȣ���Ͽ� ���� ������Ʈ�մϴ�.
		GS->AddScore(ScoreToAdd);
	}
}

void AMRGameMode::PlayerDied()
{
	UE_LOG(LogTemp, Log, TEXT("Player has died. Ending game."));
	EndGame();
}

void AMRGameMode::StartGame()
{
	if (AMRGameState* GS = GetGameState<AMRGameState>())
	{
		// 1. ���� ���¸� 'InProgress'�� �����϶�� GameState�� ����մϴ�.
		GS->SetCurrentState(EMRGameState::InProgress);

		// 2. ���̵� ��� Ÿ�̸Ӹ� �����մϴ�.
		GetWorld()->GetTimerManager().SetTimer(
			IncreaseDifficultyTimerHandle,
			this,
			&AMRGameMode::IncreaseDifficulty,
			TimeBetweenDifficultyIncrease,
			true // ����
		);
	}
}

void AMRGameMode::EndGame()
{
	if (AMRGameState* GS = GetGameState<AMRGameState>())
	{
		// 1. ���� ���¸� 'GameOver'�� �����϶�� GameState�� ����մϴ�.
		GS->SetCurrentState(EMRGameState::GameOver);

		// 2. �� �̻� �ʿ� ���� Ÿ�̸ӵ��� ������ŵ�ϴ�.
		GetWorld()->GetTimerManager().ClearTimer(IncreaseDifficultyTimerHandle);

		// TODO: ���� ���� UI�� ���ų�, �÷��̾� �Է��� ���� ���� ������ ó���մϴ�.
	}
}

void AMRGameMode::IncreaseDifficulty()
{
	if (AMRGameState* GS = GetGameState<AMRGameState>())
	{
		// 1. ���� �ӵ� ������ GameState���� �о�ɴϴ�.
		const float CurrentMultiplier = GS->GetGameSpeedMultiplier();

		// 2. ���ο� �ӵ� ������ ����մϴ�.
		const float NewMultiplier = CurrentMultiplier + SpeedIncreaseAmount;

		// 3. GameState�� ���ο� �ӵ� ������ �����϶�� ����մϴ�.
		GS->SetGameSpeedMultiplier(NewMultiplier);

		UE_LOG(LogTemp, Log, TEXT("Difficulty Increased! New speed multiplier: %f"), NewMultiplier);
	}
}