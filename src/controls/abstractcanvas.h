#ifndef ABSTRACT_CANVAS_H
#define ABSTRACT_CANVAS_H

#include <QOpenGLWidget>
#include <QMouseEvent> 
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer> 

#include "../render/projectionbase.h" 
#include <control/lookatcontroller.h>
#include <render/renderer.h>
#include <memory>

namespace player {
	class AbstractCanvas : public QOpenGLWidget
	{
		Q_OBJECT
	public:
		explicit AbstractCanvas(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
		virtual ~AbstractCanvas();
		virtual void startRendering(const QString& filePath) = 0;
		virtual void stopRendering() = 0;
		av3d::Renderable* renderable() {
			return mRenderable;
		}

		av3d::ProjectionBase* projection() {
			return mProjection;
		}

	signals:
		void fullscreenClicked();
		void previewWindowClicked(bool show);
		void deleteFileClicked();

	protected slots:
		virtual void onFullScreenClicked(bool check);
		virtual void onPreviewClicked(bool check);
		virtual void onProjectionClicked(bool check);
		virtual void onScreenShotClicked(bool check);
		virtual void onEditWithStudioClicked(bool check);
		virtual void onDeleteClicked(bool check);
		virtual void onMoreSettingsClicked(bool check);

	protected:
		virtual void mouseMoveEvent(QMouseEvent *event) override;
		virtual void mousePressEvent(QMouseEvent *event) override;
		virtual void mouseReleaseEvent(QMouseEvent *event) override;
		virtual void wheelEvent(QWheelEvent* event) override;
		
		virtual void paintGL() override;
		virtual void resizeGL(int w, int h) override;
		virtual void prepareRenderingContext(bool useGPUScale);
		virtual void createCanvas();
	private:
		AbstractCanvas(const AbstractCanvas& obj) = default;
		AbstractCanvas& operator=(const AbstractCanvas& obj) = default;
		void releaseResources();

	protected:
		av3d::Renderer* mRenderer;
		av3d::Renderable* mRenderable;
		av3d::Scene* mCurrentScene;
		av3d::Camera* mCurrentCamera;
		QTimer* mRenderTimer;
		av3d::ProjectionBase* mProjection;
		bool mIsMousePressed;
		bool mFullScreen;
		bool mPreviewWindowVisible;
		QPointF mLastPos;
		bool mIsContextPrepared;
		QSurface* mSurface;

		// Common controls for canvas
		QVBoxLayout mainLayout;
		QWidget mToolBarHolder;
		QHBoxLayout buttonsLayout;
		QVBoxLayout leftButtonLayout;
		QVBoxLayout rightButtonLayout;
		QPushButton mPreviousIndicator;
		QPushButton mNextIndicator;

	protected slots:
		void onRenderTimeout();
	};
}


#endif // ABSTRACT_CANVAS_H
