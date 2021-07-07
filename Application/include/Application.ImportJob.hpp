#ifndef APPLICATION_IMPORTJOB_HPP
#define APPLICATION_IMPORTJOB_HPP

#include "Application.hpp"
#include "Aether/ThreadPool.Job.hpp"

namespace Main {
    // Implements an Aether thread pool job to import play data from a JSON
    // to another JSON used internally.
    class Application::ImportJob : public Aether::ThreadPool::Job {
        private:
            // Pointer to application object
            Application * app;

            // Atomic to update with status
            std::atomic<double> & percent;

        protected:
            // Implements to perform the import
            void work();

        public:
            // Constructor takes application object
            ImportJob(Application *, std::atomic<double> &);
    };
};

#endif
