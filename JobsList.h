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
        int jobPid;
        JobStatus status;
        time_t timeStarted;
        time_t timeoutTime;

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

        int getPid()
        {
            return jobPid;
        }

        JobStatus getStatus()
        {

            pid_t wpid;
            int st = getProcessStatus(wpid);

            if (wpid == -1)
            {
                return DONE;
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

        void killProcess()
        {
            sigProcess(SIGKILL);
            return;
        }

        void continueProcess()
        {
            sigProcess(SIGCONT);
            return;
        }

        void stopProcess()
        {
            sigProcess(SIGTSTP);
            return;
        }

        std::string getCmdLine()
        {
            return cmd;
        }

        int getProcessStatus(pid_t &retPid)
        {
            int status;
            retPid = waitpid(this->getPid(), &status, WUNTRACED | WNOHANG | WCONTINUED);
            return status;
        }

        void sigProcess(int _sig)
        {
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