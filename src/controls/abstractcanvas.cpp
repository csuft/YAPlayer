#include "abstractcanvas.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <av3d.h>
#include "../render/sphericalprojection.h"
#include "../render/windowrenderer.h"

namespace player {
	AbstractCanvas::AbstractCanvas(QWidget *parent, Qt::WindowFlags f)
		: QOpenGLWidget(parent, f),
		mRenderer(nullptr),
		mRenderable(nullptr),
		mCurrentScene(nullptr),
		mCurrentCamera(nullptr),
		mIsContextPrepared(false),
		mIsMousePressed(false),
		mFullScreen(false),
		mPreviewWindowVisible(false),
		mRenderTimer(new QTimer),
		mSurface(nullptr)
	{  
		mRenderTimer->setInterval(1000.0 / 60.0f);
		mRenderTimer->setSingleShot(false);
		mRenderTimer->start();
		connect(mRenderTimer, &QTimer::timeout, this, &AbstractCanvas::onRenderTimeout);
	}

	AbstractCanvas::~AbstractCanvas() {
		releaseResources();
	}

	void AbstractCanvas::mouseMoveEvent(QMouseEvent *event) {
		if (mIsMousePressed) {
			event->accept();
			QPointF pos = event->localPos();
			float diffX = pos.x() - mLastPos.x();
			float diffY = pos.y() - mLastPos.y();
			float mouseFactor = 240.0f / width();
			diffX *= mouseFactor;
			diffY *= mouseFactor;

			mLastPos = pos;

			av3d::PanoramaViewer* viewer = dynamic_cast<av3d::PanoramaViewer*>(mProjection);
			if (viewer) {
				viewer->rotateBy(diffX, -diffY);
			}
		} 
	}

	void AbstractCanvas::mousePressEvent(QMouseEvent *event) {
		event->accept();
		mIsMousePressed = true;
		mLastPos = event->localPos();
	}

	void AbstractCanvas::mouseReleaseEvent(QMouseEvent *event) {
		event->accept();
		mIsMousePressed = false;
	}

	void AbstractCanvas::wheelEvent(QWheelEvent* event) {
		event->accept();
		static const float MOUSE_FACTOR = 60.0f / 120;
		av3d::PanoramaViewer* viewer = dynamic_cast<av3d::PanoramaViewer*>(mProjection); 
		if (viewer) {
			viewer->zoomBy(event->angleDelta().y() * MOUSE_FACTOR);
		} 
	}

	void AbstractCanvas::paintGL() {
		if (!mRenderer) {
			qInfo() << "Not initialized";
			return;
		}
		if (mProjection && mProjection->needsUpdate()) {
			mProjection->update();
		}
		mRenderer->render();
	}

	void AbstractCanvas::resizeGL(int w, int h) {
		if (mRenderer) {
			mRenderer->setViewport(0, 0, w, h);
		}
		if (mRenderable) {
			mRenderable->setViewport(mRenderer->viewport());
		}
	}

	void AbstractCanvas::prepareRenderingContext(bool useGPUScale) {
		mSurface = context()->surface();
		mRenderer = new WindowRenderer(mSurface, context());
		mRenderer->create();
		mRenderable = new av3d::Renderable();
		mRenderer->addRenderable(mRenderable);

		mCurrentScene = new av3d::SphereScene(1024, 512);
		mCurrentScene->create(useGPUScale);
		mRenderable->setScene(mCurrentScene);
		mRenderer->setViewport(0, 0, this->width(), this->height());
		mRenderable->setViewport(mRenderer->viewport());

		mCurrentCamera = new av3d::PerspectiveCamera();
		mRenderable->setCamera(mCurrentCamera);

		mProjection = new av3d::SphericalProjection(mCurrentCamera);
		mProjection->init();
		mIsContextPrepared = true;
	}

	void AbstractCanvas::onRenderTimeout() {
		update();
	}

	void AbstractCanvas::releaseResources() {
		if (mRenderer) {
			delete mRenderer;
			mRenderer = nullptr;
		}
		if (mRenderable) {
			delete mRenderable;
			mRenderable = nullptr;
		}
	}

	void AbstractCanvas::createCanvas() {
		mPreviousIndicator.setIcon(QIcon(":/player/previous_indictor"));
		mPreviousIndicator.setIconSize(QSize(56, 56));
		mPreviousIndicator.setObjectName("PreviousIndicator");
		mPreviousIndicator.setCursor(Qt::PointingHandCursor);
		mPreviousIndicator.setToolTip(tr("Previous"));
		mNextIndicator.setIcon(QIcon(":/player/next_indicator"));
		mNextIndicator.setIconSize(QSize(56, 56));
		mNextIndicator.setObjectName("NextIndicator");
		mNextIndicator.setCursor(Qt::PointingHandCursor);
		mNextIndicator.setToolTip(tr("Next"));

		leftButtonLayout.addStretch(1);
		leftButtonLayout.addWidget(&mPreviousIndicator, 0);
		leftButtonLayout.addStretch(1);

		rightButtonLayout.addStretch(1);
		rightButtonLayout.addWidget(&mNextIndicator, 0);
		rightButtonLayout.addStretch(1);

		buttonsLayout.addLayout(&leftButtonLayout);
		buttonsLayout.addStretch(1);
		buttonsLayout.addLayout(&rightButtonLayout);

		mainLayout.addLayout(&buttonsLayout);
		mainLayout.addWidget(&mToolBarHolder);
		mainLayout.setContentsMargins(0, 0, 0, 0);

		setLayout(&mainLayout);
	}

	void AbstractCanvas::onFullScreenClicked(bool check) {
		emit fullscreenClicked();
	}

	void AbstractCanvas::onPreviewClicked(bool check) {
		emit previewWindowClicked(mPreviewWindowVisible);
	}

	void AbstractCanvas::onProjectionClicked(bool check) {

	}

	void AbstractCanvas::onScreenShotClicked(bool check) {

	}

	void AbstractCanvas::onEditWithStudioClicked(bool check) {

	}

	void AbstractCanvas::onDeleteClicked(bool check) {
		emit deleteFileClicked();
	}

	void AbstractCanvas::onMoreSettingsClicked(bool check) {

	}
}
