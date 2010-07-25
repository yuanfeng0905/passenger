#  Phusion Passenger - http://www.modrails.com/
#  Copyright (c) 2010 Phusion
#
#  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
#
#  See LICENSE file for license information.

module PhusionPassenger
module ClassicRailsExtensions
module AnalyticsLogging

module ACBenchmarkingExtension
	def benchmark_with_passenger(title, *args)
		# Log ActionController benchmarking blocks.
		log = Thread.current[PASSENGER_ANALYTICS_WEB_LOG]
		if log
			log.measure("BENCHMARK: #{title}") do
				benchmark_without_passenger(title, *args) do
					yield
				end
			end
		else
			benchmark_without_passenger(title, *args) do
				yield
			end
		end
	end
end

end
end
end