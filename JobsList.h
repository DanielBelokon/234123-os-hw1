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
            int getJobId()
            {
                return jobId;
            }
            int getJobPid()
            {
                return jobPid;
            }
    };
    // TODO: Add your data members
private:
    std::vector<JobEntry> jobs;
    int maxJobId;

public:
    JobsList();
    ~JobsList();
    std::vector<JobEntry> &getJobsVectorList();
    void addJob(ExternalCommand *cmd, bool isStopped = false);
    void printJobsList(std::ostream &out = std::cout);
    void killAllJobs();
    void removeFinishedJobs();
    void updateMaxJobId();
    JobEntry &getJobById(int jobId);
    void removeJobById(int jobId);
    JobEntry &getLastJob(int lastJobId);
    JobEntry &getLastStoppedJob(int jobId);
    int getMaxJobIdInArray();
    JobEntry &getJobWithMaxID();
    bool continueJob(int jobId);
    // TODO: Add extra methods or modify exisitng ones as needed
};