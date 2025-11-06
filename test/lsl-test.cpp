#include <chrono>
#include <iostream>
#include <lsl_cpp.h>
#include <stdint.h>
#include <thread>


int main(int argc, char **argv) {
	try {

		bool flush = argc > 3;
		// resolve the stream of interest & make an inlet
		std::vector<lsl::stream_info> inlet_info = lsl::resolve_stream("type", "EEG", 1, LSL_FOREVER);

		int32_t maxBufLength = 360;
		lsl::stream_inlet inlet(inlet_info[0], maxBufLength);

		// Use set_postprocessing to get the timestamps in a common base clock.
		// Do not use if this application will record timestamps to disk -- it is better to 
		//  do posthoc synchronization.
		inlet.set_postprocessing(lsl::post_ALL);

		double starttime = lsl::local_clock();
		double next_display = starttime + 1;
		double next_reset = starttime + 10;

		// and retrieve the chunks
		uint64_t k = 0, num_samples = 0;
		std::vector<std::vector<double>> result;
		auto fetch_interval = std::chrono::milliseconds(20);
		auto next_fetch = std::chrono::steady_clock::now() + fetch_interval;
		double timestamp = -1.0;


		while (true) 
		{
			std::this_thread::sleep_until(next_fetch);
			if (timestamp = inlet.pull_chunk(result))
			{
				num_samples += result.size();
			}
			next_fetch += fetch_interval;
		}

	} 
	catch (std::exception &e) 
	{ 
		std::cerr << "Got an exception: " << e.what() << std::endl; 
	}

	return 0;
}