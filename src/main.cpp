#include "App.h"
#include <Cool/App/AppManager.h>

using namespace Cool;

int main() {
	Log::Initialize();
	AppManager appManager("CPU-GA");
	App app;
	return appManager.run(app);
}