#include "JobsList.h"

JobsList::JobsList()
{
    maxJobId = 0;
}

JobsList::~JobsList()
{
    delete jobs;
}

void JobsList::addJob(Command *cmd, bool isStopped)
{
    // maybe we need to execute the command first ? and get the pid from the execute ?
    // pid_t pid = execute(); ?
    JobEntry *job = new JobEntry(cmd, maxJobId, pid, isStopped ? STOPPED : RUNNING);
    jobs.push_back(job);
    maxJobId++;
}

void JobsList::printJobsList()
{
    removeFinishedJobs();

    for (auto &job : jobs)
    {
        std::cout << "[" << job->job.id << "]" << job->cmd->getCommand() << " : " << job->jobPid << " ";
        time_t time = difftime(time(NULL), job->jobStartTime);
        std::cout << time << " secs";
        if (job->status == STOPPED)
        {
            std::cout << " (stopped)";
        }
        std::cout << std::endl;
    }
}

void JobsList::killAllJobs()
{
}

void JobsList::removeFinishedJobs()
{
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
}