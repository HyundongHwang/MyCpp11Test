// MyCpp11Test.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"


int main(int argc, wchar_t* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	auto res = RUN_ALL_TESTS();
	auto tmp = 0;
	std::cin >> tmp;
    return res;
}

