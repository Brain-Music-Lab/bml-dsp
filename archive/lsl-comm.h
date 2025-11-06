#ifndef _BML_DSP_LSL_COMM_H_
#define _BML_DSP_LSL_COMM_H_

#include <lsl_cpp.h>
#include <mutex>
#include <functional>


namespace BML
{
	namespace LSL
	{
		void getLslBlocks(const std::string& prop,
						  const std::string& type,
						  std::vector<std::vector<double>>& block,
						  std::mutex& mut,
						  std::atomic_bool& running,
						  std::function<void(void)> callback,
						  double timeout = LSL_FOREVER);

		void getTest();
	}
}

#endif
