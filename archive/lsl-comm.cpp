#include <chrono>
#include <iostream>
#include <lsl_cpp.h>
#include <stdint.h>
#include <thread>
#include <atomic>

#include "lsl-comm.h"

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
			double timeout)
		{
			{
				try
				{
					std::vector<lsl::stream_info> inlet_info = lsl::resolve_stream(prop, type, 1, timeout);

					int32_t maxBufLength = 360;
					lsl::stream_inlet inlet(inlet_info[0], maxBufLength);

					// Get the timestamps in a common base clock.
					inlet.set_postprocessing(lsl::post_ALL);

					double starttime = lsl::local_clock();
					double next_display = starttime + 1;
					double next_reset = starttime + 10;

					// and retrieve the chunks
					auto fetch_interval = std::chrono::milliseconds(20);
					auto next_fetch = std::chrono::steady_clock::now() + fetch_interval;
					double timestamp = -1.0;

					{
						std::lock_guard<std::mutex> guard(mut);
						running = true;
					}

					while (running)
					{
						std::this_thread::sleep_until(next_fetch);
						{
							std::lock_guard<std::mutex> guard(mut);
							if (timestamp = inlet.pull_chunk(block))
								callback();
						}
						next_fetch += fetch_interval;
					}
				}

				catch (std::exception& e)
				{
					throw (e);
				}
			}
		}
	}
}