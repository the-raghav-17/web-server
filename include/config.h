/*
 * config.h
 *
 * Types and functions necessary
 * for setting up server config
 */


#ifndef CONFIG_H_
#define CONFIG_H_


#include <string>


namespace Config
{
    struct Type
    {
        /**
         * Path to directory where
         * all HTTP content is stored
         */
        std::string root_path{};

        /**
         * Number of worker threads in
         * the threadpool
         */
        int thread_count{};     

        /**
         * Maximum number of connections
         * allowed to be queued when listening
         * for multiple connections
         */
        int max_queued_conn{};
    };

    Type parse_config_file(const std::string& file);
};



#endif // CONFIG_H_
