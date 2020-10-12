#include "Display.h"

using namespace std;

ScanLineZBuffer* Display::engine = NULL;
Model* Display::model = NULL;
Point3f Display::point_1st = Point3f(0.0, 0.0, 0.0);
Point3f Display::point_2rd = Point3f(0.0, 0.0, 0.0);
float** Display::RotateMat = NULL;

int Display::idx1 = -1, Display::idx2 = -1;
string path = "../models";
vector<string> fileNames;

//void Display::setSize(int w, int h)
//{
//	width = w;
//	height = h;
//}

Display::Display(Model* model, ScanLineZBuffer* slzBuffer)
{
	this->model = model;
	engine = slzBuffer;
	RotateMat = new float*[3];
	for (int i = 0; i < 3; ++i)
	{
		RotateMat[i] = new float[3];
		for (int j = 0; j < 3; ++j)
		{
			RotateMat[i][j] = 0.0f;
		}
	}
}

Display::~Display()
{
	for (int i = 0; i < 3; ++i)
	{
		delete[] RotateMat[i];
	}
	delete[] RotateMat;
	RotateMat = NULL;
}

void Display::loop()
{
	int width = 0, height = 0;
	engine->getSize(width, height);

	glClear(GL_COLOR_BUFFER_BIT); // clear buffer to preset values.  Indicates the buffers currently enabled for color writing.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // replace the current matrix with the identity matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	
	// GL_MODELVIEW applies subsequent matrix operations to the modelview matrix stack
	// GL_PROJECTION applies subsequent maxtrix to the projection matrix stack
	// GL_COLOR applies subsequent maxtrix to the color matrix stack

	engine->run(*model);

	glBegin(GL_POINTS);

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			
			Color3f rgb(0.0f, 0.0f, 0.0f);
			int tmp = engine->idBuffer[y][x];
			if (tmp >= 0)
			{
				rgb = model->faces[tmp].color;
				//rgb = Color3f(1.0, 1.0, 1.0);
			}
			//std::cout << "RGB:" << rgb.r << " " << rgb.g << " " << rgb.b << std::endl;
			glColor3f(rgb.r, rgb.g, rgb.b);
			glVertex2i(x, y);
		}
	}
	glEnd();

	glFinish();
}

void Display::reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	engine->setSize(w, h);
	engine->run(*model);
}

void Display::run()
{
	int width = 0, height = 0;
	engine->getSize(width, height);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // select either a single buffered window or a rgb model
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 80);
	glutCreateWindow("KAUST_BEACON");
	glutDisplayFunc(loop);
	glutReshapeFunc(reshape);
	glutMouseFunc(drag_mouse);
	glutSpecialFunc(keyboard);

	//
	fileNames = getListFiles(path, "*.obj");
	glutCreateMenu(screen_menu);
	for (int i = 0; i < fileNames.size(); ++i)
	{
		glutAddMenuEntry(fileNames[i].c_str(), i);
	}
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}

