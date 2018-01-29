#include "windowrenderer.h"

namespace player {
	WindowRenderer::WindowRenderer(QSurface* winSurface, QOpenGLContext* winContext) :
		av3d::Renderer(winSurface),
		mWindowContext(winContext)
	{

	}

	void WindowRenderer::initializeOpenGL()
	{
		mContext = mWindowContext;
		av3d::Renderer::initializeOpenGL();
	}

	void WindowRenderer::setupOpenGLEnv()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		glCullFace(GL_FRONT);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	}

	void WindowRenderer::render()
	{
		setupOpenGLEnv();
		av3d::Renderer::render();
	}
}

