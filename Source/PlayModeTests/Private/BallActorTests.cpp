#include "CQTest.h"
#include "ObjectBuilder.h"
#include "Components/MapTestSpawner.h"

TEST_CLASS(FCGBallTests, "Project.ActorTests.Ball")
{
	TUniquePtr<FMapTestSpawner> Spawner;
protected:
	BEFORE_EACH()
	{
		Spawner = MakeUnique<FMapTestSpawner>("/Game/FirstPerson/Maps", "FirstPersonMap");
		Spawner->AddWaitUntilLoadedCommand(TestRunner);
	}
	
	TEST_METHOD(WhenPlayerTouchesSpikeTrap_ThenPlayerLifted)
	{
		TestCommandBuilder
		    .StartWhen([this](){return nullptr != Spawner->FindFirstPlayerPawn(); }, FTimespan::FromSeconds(30))
		    .Do([this]{
				
			});
	}
};