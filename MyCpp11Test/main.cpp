// MyCpp11Test.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"


int main(int argc, wchar_t* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    auto res = RUN_ALL_TESTS();
    return res;
}



// devenv.com C:\project\MyCpp11Test\MyCpp11Test.sln /Build "Debug|x86"; C:\project\MyCpp11Test\bin\x86\Debug\MyCpp11Test.exe;
// git add *; git commit -m modify; git fetch --all; git rebase origin/master; git push origin master;