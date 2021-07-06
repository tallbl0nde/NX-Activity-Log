#ifndef APPLICATION_EXPORTJOB_HPP
#define APPLICATION_EXPORTJOB_HPP

#include "Application.hpp"
#include "Aether/ThreadPool.Job.hpp"

namespace Main {
    // Implements an Aether thread pool job to export the current play data
    // read within the application to a JSON file on disk.
    class Application::ExportJob : public Aether::ThreadPool::Job {
        private:
            // Pointer to application object
            Application * app;

            // Atomic to update with status
            std::atomic<double> & percent;

        protected:
            // Implements to perform the export
            void work();

        public:
            // Constructor takes application object
            ExportJob(Application *, std::atomic<double> &);
    };
};

#endif
