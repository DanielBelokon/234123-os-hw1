#include "JobsList.h"

JobsList::JobsList()
{
}

JobsList::~JobsList()
{
}

void JobsList::addJob(Command *cmd, bool isStopped)
{
}

void JobsList::printJobsList()
{
}

void JobsList::killAllJobs()
{
}

void JobsList::removeFinishedJobs()
{
}

JobsList::JobEntry *JobsList::getJobById(int jobId)
{
}

void JobsList::removeJobById(int jobId)
{
}

JobsList::JobEntry *JobsList::getLastJob(int *lastJobId)
{
}

JobsList::JobEntry *JobsList::getLastStoppedJob(int *jobId)
{
}