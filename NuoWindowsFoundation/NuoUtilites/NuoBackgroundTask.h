//
//  NuoBackgroundTask.h
//  ModelViewer
//
//  Created by Dong Feng on 2/20/2021.
//  Copyright © 2021 middleware. All rights reserved.
//

#ifndef NuoBackgroundTask_hpp
#define NuoBackgroundTask_hpp


#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>



class NuoTimer;
typedef std::shared_ptr<NuoTimer> PNuoTimer;


typedef std::function<void(float)> NuoTaskProgress;
typedef std::function<void(void)> NuoTaskCompletion;
typedef std::function<void(NuoTaskProgress)> NuoTask;


class NuoBackgroundTask
{

	NuoTask _work;
	PNuoTimer _timer;

	std::atomic<bool> _running;
	std::atomic<bool> _started;

	std::atomic<float> _progress;

public:

	NuoBackgroundTask(NuoTask work);

	bool Resume(float* progress);
	void Await();

	static void BackgroundTask(NuoTask work, NuoTaskProgress progress, NuoTaskCompletion completion);

};



#endif /* NuoDispatch_hpp */
