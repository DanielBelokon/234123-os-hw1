#pragma once

#include "ExternalCommands.h"
#include <ctime>
#include <vector>
#include <map>
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
        std::string cmd;
        int jobId;
        pid_t jobPid;
        JobStatus status;
        time_t timeStarted;
        time_t timeoutTime;
        bool isForeground = false;

        JobEntry(std::string cmd, int jobId, int pid, time_t timeout = -1) : cmd(cmd), jobId(jobId), jobPid(pid)
        {
            timeStarted = time(nullptr);

            if (timeout != -1)
            {
                timeoutTime = timeStarted + timeout;
            }
            else
            {
                timeoutTime = -1;
            }
        }

        int getJobId()
        {
            return jobId;
        }

        pid_t getPid()
        {
            return jobPid;
        }

        JobStatus getStatus()
        {
            int st;
            pid_t wpid = waitpid(this->getPid(), &st, WUNTRACED | WNOHANG);
            if (wpid == -1)
            {
                return REMOVED;
            }

            if (wpid == 0)
            {
                return status;
            }

            if (WIFSTOPPED(st))
            {
                // printf("child stopped (signal %d)\n", WSTOPSIG(st));
                status = STOPPED;
            }
            else if (WIFEXITED(st) || WIFSIGNALED(st))
            {
                // printf("child exited, status=%d\n", WEXITSTATUS(st));
                status = DONE;
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

        void killProcess()
        {
            status = REMOVED;
            sigProcess(SIGKILL);
            return;
        }

        void continueProcess()
        {
            status = RUNNING;
            sigProcess(SIGCONT);
            return;
        }

        void stopProcess()
        {
            status = STOPPED;
            sigProcess(SIGSTOP);
            return;
        }

        std::string getCmdLine()
        {
            return cmd;
        }

        void sigProcess(int _sig)
        {
            // printf("smash > signal %d was sent to pid %d\n", _sig, this->getPid());
            if (kill(this->getPid(), _sig) == -1)
            {
                perror("smash error: kill failed");
            }
        }
    };
    // TODO: Add your data members
private:
    std::vector<JobEntry> jobs;
    std::map<time_t, int> timeoutMap;
    int maxJobId;

public:
    JobsList();
    ~JobsList();
    std::vector<JobEntry> &getJobsVectorList();
    int addJob(pid_t pid, std::string cmd, time_t timeout = -1);
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

    void timeoutJob();

    // TODO: Add extra methods or modify exisitng ones as needed
};