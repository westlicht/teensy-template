#include "App.h"

extern "C" int main(void)
{
	auto &app = App::instance();
	app.init();
	app.run();
}
