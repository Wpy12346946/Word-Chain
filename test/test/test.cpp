#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/core.lib")
#else
#pragma comment(lib, "../x64/Release/core.lib")
#endif
extern "C" __declspec(dllimport) int testy();
extern "C" __declspec(dllimport) int gen_chain_word(char* words[], int len, char* result[], char head, char tail, char reject, bool enable_loop);

extern "C" __declspec(dllimport) int gen_chain_char(char* words[], int len, char* result[], char head, char tail, char reject, bool enable_loop);

extern "C" __declspec(dllimport) int gen_chains_all(char* words[], int len, char* result[]);
namespace test
{
	TEST_CLASS(test)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Assert::AreEqual(1, testy());
		}
	};
}
