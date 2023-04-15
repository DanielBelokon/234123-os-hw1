#include "JobsList.h"
#include <iostream>

JobsList::JobsList()
{
    maxJobId = 0;
}

JobsList::~JobsList()
{
    for (auto &job : jobs)
    {
        delete job;
    }
}

void JobsList::addJob(ExternalCommand *cmd, bool isStopped)
{
    // maybe we need to execute the command first ? and get the pid from the execute ?
    //pid_t pid = (int)cmd->execute(); // Note : execute is void , maybe need to change.
    pid_t pid = cmd->getPid();
    JobEntry *job = new JobEntry(cmd, maxJobId, pid, isStopped ? STOPPED : RUNNING);
    jobs.push_back(job);
    maxJobId++;
}

void JobsList::printJobsList()
{
    removeFinishedJobs();

    for (auto &job : jobs)
    {
        std::cout << "[" << job->jobId << "]" << job->cmd->getCommandName() << " : " << job->jobPid << " ";
        time_t delta_time = difftime(time(nullptr), job->timeStarted);
        std::cout << delta_time << " secs";
        if (job->status == STOPPED)
        {
            std::cout << " (stopped)";
        }
        std::cout << std::endl;
    }
}

void JobsList::killAllJobs()
{
    for (auto &job : jobs)
    {
        job->cmd->killProcess();
    }
}

void JobsList::removeFinishedJobs()
{
    for (int i = 0; i < jobs.size(); i++)
    {
        if (jobs[i]->status == DONE || jobs[i]->status == REMOVED)
        {
            jobs.erase(jobs.begin() + i);
        }
    }
    updateMaxJobId();
}

JobsList::JobEntry *JobsList::getJobById(int jobId)
{
    for (int i = 0; i < jobs.size(); i++)
    {
        if (jobs[i]->jobId == jobId)
        {
            return jobs[i];
        }
    }
    return NULL;
}

void JobsList::removeJobById(int jobId)
{
    for (int i = 0; i < jobs.size(); i++)
    {
        if (jobs[i]->jobId == jobId)
        {
            jobs.erase(jobs.begin() + i);
        }
    }
}

JobsList::JobEntry *JobsList::getLastJob(int *lastJobId)
{
    return jobs[jobs.size() - 1];
}

JobsList::JobEntry *JobsList::getLastStoppedJob(int *jobId)
{
    for (int i = jobs.size() - 1; i >= 0; i--)
    {
        if (jobs[i]->status == STOPPED)
        {
            return jobs[i];
        }
    }
    return NULL;
}

int JobsList::getMaxJobIdInArray()
{
    int temp = -1;
    for (int i = 0; i < jobs.size(); i++)
    {
        if (jobs[i]->jobId > temp)
        {
            temp = jobs[i]->jobId;
        }
    }
    return temp;
}
void JobsList::updateMaxJobId()
{
    maxJobId = getMaxJobIdInArray();
}
JobEntry* JobsList::getJobWithMaxID()
{
    for (int i = 0; i < jobs.size(); i++)
    {
        if (jobs[i]->jobId == maxJobId)
        {
            return jobs[i];
        }
    }
    return NULL;
}