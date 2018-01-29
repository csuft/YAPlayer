#ifndef VIDEO_CANVAS_H
#define VIDEO_CANVAS_H
 
#include "abstractcanvas.h" 
#include "ui_videocanvastool.h"
#include <av_toolbox/sp.h>
#include <av_toolbox/threadsafe_queue.h>
#include <player/player.h>
#include <av_toolbox/scaler.h>
#include <QPointer>
#include <QTime>

struct AVFrame;
class QTimer;

namespace player {
	class VideoCanvas : public AbstractCanvas {
		Q_OBJECT

	public:
		explicit VideoCanvas(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
		~VideoCanvas();
		virtual void startRendering(const QString& filePath) override;
		virtual void stopRendering() override;

	protected:  
		virtual void paintGL() override; 

	protected slots:
		void onPlayClicked(bool check);
		void onVolumeClicked(bool check);
		void onPlayerStatusChanged(ins::Player::NotifyType type, int val, int extra);
		void onUpdateSliderTimeout();

		virtual void onFullScreenClicked(bool check);
		virtual void onPreviewClicked(bool check);
		virtual void onProjectionClicked(bool check);
		virtual void onScreenShotClicked(bool check);
		virtual void onEditWithStudioClicked(bool check);
		virtual void onDeleteClicked(bool check);
		virtual void onMoreSettingsClicked(bool check);

	private:
		void createPlayer(const QString& filePath, const std::vector<std::pair<std::string, std::string>>& options);
		void prepareToPlay();

	private:
		Ui::videoCanvasTool mVideoCanvasToolBar;  
		QTimer* mUpdateSliderTimer;
		// player related
		ins::ThreadSafeQueue<ins::sp<AVFrame>> mFrameBuffer;
		ins::sp<ins::Player> mPlayer;
		ins::Player::PlayerStatus mPlayerStatus;

		// seconds
		float mVideoDuration;
		QTime mDurationTime;
		float mCurrentPosition;
		float mCurrentVolume;
	};
}


#endif // VIDEO_CANVAS_H
