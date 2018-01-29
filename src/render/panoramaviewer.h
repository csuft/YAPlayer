#ifndef PANORAMAVIEWER_H
#define PANORAMAVIEWER_H

namespace av3d {

	class PanoramaViewer {
	public:
		virtual void rotateBy(float pan, float tilt) = 0;
		virtual void rotate(float pan, float tilt) = 0;
		virtual void zoomBy(float value) = 0;
		virtual float pan() const = 0;
		virtual float tilt() const = 0;
	};
}


#endif // PANORAMALISTENER_H
