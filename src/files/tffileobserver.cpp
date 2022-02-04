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

#include <system_error>
#include <cerrno>
#include <fcntl.h>
#include <sys/fanotify.h>
#include <unistd.h>
#include "tffileobserver.hpp"

namespace TF::Linux
{

    FileObserver::FileObserver(int flags, int modes) : m_notifier_fd{0}, m_pipe_fd{0, 0}
    {
        m_notifier_fd = fanotify_init(flags, modes);
        if (m_notifier_fd < 0)
        {
            throw std::system_error{errno, std::system_category(), "fanotify_init failed"};
        }

        auto pipe_api_result = pipe2(m_pipe_fd, O_CLOEXEC | O_NONBLOCK);
        if (pipe_api_result < 0)
        {
            throw std::system_error{errno, std::system_category()};
        }
    }

    FileObserver::~FileObserver()
    {
        close(m_notifier_fd);
    }

    void FileObserver::mark(const string_type & path, uint32_t flags, uint64_t mask) const
    {
        mark(flags, mask, AT_FDCWD, path);
    }

    void FileObserver::mark(uint32_t flags, uint64_t mask, int dirfd, const string_type & path) const
    {
        auto path_cstring_value = path.cStr();
        auto mark_api_result = fanotify_mark(m_notifier_fd, flags, mask, dirfd, path_cstring_value.get());
        if (mark_api_result < 0)
        {
            throw std::system_error{errno, std::system_category(), "fanotify_mark failed"};
        }
    }

    void FileObserver::run(const std::function<void(event_metadata_type *)> & event_callback)
    {
        bool keep_monitoring{true};
        Poller poller;
        poller.add_handle(m_pipe_fd[0], PollEvent::Read, [&keep_monitoring](int) -> void {
            keep_monitoring = false;
        });

        poller.add_handle(m_notifier_fd, PollEvent::Read, [&event_callback, &keep_monitoring, this](int) -> void {
            event_metadata_type event_buffer[EVENT_BUFFER_SIZE];
            event_metadata_type * current_event;

            while (keep_monitoring)
            {
                auto bytes_read =
                    read(this->m_notifier_fd, reinterpret_cast<void *>(event_buffer), sizeof(event_buffer));
                if (bytes_read < 0 && errno != EAGAIN)
                {
                    throw std::system_error{errno, std::system_category(), "read failed"};
                }

                if (bytes_read <= 0)
                {
                    break;
                }

                current_event = event_buffer;

                while (keep_monitoring && FAN_EVENT_OK(current_event, bytes_read))
                {
                    if (current_event->vers != FANOTIFY_METADATA_VERSION)
                    {
                        throw std::runtime_error{"Mismatched event metadata version"};
                    }

                    event_callback(current_event);

                    current_event = FAN_EVENT_NEXT(current_event, bytes_read);
                }
                break;
            }
        });

        while (keep_monitoring)
        {
            (void)poller.wait_for(std::chrono::milliseconds(250));
        }

        // Read any pending signals from the signal pipe.
        char buffer[1024];
        (void)read(m_pipe_fd[0], buffer, sizeof(buffer));
    }

    void FileObserver::stop()
    {
        char signal_value = '1';
        (void)write(m_pipe_fd[1], &signal_value, sizeof(signal_value));
    }

} // namespace TF::Linux
