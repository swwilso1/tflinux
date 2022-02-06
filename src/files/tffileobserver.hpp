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

#ifndef TFFILEOBSERVER_HPP
#define TFFILEOBSERVER_HPP

#include <functional>
#include <sys/fanotify.h>
#include "TFFoundation.hpp"

using namespace TF::Foundation;

namespace TF::Linux
{

    class FileObserver
    {
    public:
        using string_type = String;
        using event_metadata_type = struct fanotify_event_metadata;

        FileObserver(unsigned int flags, unsigned int modes);

        ~FileObserver();

        void mark(const string_type & path, uint32_t flags, uint64_t mask) const;

        void mark(uint32_t flags, uint64_t mask, int dirfd, const string_type & path) const;

        void run(const std::function<void(event_metadata_type *)> & event_callback);

        void stop();

    private:
        int m_notifier_fd;
        int m_pipe_fd[2];

        const static int EVENT_BUFFER_SIZE = 100;
    };

} // namespace TF::Linux

#endif // TFFILEOBSERVER_HPP
