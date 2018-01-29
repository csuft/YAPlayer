#ifndef SPHERICALPROJECTION_H
#define SPHERICALPROJECTION_H

#include "projectionbase.h"
#include "panoramaviewer.h"

namespace av3d {

	class SphericalProjection : public ProjectionBase, public PanoramaViewer
	{
		Q_OBJECT
	public:
		SphericalProjection(Camera* camera, float radius = 1000);

		void setSphereRadius(float radius);
		virtual void rotateBy(float pan, float tilt);
		virtual void rotate(float pan, float tilt);
		virtual void zoomBy(float value);
		virtual float pan() const {
			return mLookatController->panAngle();
		}
		virtual float tilt() const {
			return mLookatController->tiltAngle();
		}

	protected:
		Camera* mPerspectiveCamera;
		float mFieldOfView;
		float mMinFOV;
		float mMaxFOV;
		float mSphereRadius;

		void initDefault() override;
	};
}

#endif // SPHERICALPROJECTION_H
