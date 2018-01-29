#ifndef IMAGE_CANVAS_H
#define IMAGE_CANVAS_H 

#include "abstractcanvas.h" 
#include "ui_imagecanvastool.h"

namespace player {
	class ImageCanvas : public AbstractCanvas {
		Q_OBJECT

	public:
		explicit ImageCanvas(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
		~ImageCanvas();
		virtual void startRendering(const QString& filePath) override;
		virtual void stopRendering() override;  

	protected slots:
		virtual void onFullScreenClicked(bool check) override;
		virtual void onPreviewClicked(bool check) override;
		virtual void onDeleteClicked(bool check) override;
	private:
		Ui::imageCanvasTool mImagecanvasToolBar;   
	};
}


#endif // IMAGE_CANVAS_H
