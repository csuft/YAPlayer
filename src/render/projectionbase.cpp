#include "projectionbase.h"

namespace av3d {

	ProjectionBase::ProjectionBase(Camera* camera) : mCamera(camera) {
		mLookatController = new LookAtController(mCamera);
		mLookatController->setYFactor(1);
		mLookatController->setWrapAngle(false);
	}

	void ProjectionBase::init() {
		initDefault();
		mLookatController->update(false);
	}

	void ProjectionBase::setCamera(Camera *camera) {
		if (camera != nullptr) {
			mCamera = camera;
			mLookatController->setCamera(camera);
			mLookatController->update(false);
		}
	}

	void ProjectionBase::takeOver(const ProjectionBase *fromProjection) {
		if (fromProjection == nullptr) {
			return;
		}

		mLookatController->setPanAngle(fromProjection->mLookatController->panAngle());
		mLookatController->setTiltAngle(fromProjection->mLookatController->tiltAngle());
		mLookatController->setDistance(fromProjection->mLookatController->distance());
		mLookatController->update(false);
		initDefault();
	}
}


