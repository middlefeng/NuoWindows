//
//  NuoBackgroundTask.cpp
//  ModelViewer
//
//  Created by Dong Feng on 2/20/2021.
//  Copyright © 2021 middleware. All rights reserved.
//

#include "NuoBackgroundTask.h"

#include "NuoTimer.h"
#include "NuoDispatch.h"


void NuoBackgroundTask::BackgroundTask(NuoTask work, NuoTaskProgress progress, NuoTaskCompletion completion)
{
    NuoDispatch* dispatch = NuoDispatch::GetInstance();

    std::shared_ptr<std::atomic<bool>> running = std::make_shared<std::atomic<bool>>();
    *running = true;

    std::shared_ptr<std::atomic<float>> progressRate = std::make_shared<std::atomic<float>>(0.0f);

    PNuoTimer timer = NuoTimer::MakeTimer(1, [progress, completion, progressRate, running](NuoTimer* timer)
        {
            if (*running)
            {
                progress(*progressRate);
                return true;
            }

            completion();
            return false;
        });

    dispatch->DispatchAsync([progressRate, running, work]()
        {
            work([progressRate](float progress)
                {
                    *progressRate = progress;
                });

            *running = false;
        });
}


