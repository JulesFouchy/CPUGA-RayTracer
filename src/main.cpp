
#include "Cool/Log/Log.h"
#include <iostream>

using namespace Cool;

int main() {
	Log::Initialize();
	Log::Info("Helo !");
	std::cin.get();
}