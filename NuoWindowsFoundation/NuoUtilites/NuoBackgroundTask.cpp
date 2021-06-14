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


NuoBackgroundTask::NuoBackgroundTask(NuoTask work)
    : _work(work), _started(false), _running(false), _progress(0)
{
}



bool NuoBackgroundTask::Resume(float* progress)
{
    if (!_started)
    {
        auto running = &_running;

        _timer = NuoTimer::MakeTimer(1, [running](NuoTimer* timer)
            {
                if (!(*running))
                    return false;
            });

        auto progressRate = &_progress;
        auto work = _work;

        _running = true;

        NuoDispatch* dispatch = NuoDispatch::GetInstance();
        dispatch->DispatchAsync([progressRate, running, work]()
            {
                work([progressRate](float progress)
                    {
                        *progressRate = progress;
                    });

                *running = false;
            });

        _started = true;
    }

    MSG msg;

    // Main message loop:
    if (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (progress)
        *progress = _progress;

    return _running;
}


void NuoBackgroundTask::Await()
{
    while (Resume(nullptr));
}


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


