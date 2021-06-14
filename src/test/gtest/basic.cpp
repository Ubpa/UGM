#include <gtest/gtest.h>

#include <UGM/UGM.hpp>

using namespace Ubpa;

TEST(vec, hadamard_product) {
	vecf4 v{ 1,2,3,4 };
	vecf4 w{ 4,3,2,1 };
	{
		vecf4 vw{ 1.f * 4.f,2.f * 3.f,3.f * 2.f,4.f * 1.f };
		EXPECT_EQ(v.hadamard_product(w), vw);
	}
}

TEST(mat, hadamard_product) {
	matf2 v{
		1,2,
		3,4
	};
	matf2 w{
		4,3,
		2,1
	};
	{
		matf2 vw{
			1.f * 4.f, 2.f * 3.f,
			3.f * 2.f, 4.f * 1.f
		};
		EXPECT_EQ(v.hadamard_product(w), vw);
	}
}