/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2022 to 2022 Tectiform Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


******************************************************************************/

#include "tfsystemdservice.hpp"

namespace TF::Linux
{

    // We could use signals and the signal manager to handle termination, but polling after a launch takes
    // so much less code, and we do not get any huge benefit from using signals (we do not have a bunch of
    // other work to do here).

    SystemdService::SystemdService(const string_type & service_name) : m_service_name{service_name} {}

    void SystemdService::start()
    {
        auto result = run_basic_operation("start");
        if (std::get<0>(result) != 0)
        {
            LOG(LogPriority::Info, "Failed to start service %S", m_service_name)
            throw std::runtime_error{"Unable to start service"};
        }
    }

    void SystemdService::stop()
    {
        auto result = run_basic_operation("stop");
        if (std::get<0>(result) != 0)
        {
            LOG(LogPriority::Info, "Failed to stop service %S", m_service_name)
            throw std::runtime_error{"Unable to stop service"};
        }
    }

    void SystemdService::restart()
    {
        stop();
        start();
    }

    auto SystemdService::get_status() const -> Status
    {
        auto result = run_basic_operation("status");
        if (std::get<0>(result) != 0)
        {
            return Status::STOPPED;
        }

        auto stdout_data_bytes = std::get<1>(result).bytes();
        string_type stdout_string{stdout_data_bytes};

        auto location = stdout_string.rangeOfString("Active: active");
        if (! location.is_empty())
        {
            return Status::RUNNING;
        }

        location = stdout_string.rangeOfString("Active: inactive");
        if (! location.is_empty())
        {
            return Status::STOPPED;
        }

        return Status::UNKNOWN;
    }

    auto SystemdService::is_running() const -> bool
    {
        return get_status() == Status::RUNNING;
    }

    auto SystemdService::run_basic_operation(const string_type & op) const -> std::tuple<int, data_type, data_type>
    {
        Process systemd{};
        string_type command = "systemclt " + op + " " + m_service_name + ".service";
        systemd.launch(command);
        auto stdout_handle = systemd.handle_for_standard_out();
        auto stderr_handle = systemd.handle_for_standard_err();
        systemd.update_exit_status();
        while (systemd.is_alive())
        {
            Sleep(std::chrono::milliseconds{30});
            systemd.update_exit_status();
        }
        auto exit_code = systemd.stop_code();
        auto stdout_data = stdout_handle.readToEndOfFile();
        auto stderr_data = stderr_handle.readToEndOfFile();

        return {exit_code, stdout_data, stderr_data};
    }

} // namespace TF::Linux