void Display::drag_mouse(int button, int state, int x, int y)
{
	int width = 0, height = 0;
	engine->getSize(width, height);

	if (button == GLUT_LEFT_BUTTON)
	{
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;// the position of the window
		if (state == GLUT_DOWN)
		{
			x1 = x;
			y1 = height - 1 - y;// convert the window coordinates to graphic coordinates 
			idx1 = engine->idBuffer[y1][x1];
			if (idx1 >= 0)// calculate the average coordinates of the corresponding bin
			{
				vector<int> vertexIdx = model->faces[idx1].vertexIdx;
				int vertex_num = vertexIdx.size();
				for (int i = 0; i < vertex_num; ++i)
				{
					point_1st += model->vertexes[vertexIdx[i]].point;
				}
				point_1st /= vertex_num;
				
			}
			//cout << "current coordinate position(" << x1 << "," << y1 << ")" << endl;
			return;
		}
		if (state == GLUT_UP)
		{
			x2 = x;
			y2 = height - 1 - y;
			idx2 = engine->idBuffer[y2][x2];
			if (idx2 >= 0)// calculate the corresponding coordinates
			{
				vector<int> vertexIdx = model->faces[idx2].vertexIdx;
				int vertex_num = vertexIdx.size();
				for (int i = 0; i < vertex_num; ++i)
				{
					point_2rd += model->vertexes[vertexIdx[i]].point;
				}
				point_2rd /= vertex_num;

			}
			//cout << " the current position(" << x2 << "," << y2 << ")" << endl;
		}
	}

	if (idx1 >= 0 && idx2 >= 0 && idx1 != idx2)
	{
		//cout << " the first point" << point_1st.x << "," << point_1st.y << "," << point_1st.z << endl;
		//cout << " the second point" << point_2rd.x << "," << point_2rd.y << "," << point_2rd.z << endl;
		Point3f center_point(width / 2, height / 2, 0.0);
		Vec3f vec1 = normalize(point_1st - center_point),
			vec2 = normalize(point_2rd - center_point);

		Vec3f rotate_axi = cross(vec1, vec2);
		rotate_axi = normalize(rotate_axi);

		//cout << "rotate_axi:" << rotate_axi.x << "," << rotate_axi.y << "," << rotate_axi.z << endl;

		float cosine = dot(vec1, vec2);// the rotate angle cos
		//cout << "cosine:" << cosine << endl;
		float sine = -sqrt(1 - cosine*cosine);

		// refer to the link http://blog.csdn.net/qiuchangyong/article/details/5859628
		float n1 = rotate_axi.x,
			  n2 = rotate_axi.y,
			  n3 = rotate_axi.z;

		RotateMat[0][0] = n1*n1 + (1 - n1*n1)*cosine;
		RotateMat[0][1] = n1*n2*(1 - cosine) + n3*sine;
		RotateMat[0][2] = n1*n3*(1 - cosine) - n2*sine;

		RotateMat[1][0] = n1*n2*(1 - cosine) - n3*sine;
		RotateMat[1][1] = n2*n2 + (1 - n2*n2)*cosine;
		//RotateMat[1][1] = n2*n2*(1 - cosine) + cosine;
		RotateMat[1][2] = n2*n3*(1 - cosine) + n1*sine;

		RotateMat[2][0] = n1*n2*(1 - cosine) + n2*sine;
		RotateMat[2][1] = n2*n3*(1 - cosine) - n1*sine;
		RotateMat[2][2] = n3*n3 + (1 - n3*n3)*cosine;


		/*for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				cout << RotateMat[i][j] << " ";
			}
			cout << endl;
		}*/

		model->rotate(RotateMat,model->vertexes);

		engine->needUpdate = true;
		engine->run(*model);
		glutPostRedisplay();
		idx1 = -1;
		idx2 = -1;
	}


}

void Display::keyboard(int key, int x, int y)
{
	float theta = 0.0;
	const float PI = 3.1415926;
	float dtheta = PI / 12;
	float n1 = 1.0, n2 = 0.0, n3 = 0.0;
	
	//------modify-------
	  double  scale = 1.0;
	//------------------
	switch (key)
	{
	case GLUT_KEY_LEFT:
		theta = dtheta;
		n1 = 0.0;
		n2 = 1.0;
		n3 = 0.0;
		break;
	case GLUT_KEY_RIGHT:
		theta = -dtheta;
		n1 = 0.0;
		n2 = 1.0;
		n3 = 0.0;
		break;
	case GLUT_KEY_UP:
		/*theta = dtheta;
		n1 = 1.0;
		n2 = 0.0;
		n3 = 0.0;*/
		// ----modify-------
		scale = scale * 1.5;
		model->rescale(scale);
		engine->rescale(scale);
		// ------------------
		break;
	case GLUT_KEY_DOWN:
		theta = -dtheta;
		n1 = 1.0;
		n2 = 0.0;
		n3 = 0.0;
		break;
	case GLUT_KEY_PAGE_UP:
		theta = -dtheta;
		n1 = 0.0;
		n2 = 0.0;
		n3 = 1.0;
		break;
	case GLUT_KEY_PAGE_DOWN:
		theta = dtheta;
		n1 = 0.0;
		n2 = 0.0;
		n3 = 1.0;
		break;

	default:
		break;
	}

	float cosine = cos(theta),sine = sin(theta);

	RotateMat[0][0] = n1*n1 + (1 - n1*n1)*cosine;
	RotateMat[0][1] = n1*n2*(1 - cosine) + n3*sine;
	RotateMat[0][2] = n1*n3*(1 - cosine) - n2*sine;

	RotateMat[1][0] = n1*n2*(1 - cosine) - n3*sine;
	//RotateMat[1][1] = n2*n2*(1 - n2*n2)*cosine;
	RotateMat[1][1] = n2*n2*(1 - cosine) + cosine;
	RotateMat[1][2] = n2*n3*(1 - cosine) + n1*sine;

	RotateMat[2][0] = n1*n2*(1 - cosine) + n2*sine;
	RotateMat[2][1] = n2*n3*(1 - cosine) - n1*sine;
	RotateMat[2][2] = n3*n3 + (1 - n3*n3)*cosine;

	model->rotate(RotateMat, model->vertexes);

	engine->needUpdate = true;
	engine->run(*model);
	glutPostRedisplay();
}

void Display::screen_menu(int value)
{
	string filename = path + "/" + fileNames[value];
	*model = Model(filename);
	engine->needUpdate = true;
	engine->run(*model);
	glutPostRedisplay();
}
