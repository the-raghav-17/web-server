/*
 * SPDX-License-Identifier: GNU GPLv3
 * Copyright (c) 2026 Raghav Sharma
 * GitHub: https://github.com/the-raghav-17
 */

#ifndef RESPONDER_H_
#define RESPONDER_H_


class Responder
{
public:
    Responder(const std::string& request_msg, const std::string& root_path="public") noexcept;

    std::string   generate_response();
    Http::Request get_request_details();

private:
    std::string   m_root_path{};
    std::string   m_request_msg{};
    Http::Request m_request{};
    std::string   m_response_msg{};

    std::string process_path(const std::string& path);
};


#endif // RESPONDER_H_
