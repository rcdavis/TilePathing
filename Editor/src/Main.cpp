
#include "Utils/Log.h"
#include "Application.h"

int main() {
	Log::Init("Editor");

	LOG_INFO("Starting Tile Pathing...");

	Application::Get().Run();

	LOG_INFO("Closing Tile Pathing...");

	return 0;
}
