#include "JobsList.h"
#include <iostream>

JobsList::JobsList()
{
    maxJobId = 0;
}

JobsList::~JobsList()
{
}
std::vector<JobsList::JobEntry> &JobsList::getJobsVectorList()
{
    return jobs;
}
int JobsList::addJob(pid_t pid, std::string cmd, time_t timeout)
{
    JobEntry job = JobEntry(cmd, ++maxJobId, pid, timeout);

    jobs.push_back(job);

    if (timeout != -1)
    {
        alarm(timeout);
    }

    return job.jobId;
}

void JobsList::printJobsList(std::ostream &out)
{
    removeFinishedJobs();

    for (auto &job : jobs)
    {
        if (job.isForeground)
            continue;
        std::cout << "[" << job.jobId << "] " << job.cmd << " : " << job.jobPid << " ";
        time_t delta_time = difftime(time(nullptr), job.timeStarted);
        std::cout << delta_time << " secs";
        if (job.getStatus() == STOPPED)
        {
            std::cout << " (stopped)";
        }
        std::cout << std::endl;
    }
}

void JobsList::killAllJobs()
{
    removeFinishedJobs();
    std::cout << "smash: sending SIGKILL signal to " << jobs.size() << " jobs:" << std::endl;

    for (auto &job : jobs)
    {
        std::cout << job.jobPid << ": " << job.cmd << std::endl;
        job.killProcess();
    }
}

void JobsList::removeFinishedJobs()
{
    for (int i = 0; i < jobs.size(); i++)
    {
        JobStatus status = jobs[i].getStatus();
        if (status == DONE || status == REMOVED)
        {
            jobs.erase(jobs.begin() + i);
        }
    }
    updateMaxJobId();
}

JobsList::JobEntry &JobsList::getJobById(int jobId)
{
    for (int i = 0; i < jobs.size(); i++)
    {
        if (jobs[i].jobId == jobId)
        {
            return jobs[i];
        }
    }

    throw std::invalid_argument("Job not found");
}

void JobsList::removeJobById(int jobId)
{
    for (int i = 0; i < jobs.size(); i++)
    {
        if (jobs[i].jobId == jobId)
        {
            jobs.erase(jobs.begin() + i);
            return;
        }
    }

    throw std::invalid_argument("Job not found");
}

JobsList::JobEntry &JobsList::getLastJob()
{
    return jobs[jobs.size() - 1];
}

JobsList::JobEntry &JobsList::getLastStoppedJob()
{
    for (int i = jobs.size() - 1; i >= 0; i--)
    {
        if (jobs[i].getStatus() == STOPPED)
        {
            return jobs[i];
        }
    }

    throw std::invalid_argument("No stopped jobs");
}

int JobsList::getMaxJobIdInArray()
{
    int temp = 0;
    for (int i = 0; i < jobs.size(); i++)
    {
        if (jobs[i].jobId > temp)
        {
            temp = jobs[i].jobId;
        }
    }
    return temp;
}

void JobsList::updateMaxJobId()
{
    maxJobId = getMaxJobIdInArray();
}

JobsList::JobEntry &JobsList::getJobWithMaxID()
{
    for (int i = 0; i < jobs.size(); i++)
    {
        if (jobs[i].jobId == maxJobId)
        {
            return jobs[i];
        }
    }

    throw std::invalid_argument("Job not found");
}

bool JobsList::continueJob(int jobId)
{
    getJobById(jobId).continueProcess();
    return true;
}

int JobsList::size()
{
    return jobs.size();
}

int JobsList::countStoppedJobs()
{
    int count = 0;
    for (int i = 0; i < jobs.size(); i++)
    {
        if (jobs[i].getStatus() == STOPPED)
        {
            count++;
        }
    }
    return count;
}

void JobsList::timeoutJob()
{
    removeFinishedJobs();
    for (int i = 0; i < jobs.size(); i++)
    {
        if (jobs[i].timeoutTime != -1 && jobs[i].timeoutTime <= time(nullptr))
        {
            std::cout << "smash: " << jobs[i].cmd << " timed out!" << std::endl;
            jobs[i].killProcess();
        }
    }
}