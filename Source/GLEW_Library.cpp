#include <GL/glew.h>
#include <GLEW_Library.h>
#include <Logging.h>

namespace Rubiks_Cube_Game
{
GLEW_Library::GLEW_Library()
{
	GLenum error_code = glewInit();
	if (error_code != GLEW_OK)
	{
		LOG("GLEW_ERROR:\t\tGLEW library not initialized properly\n");
	}
	else
	{
		LOG("GLEW_MSG:\t\tGLEW library initialized properly\n");
		if (GLEW_VERSION_3_3)
		{
			LOG("GLEW_MSG:\t\tOPENGL 3.3 is available\n");
		}
	}
}

GLEW_Library::~GLEW_Library()
{

}
}