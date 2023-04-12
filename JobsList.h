#pragma once

#include "ExternalCommands.h"
#include <ctime>
#include <vector>
class JobsList
{
    enum JobStatus
    {
        RUNNING,
        STOPPED,
        DONE,
        REMOVED
    };

public:
    class JobEntry
    {
        public:
            ExternalCommand *cmd;
            int jobId;
            int jobPid;
            JobStatus status;
            time_t timeStarted;
            JobEntry(ExternalCommand *cmd, int jobId, int pid, JobStatus status) : cmd(cmd), jobId(jobId), jobPid(pid), status(status), timeStarted(time(nullptr))
            {
            }
    };
    // TODO: Add your data members
private:

    std::vector<JobEntry*> jobs;
    int maxJobId;

public:
    JobsList();
    ~JobsList();
    void addJob(ExternalCommand *cmd, bool isStopped = false);
    void printJobsList();
    void killAllJobs();
    void removeFinishedJobs();
    JobEntry *getJobById(int jobId);
    void removeJobById(int jobId);
    JobEntry *getLastJob(int *lastJobId);
    JobEntry *getLastStoppedJob(int *jobId);
    // TODO: Add extra methods or modify exisitng ones as needed
};