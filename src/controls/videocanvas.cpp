#include "videocanvas.h"

#include <av3d.h>
#include "../render/sphericalprojection.h"
#include <QFileDialog> 
#include "../controls/msgbox.h"
#include <av_toolbox/platform.h>
#include <av_toolbox/scaler.h>
#ifndef _WINDOWS
#include <av_toolbox/frame_util.h>
#endif 

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/frame.h> 
#include <libswscale/swscale.h>
}

namespace player {
	VideoCanvas::VideoCanvas(QWidget *parent, Qt::WindowFlags f)
		: AbstractCanvas(parent, f),  
		mPlayer(nullptr), 
		mUpdateSliderTimer(nullptr),
		mVideoDuration(0.0f),
		mPlayerStatus(ins::Player::kStatusUnKnown),
		mCurrentPosition(0.0f) 
	{
		mVideoCanvasToolBar.setupUi(&mToolBarHolder); 
		createCanvas(); 
		connect(mVideoCanvasToolBar.playBtn, &QAbstractButton::clicked, this, &VideoCanvas::onPlayClicked);
		connect(mVideoCanvasToolBar.fullscreenBtn, &QAbstractButton::clicked, this, &VideoCanvas::onFullScreenClicked);
		connect(mVideoCanvasToolBar.voiceBtn, &QAbstractButton::clicked, this, &VideoCanvas::onVolumeClicked);
		connect(mVideoCanvasToolBar.previewWindowBtn, &QAbstractButton::clicked, this, &VideoCanvas::onPreviewClicked);
		connect(mVideoCanvasToolBar.projectionBtn, &QAbstractButton::clicked, this, &VideoCanvas::onProjectionClicked);
		connect(mVideoCanvasToolBar.snapshotBtn, &QAbstractButton::clicked, this, &VideoCanvas::onScreenShotClicked);
		connect(mVideoCanvasToolBar.editBtn, &QAbstractButton::clicked, this, &VideoCanvas::onEditWithStudioClicked);
		connect(mVideoCanvasToolBar.deleteBtn, &QAbstractButton::clicked, this, &VideoCanvas::onDeleteClicked);
		connect(mVideoCanvasToolBar.moreBtn, &QAbstractButton::clicked, this, &VideoCanvas::onMoreSettingsClicked);
	}

	VideoCanvas::~VideoCanvas() {
		stopRendering();
	}

	void VideoCanvas::startRendering(const QString& filePath) {
		if (!mIsContextPrepared) {
			prepareRenderingContext(true);
		}
		createPlayer(filePath, { {"enable_hwaccel", "false"},{"no_audio", "true"} });
		if (mUpdateSliderTimer) {
			delete mUpdateSliderTimer;
		}
		mUpdateSliderTimer = new QTimer(this);
		mUpdateSliderTimer->setSingleShot(false);
		mUpdateSliderTimer->setInterval(1000.0f);
		mUpdateSliderTimer->start();
		connect(mUpdateSliderTimer, &QTimer::timeout, this, &VideoCanvas::onUpdateSliderTimeout);
	}

	void VideoCanvas::paintGL() {
		ins::sp<AVFrame> currentFrame;  
		if (mFrameBuffer.try_pop(currentFrame) && currentFrame != nullptr) {
			mCurrentScene->resetSourceSize(currentFrame->width, currentFrame->height);
			mCurrentScene->setData(QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::UInt8, currentFrame->data[0], currentFrame->data[1], currentFrame->data[2]);
		}

		AbstractCanvas::paintGL();
	}

	void VideoCanvas::stopRendering() {
		if (mPlayer) {
			mPlayer->ReleaseSync();
		}
	}  

	// set control status
	void VideoCanvas::prepareToPlay() {
		mVideoDuration = mPlayer->GetDuration() / 1000.0f;
		mDurationTime = QTime(mVideoDuration / 3600, mVideoDuration / 60, (int)mVideoDuration % 60);
		mCurrentPosition = mPlayer->CurrentPosition() / 1000.0f;
		mCurrentVolume = mPlayer->Volume();
		mVideoCanvasToolBar.progressTime->setText(QString("00:00:00/00:00:00"));
		mVideoCanvasToolBar.playBtn->setIcon(QIcon(":/player/pause"));
		mVideoCanvasToolBar.progressSlider->setTickInterval(1);
		mVideoCanvasToolBar.progressSlider->setValue(0);
		mVideoCanvasToolBar.progressSlider->setRange(0, mVideoDuration);
	} 

	void VideoCanvas::onUpdateSliderTimeout() {
		if (mPlayerStatus == ins::Player::kStatusPlaying) {
			mCurrentPosition = mPlayer->CurrentPosition()/1000.0f;
		}  
		mVideoCanvasToolBar.progressSlider->setValue(mCurrentPosition);
		QTime currentTime(mCurrentPosition / 3600, mCurrentPosition / 60, (int)mCurrentPosition % 60);
		mVideoCanvasToolBar.progressTime->setText(currentTime.toString("hh:mm:ss").append("/").append(mDurationTime.toString("hh:mm:ss")));
	}

