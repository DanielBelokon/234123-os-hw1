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
void JobsList::addJob(ExternalCommand *cmd, bool isStopped)
{
    // maybe we need to execute the command first ? and get the pid from the execute ?
    //pid_t pid = (int)cmd->execute(); // Note : execute is void , maybe need to change.
    pid_t pid = cmd->getPid();
    JobEntry job = JobEntry(cmd, ++maxJobId, pid, isStopped ? STOPPED : RUNNING);
    jobs.push_back(job);
}

void JobsList::printJobsList(std::ostream &out)
{
    removeFinishedJobs();

    for (auto &job : jobs)
    {
        std::cout << "[" << job.jobId << "]" << job.cmd->getCommandName() << " : " << job.jobPid << " ";
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
    std::cout << "smash: sending SIGKILL signal to " << jobs.size() << " jobs:" << std::endl;

    for (auto &job : jobs)
    {
        std::cout << job.jobPid << ": " << job.cmd->getCmdLine() << std::endl;
        job.cmd->killProcess();
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
    getJobById(jobId).cmd->continueProcess();
    getJobById(jobId).status = RUNNING;
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