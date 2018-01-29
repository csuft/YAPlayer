#ifndef INSTA360STUDIO_H
#define INSTA360STUDIO_H

#include <QtWidgets/QMainWindow>
#include <QMenu>
#include <QDir>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPointer>

#include "ui_insta360studio.h"
#include "ui_placeholder.h" 
#include "../controls/imagecanvas.h"
#include "../controls/videocanvas.h"

namespace player {
	class Insta360Studio : public QMainWindow
	{
		Q_OBJECT

	public:
		Insta360Studio(QWidget *parent = Q_NULLPTR);
		~Insta360Studio();

		enum class CurrentFileType {
			VIDEO,
			IMAGE,
			UNINITIALIZED
		};

	signals:

	private slots :
		void onOpenFileTriggered(bool checked);
		void onOpenStreamTriggered(bool checked);
		void onQuitProgramTriggered(bool checked);
		void onTutorialTriggered(bool checked);
		void onCheckUpdatesTriggered(bool checked);
		void onAboutTriggered(bool checked);
		void onSettingsTriggered();
		void onLanguageTriggered();

		// common operations for image and video tool buttons
		void onFullscreenClicked();
		void onPreviewWindowClicked(bool show);
		void onDeleteFileClicked();

	private:
		void updateWindowTitle();

	private:
		Ui::Insta360StudioClass ui;
		Ui::placeHolder placeHolder;
		QPointer<ImageCanvas> mImageCanvas;
		QPointer<VideoCanvas> mVideoCanvas;

		// Menu UI
		QWidget mPlaceHolderWidget;
		QLabel mProductName;
		QAction mLanguageAction;
		QAction mSettingsAction;
		QMenu mHelpMenu;
		QAction mCheckUpdatesAction;
		QAction mTutorialAction;
		QAction mAboutPlayer;

		QDir mCurrentDir;
		QString mCurrentFile;
		CurrentFileType mOpenedType;
	};
}


#endif // INSTA360STUDIO_H
