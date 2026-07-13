/*
 * SPDX-License-Identifier: GNU GPLv3
 * Copyright (c) 2026 Raghav Sharma
 * GitHub: https://github.com/the-raghav-17
 */

#ifndef RESPONDER_H_
#define RESPONDER_H_


/**
 * Responder class is responsible for parsing the request message
 * and then generating a valid response for that message.
 * It will produce a response string for a particular request message
 * and for that it will also read the file contents requested.
 */
class Responder
{
public:
    /**
     * Responder constructor takes the request message
     * and the name of the root directory where all the
     * HTTP content is stored
     */
    Responder(const std::string& request_msg, const std::string& root_path="public") noexcept;

    /**
     * Generates a string which to be sent back to the client
     * which sent the request message
     */
    std::string   generate_response();

    /**
     * Get the parsed request message.
     * Returns the struct which has all the details of
     * the parsed request message.
     */
    Http::Request get_request_details();

private:
    /**
     * The root directory where all the 
     * HTTP content is stored
     */
    std::string   m_root_path{};

    /**
     * The original request message sent
     * by the client to be parsed.
     */
    std::string   m_request_msg{};

    /**
     * The parsed request message
     */
    Http::Request m_request{};

    /**
     * The response message generated
     * by the responder
     */
    std::string   m_response_msg{};

    /**
     * Modifies the path of resource requested
     * by adding the root path string in front
     * and making any other changes required
     */
    std::string process_path(const std::string& path);
};


#endif // RESPONDER_H_
