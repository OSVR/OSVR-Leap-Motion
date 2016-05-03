#include <osvr/PluginKit/PluginKit.h>
#include <chrono>
#include <thread>

namespace LeapOsvr {
	
	class HardwareDetection {

		public:

			HardwareDetection();
			OSVR_ReturnCode operator()(OSVR_PluginRegContext pContext);

		private:

			bool mFound;

	};

}
