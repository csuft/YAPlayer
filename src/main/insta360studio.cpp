#include "insta360studio.h"
#include "../controls/nofocusdelegate.h"
#include "../controls/msgbox.h" 
#include <QApplication>
#include <iostream>
#include <QFileDialog>
#include <QFileInfo>

namespace player {
	Insta360Studio::Insta360Studio(QWidget *parent)
		: QMainWindow(parent), mOpenedType(CurrentFileType::UNINITIALIZED),
		mImageCanvas(nullptr),
		mVideoCanvas(nullptr) 
	{
		ui.setupUi(this);
		setMinimumSize(980, 690);
		setWindowIcon(QIcon(":/player/logo"));
		setWindowTitle(QStringLiteral("Insta360 Player"));

		mProductName.setObjectName("product_label");
		mProductName.setPixmap(QPixmap(":/player/product_name_gray"));
		ui.menuBar->setCornerWidget(qobject_cast<QWidget*>(&mProductName), Qt::TopLeftCorner);

		placeHolder.setupUi(&mPlaceHolderWidget);
		QVBoxLayout* mainLayout = new QVBoxLayout(this);
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->addWidget(&mPlaceHolderWidget);
		centralWidget()->setLayout(mainLayout);

		mHelpMenu.setTitle(tr("&Help"));
		mCheckUpdatesAction.setText(tr("Check Updates"));
		mTutorialAction.setText(tr("Player Tutorial"));
		mAboutPlayer.setText(tr("About Player"));
		mHelpMenu.addAction(&mCheckUpdatesAction);
		mHelpMenu.addAction(&mTutorialAction);
		mHelpMenu.addAction(&mAboutPlayer);

		mLanguageAction.setText(tr("Language"));
		mSettingsAction.setText(tr("Settings"));

		ui.menuBar->addAction(&mLanguageAction);
		ui.menuBar->addAction(&mSettingsAction);
		ui.menuBar->addMenu(&mHelpMenu);
		connect(ui.action_Open_File, &QAction::triggered, this, &Insta360Studio::onOpenFileTriggered);
		connect(ui.action_Open_Live_Stream, &QAction::triggered, this, &Insta360Studio::onOpenStreamTriggered);
		connect(ui.action_Quit, &QAction::triggered, this, &Insta360Studio::onQuitProgramTriggered);
		connect(&mLanguageAction, &QAction::triggered, this, &Insta360Studio::onLanguageTriggered);
		connect(&mSettingsAction, &QAction::triggered, this, &Insta360Studio::onSettingsTriggered);
		connect(&mCheckUpdatesAction, &QAction::triggered, this, &Insta360Studio::onCheckUpdatesTriggered);
		connect(&mTutorialAction, &QAction::triggered, this, &Insta360Studio::onTutorialTriggered);
		connect(&mAboutPlayer, &QAction::triggered, this, &Insta360Studio::onAboutTriggered);
	}

	Insta360Studio::~Insta360Studio() {}

