#include "CQTest.h"

TEST_CLASS(FExampleUnitTests, "Project.UnitTests")
{
protected:
	BEFORE_EACH()
	{
		
	}

	TEST_METHOD(Add_ZeroVectors_Zero)
	{
		const auto res = FVector::Zero() + FVector::Zero();
		ASSERT_THAT(AreEqual(FVector::Zero(), res));
	}
};