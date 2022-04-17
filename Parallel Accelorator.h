#pragma once
#include <thread>
#include <vector>
#include <future>

namespace Parallel_Accelorator {
		template<typename RT, typename T>
		void call(std::promise<RT> && p,T Data, std::function<RT(T)> callable) {
			p.set_value(callable(Data));
		}
		template< typename RT, typename T>
		std::vector<RT> Parallel_Accelorator(std::vector<T> Data, std::function<RT(T)> callable) {
			std::vector<RT> ret;
			std::vector<std::thread> threads;
			std::vector<std::promise<RT>> threadPromises;
			std::vector<std::future<RT>> threadFutures;
			for (int i = 0; i < Data.size(); i++)
			{
				threadPromises.push_back(std::promise<RT>());
				threadFutures.push_back(threadPromises[threadPromises.size()-1].get_future());
				//std::thread(&call<RT,T>, std::move(threadFutures[i]), Data[i], callable);
				//threads.push_back();
			}
			for (int i = 0; i < threads.size(); i++)
			{
				threads[i].join();
				ret.push_back(threadFutures[i].get());
			}
			return ret;
		}
}