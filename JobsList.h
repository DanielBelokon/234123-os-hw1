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

        JobStatus getStatus()
        {

            pid_t wpid;
            int st = cmd->getProcessStatus(wpid);

            if (wpid == -1)
            {
                perror("smash: waitpid:");
                // exit(EXIT_FAILURE);
            }

            if (wpid == 0)
            {
                return status;
            }

            if (WIFEXITED(st) || WIFSIGNALED(st))
            {
                // printf("child exited, status=%d\n", WEXITSTATUS(st));
                status = DONE;
            }
            else if (WIFSTOPPED(st))
            {
                // printf("child stopped (signal %d)\n", WSTOPSIG(st));
                status = STOPPED;
            }
            else if (WIFCONTINUED(st))
            {
                // printf("child continued\n");
                status = RUNNING;
            }
            else
            { /* Non-standard case -- may never happen */
                // printf("Unexpected status (0x%x)\n", status);
            }

            return status;
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
    int addJob(ExternalCommand *cmd, bool isStopped = false);
    void printJobsList(std::ostream &out = std::cout);
    void killAllJobs();
    void removeFinishedJobs();
    void updateMaxJobId();
    JobEntry &getJobById(int jobId);
    void removeJobById(int jobId);
    JobEntry &getLastJob();
    JobEntry &getLastStoppedJob();
    int getMaxJobIdInArray();
    JobEntry &getJobWithMaxID();
    bool continueJob(int jobId);
    int size();
    int countStoppedJobs();

    // TODO: Add extra methods or modify exisitng ones as needed
};