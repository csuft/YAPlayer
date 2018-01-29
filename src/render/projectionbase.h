#ifndef PROJECTIONBASE_H
#define PROJECTIONBASE_H

#include <control/lookatcontroller.h>

namespace av3d {

	class ProjectionBase : public QObject
	{
		Q_OBJECT
	public:
		ProjectionBase(Camera* camera);
		void init();

		void setCamera(Camera* camera);

		Camera* camera() {
			return mCamera;
		}

		bool needsUpdate() const {
			return mLookatController->needsUpdate();
		}
		 
		void update() {
			mLookatController->update();
		}

		virtual void setViewport(float width, float height) {
			mViewportWidth = width;
			mViewportHeight = height;
		}

		virtual void takeOver(const ProjectionBase* fromProjection);

	signals:
		void projectionChanged();

	protected:
		LookAtController* mLookatController;
		float mViewportWidth;
		float mViewportHeight;
		Camera* mCamera;

		virtual void initDefault() = 0;
	}; 
}

#endif // PROJECTIONBASE_H