	void Insta360Studio::onOpenFileTriggered(bool checked) {
		QString filePath = QFileDialog::getOpenFileName(this, tr("Open panoramic contents"), "", tr("Panoramic Contents (*.jpg *.insp *.mp4 *.insv)"), Q_NULLPTR, QFileDialog::DontResolveSymlinks | QFileDialog::DontUseCustomDirectoryIcons);

		QFileInfo selectedFile(filePath);
		if (selectedFile.exists()) {
			mCurrentDir = selectedFile.absoluteDir();
			QString selectedFileName = selectedFile.fileName();
			if (!selectedFileName.compare(mCurrentFile)) {
				return;
			}
			mCurrentFile = selectedFileName;
			if (!selectedFile.suffix().compare("insp") || !selectedFile.suffix().compare("jpg")) {
				if (mImageCanvas == nullptr) {
					mImageCanvas = new ImageCanvas;
				}
				
				if (mOpenedType == CurrentFileType::UNINITIALIZED) {
					centralWidget()->layout()->replaceWidget(&mPlaceHolderWidget, mImageCanvas);
				}
				else if (mOpenedType == CurrentFileType::VIDEO) {
					centralWidget()->layout()->replaceWidget(mVideoCanvas, mImageCanvas);
					mVideoCanvas->stopRendering();
				}
				mOpenedType = CurrentFileType::IMAGE;
				// common operations
				connect(mImageCanvas, &ImageCanvas::fullscreenClicked, this, &Insta360Studio::onFullscreenClicked);
				connect(mImageCanvas, &ImageCanvas::previewWindowClicked, this, &Insta360Studio::onPreviewWindowClicked);
				connect(mImageCanvas, &ImageCanvas::deleteFileClicked, this, &Insta360Studio::onDeleteFileClicked);
				mImageCanvas->startRendering(filePath);
			}
			else {
				if (mVideoCanvas == nullptr) {
					mVideoCanvas = new VideoCanvas;
				}
				
				if (mOpenedType == CurrentFileType::UNINITIALIZED) {
					centralWidget()->layout()->replaceWidget(&mPlaceHolderWidget, mVideoCanvas);
				}
				else if (mOpenedType == CurrentFileType::IMAGE) {
					centralWidget()->layout()->replaceWidget(mImageCanvas, mVideoCanvas);
					mImageCanvas->stopRendering();
				}
				mOpenedType = CurrentFileType::VIDEO;
				// common operations
				connect(mVideoCanvas, &VideoCanvas::fullscreenClicked, this, &Insta360Studio::onFullscreenClicked);
				connect(mVideoCanvas, &VideoCanvas::previewWindowClicked, this, &Insta360Studio::onPreviewWindowClicked);
				connect(mVideoCanvas, &VideoCanvas::deleteFileClicked, this, &Insta360Studio::onDeleteFileClicked);
				mVideoCanvas->startRendering(filePath);
			}
			updateWindowTitle();
		}
	}

	void Insta360Studio::updateWindowTitle() {
		static const QString productName = "Insta360 Player - ";
		QString newTitle = productName + mCurrentFile;
		setWindowTitle(newTitle);
	}

	void Insta360Studio::onOpenStreamTriggered(bool checked) {
		CMessageBox msgBox;
		msgBox.setInformation(tr("Warning"), tr("Not implemented"), QPixmap(":/control/warning"));
		msgBox.exec();
	}

	void Insta360Studio::onQuitProgramTriggered(bool checked) {
		qApp->quit();
	}

	void Insta360Studio::onTutorialTriggered(bool checked) {
		CMessageBox msgBox;
		msgBox.setInformation(tr("Warning"), tr("Not implemented"), QPixmap(":/control/warning"));
		msgBox.exec();
	}

	void Insta360Studio::onCheckUpdatesTriggered(bool checked) {
		CMessageBox msgBox;
		msgBox.setInformation(tr("Warning"), tr("Not implemented"), QPixmap(":/control/warning"));
		msgBox.exec();
	}

	void Insta360Studio::onAboutTriggered(bool checked) {
		CMessageBox msgBox;
		msgBox.setInformation(tr("Warning"), tr("Not implemented"), QPixmap(":/control/warning"));
		msgBox.exec();
	}

	void Insta360Studio::onSettingsTriggered() {

	}

	void Insta360Studio::onLanguageTriggered() {
		CMessageBox msgBox;
		msgBox.setInformation(tr("Warning"), tr("Not implemented"), QPixmap(":/control/warning"));
		msgBox.exec();
	}

	void Insta360Studio::onFullscreenClicked() { 
		if (isFullScreen()) {
			ui.menuBar->show();
			showNormal();
		}
		else {
			ui.menuBar->hide();
			showFullScreen();
		}
	}

	void Insta360Studio::onPreviewWindowClicked(bool show) {

	}

	void Insta360Studio::onDeleteFileClicked() {

	}
}
