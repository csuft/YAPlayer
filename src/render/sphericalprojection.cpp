#include "sphericalprojection.h"

namespace av3d {

	SphericalProjection::SphericalProjection(Camera* camera,float radius):
		ProjectionBase(camera),
		mSphereRadius(radius) {

	}

	void SphericalProjection::initDefault() {
		mLookatController->setPanAngle(90);
		mLookatController->setTiltAngle(0);
		setSphereRadius(mSphereRadius);
		mLookatController->setDistance(00);
		mLookatController->setWrapAngle(false);
		emit ProjectionBase::projectionChanged();
	}

	void SphericalProjection::setSphereRadius(float radius) {
		mSphereRadius = radius;
		mLookatController->setMinDistance(radius * 0.01);
		mLookatController->setMaxDistance(radius * 1.8);
	}

	void SphericalProjection::rotate(float pan, float tilt) {
		mLookatController->setPanAngle(pan);
		mLookatController->setTiltAngle(tilt);
		emit ProjectionBase::projectionChanged();
	}

	void SphericalProjection::rotateBy(float pan, float tilt) {
		mLookatController->setPanAngle(mLookatController->panAngle() + pan);
		mLookatController->setTiltAngle(mLookatController->tiltAngle() + tilt);
		emit ProjectionBase::projectionChanged();
	}

	void SphericalProjection::zoomBy(float value) {
		mLookatController->setDistance(mLookatController->distance() - value);
		emit ProjectionBase::projectionChanged();
	}

}
