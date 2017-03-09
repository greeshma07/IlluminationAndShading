
#ifndef IM_project_4_callbackFunctions_h
#define IM_project_4_callbackFunctions_h


#endif

void onKeyboard(unsigned char key, int x, int y)
{
	if(key=='m' || key== 'M')
		key='m';
	switch (key)
	{
	case 27:
		exit(1);

		break;
	case 'm':	if(meshView)
						meshView=false;
					else
						meshView=true;
					glutPostRedisplay();
	case '0':
		shadingType = "none";
		glutPostRedisplay();
		break;
	case '1':
		shadingType = "flat";
		glutPostRedisplay();
		break;
	case '2':
		shadingType = "flatPhong";
		glutPostRedisplay();
		break;

	default:
		break;
	}
}
