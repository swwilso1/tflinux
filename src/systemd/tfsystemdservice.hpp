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

#ifndef TFSYSTEMDSERVICE_HPP
#define TFSYSTEMDSERVICE_HPP

#include <tuple>
#include "TFFoundation.hpp"

using namespace TF::Foundation;

namespace TF::Linux
{

    /**
     * SystemdService provides an interface to manipulation systemd services on a Linux system.
     */
    class SystemdService
    {
    public:
        enum class Status
        {
            STOPPED,
            RUNNING,
            UNKNOWN
        };

        using string_type = String;

        /**
         * @brief constructor with service name parameter.
         * @param service_name the name of the service.
         */
        explicit SystemdService(const string_type & service_name);

        /**
         * @brief method to start the systemd service.
         */
        void start();

        /**
         * @brief method to stop the systemd service.
         */
        void stop();

        /**
         * @brief method to restart the systemd service.
         */
        void restart();

        /**
         * @brief method to get the running status of the systemd service.
         * @return Status::RUNNING if the service is running, Status::STOPPED if
         * the system is not running and Status::UNKNOWN if the method cannot determine
         * the status.
         */
        [[nodiscard]] auto get_status() const -> Status;

        /**
         * @brief method to check if the service is running
         * @return true if the service is running.
         */
        [[nodiscard]] auto is_running() const -> bool;

    private:
        using data_type = Data;

        string_type m_service_name{};

        [[nodiscard]] auto run_basic_operation(const string_type & op) const -> std::tuple<int, data_type, data_type>;
    };

} // namespace TF::Linux

#endif // TFSYSTEMDSERVICE_HPP
