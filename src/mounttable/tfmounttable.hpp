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

#ifndef TFMOUNTTABLE_HPP
#define TFMOUNTTABLE_HPP

#include <vector>
#include "TFFoundation.hpp"

using namespace TF::Foundation;

namespace TF::Linux
{

    struct MountTableEntry
    {
        using string_type = String;

        string_type file_system_name;
        string_type directory;
        string_type type;
        string_type options;
        int frequency;
        int pass_number;

        MountTableEntry() : file_system_name{}, directory{}, type{}, options{}, frequency{0}, pass_number{0} {}
    };

    using MountTable = std::vector<MountTableEntry>;

    MountTable load_mount_table();

} // namespace TF::Linux

#endif // TFMOUNTTABLE_HPP
