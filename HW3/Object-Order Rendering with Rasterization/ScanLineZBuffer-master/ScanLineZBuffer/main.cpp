#include <iostream>
#include "Vec.h"
#include "utils.h"
#include "Model.h"
#include "Shader.h"
#include "ScanLineZBuffer.h"
#include "Display.h"
#include <ctime>


using namespace std;

int main()
{
	
	Model model("../models/al.obj");
	// we define the size of the window here
	int width = 800, height = 600;
	
	// we use the z bufer to calculate the face id corresponding to the
	// screen coordinates that need to be drawn
	ScanLineZBuffer slzBuffer;
	slzBuffer.setSize(width, height);
	slzBuffer.run(model);

	Display display(&model, &slzBuffer);
	display.run();
	
	//system("pause");
	return 0;
}