	void VideoCanvas::createPlayer(const QString& filePath, 
		const std::vector<std::pair<std::string, std::string>>& options) {
		mPlayer = std::make_shared<ins::Player>(filePath.toLocal8Bit().constData());
		for (auto& opt: options) {
			mPlayer->SetOption(opt.first, opt.second);
		}
		mPlayer->SetStatusNotify([this](ins::Player::NotifyType type, int val, int extra) {
			onPlayerStatusChanged(type, val, extra);
		});
		mPlayer->SetVideoRenderer([this](const ins::sp<AVFrame>& frame) {
			if (frame->format == AV_PIX_FMT_VIDEOTOOLBOX) {
#ifndef _WINDOWS 
				auto pixel_buffer = reinterpret_cast<CVImageBufferRef>(frame->data[3]);
				auto cpu_frame = CopyCVPixelBufferToFrame(pixel_buffer);
				mFrameBuffer.push(cpu_frame);
#endif 
			}
			else {   
				mFrameBuffer.push(frame);
			}
		});
		mPlayer->PrepareAsync();
	}

	void VideoCanvas::onPlayerStatusChanged(ins::Player::NotifyType type, int val, int extra) {
		if (type == ins::Player::kNotifyBufferingProgress) {
			qInfo() << "buffering progress -> " << val;
		}

		if (type == ins::Player::kNotifyStatus) {
			auto status = static_cast<ins::Player::PlayerStatus>(val);
			switch (status) {
			case ins::Player::kStatusUnKnown: {
				qInfo() << "status -> unknown";
				mPlayerStatus = ins::Player::kStatusUnKnown;
				break;
			}

			case ins::Player::kStatusPrepared: {
				qInfo() << "status -> prepared";
				prepareToPlay();
				mPlayerStatus = ins::Player::kStatusPrepared; 
				mPlayer->PlayAsync();
				break;
			}

			case ins::Player::kStatusPlaying: {
				qInfo() << "status -> playing";
				mPlayerStatus = ins::Player::kStatusPlaying;
				break;
			}

			case ins::Player::kStatusBuffering: {
				qInfo() << "status -> buffering";
				mPlayerStatus = ins::Player::kStatusBuffering;
				break;
			}

			case ins::Player::kStatusPaused: {
				mPlayerStatus = ins::Player::kStatusPaused; 
				break;
			}

			case ins::Player::kStatusEnd: {
				mPlayerStatus = ins::Player::kStatusEnd;
				mVideoCanvasToolBar.playBtn->setIcon(QIcon(":/player/stop"));
				break;
			}

			case ins::Player::kStatusFailed: {
				mPlayerStatus = ins::Player::kStatusFailed;
				qInfo() << "status -> failed";
				break;
			}

			default: {
				break;
			}
			}
		}
	}

	void VideoCanvas::onPlayClicked(bool check) { 
		if (mPlayerStatus == ins::Player::kStatusPlaying) {
			mVideoCanvasToolBar.playBtn->setIcon(QIcon(":/player/pause"));
			if (mPlayer) {
				mPlayer->PauseAsync();
			}
		}
		else if (mPlayerStatus == ins::Player::kStatusPaused || mPlayerStatus == ins::Player::kStatusEnd) {
			mVideoCanvasToolBar.playBtn->setIcon(QIcon(":/player/play"));
			if (mPlayer) {
				mPlayer->PlayAsync();
			}
		}
	}

	void VideoCanvas::onFullScreenClicked(bool check) {
		if (mFullScreen) {
			mVideoCanvasToolBar.fullscreenBtn->setIcon(QIcon(":/player/fullscreen"));
			mFullScreen = false;
		}
		else {
			mVideoCanvasToolBar.fullscreenBtn->setIcon(QIcon(":/player/fullscreen_exit"));
			mFullScreen = true;
		}
		AbstractCanvas::onFullScreenClicked(check);
	}

	void VideoCanvas::onVolumeClicked(bool check) {

	}

	void VideoCanvas::onPreviewClicked(bool check) {
		AbstractCanvas::onPreviewClicked(check);
	}

	void VideoCanvas::onProjectionClicked(bool check) {

	}

	void VideoCanvas::onScreenShotClicked(bool check) {
		QString savePath = QFileDialog::getSaveFileName(nullptr, tr("Save screenshot to directory"), 
			"File.txt", tr("All (*.*)"));
	}

	void VideoCanvas::onEditWithStudioClicked(bool check) {

	}

	void VideoCanvas::onDeleteClicked(bool check) {

	}

	void VideoCanvas::onMoreSettingsClicked(bool check) {

	} 
}
