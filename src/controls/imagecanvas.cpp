#include "imagecanvas.h"
#include <render/spherescene.h>
#include "../render/sphericalprojection.h"
#include <QToolButton>

namespace player {
	ImageCanvas::ImageCanvas(QWidget *parent, Qt::WindowFlags f)
		: AbstractCanvas(parent, f){
		mImagecanvasToolBar.setupUi(&mToolBarHolder); 
		createCanvas();
		connect(mImagecanvasToolBar.fullscreenBtn, &QAbstractButton::clicked, this, &ImageCanvas::onFullScreenClicked);
		connect(mImagecanvasToolBar.previewWindowBtn, &QAbstractButton::clicked, this, &ImageCanvas::onPreviewClicked);
		connect(mImagecanvasToolBar.projectionModeBtn, &QAbstractButton::clicked, this, &ImageCanvas::onProjectionClicked);
		connect(mImagecanvasToolBar.snapshotBtn, &QAbstractButton::clicked, this, &ImageCanvas::onScreenShotClicked);
		connect(mImagecanvasToolBar.deleteFileBtn, &QAbstractButton::clicked, this, &ImageCanvas::onDeleteClicked);
		connect(mImagecanvasToolBar.MoreBtn, &QAbstractButton::clicked, this, &ImageCanvas::onMoreSettingsClicked);
	}

	ImageCanvas::~ImageCanvas() {

	}

	void ImageCanvas::startRendering(const QString& filePath) {
		if (!mIsContextPrepared) {
			prepareRenderingContext(false);
		} 

		QImage image(filePath);  
		mCurrentScene->resetSourceSize(image.width(), image.height());
		mCurrentScene->setData(QOpenGLTexture::BGRA, QOpenGLTexture::UInt8, image.bits());
	}

	void ImageCanvas::stopRendering() {

	}  

	void ImageCanvas::onFullScreenClicked(bool check) {
		if (mFullScreen) {
			mImagecanvasToolBar.fullscreenBtn->setIcon(QIcon(":/player/fullscreen"));
			mFullScreen = false;
		}
		else {
			mImagecanvasToolBar.fullscreenBtn->setIcon(QIcon(":/player/fullscreen_exit"));
			mFullScreen = true;
		}
		AbstractCanvas::onFullScreenClicked(check);
	}

	void ImageCanvas::onPreviewClicked(bool check) {
		if (mPreviewWindowVisible) {
			mImagecanvasToolBar.previewWindowBtn->setIcon(QIcon(":/player/preview_window_disabled"));
			mPreviewWindowVisible = false;
		}
		else {
			mImagecanvasToolBar.previewWindowBtn->setIcon(QIcon(":/player/preview_window_enabled"));
			mPreviewWindowVisible = true;
		}
		AbstractCanvas::onPreviewClicked(check);
	} 

	void ImageCanvas::onDeleteClicked(bool check) {
		// todo
		AbstractCanvas::onDeleteClicked(check);
	} 
}
