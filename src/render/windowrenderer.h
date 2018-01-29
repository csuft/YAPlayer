#ifndef WINDOWRENDERER_H
#define WINDOWRENDERER_H

#include <render/renderer.h> 
#include "../controls/abstractcanvas.h"

namespace player {
	class WindowRenderer : public av3d::Renderer
	{
	public:
		explicit WindowRenderer(QSurface* winSurface, QOpenGLContext* winContext);
		void initializeOpenGL() override;
		void render() override;

	private:
		QOpenGLContext* mWindowContext;
		void setupOpenGLEnv();
	};
}


#endif // QUICKWINDOWRENDERER_H
