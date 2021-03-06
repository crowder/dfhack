/*
https://github.com/peterix/dfhack
Copyright (c) 2009-2012 Petr Mrázek (peterix@gmail.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/
/* Based on luafilesystem
Copyright © 2003-2014 Kepler Project.

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <string>

#include "modules/Filesystem.h"

bool DFHack::Filesystem::chdir (std::string path)
{
    return !(bool)::chdir(path.c_str());
}

std::string DFHack::Filesystem::getcwd ()
{
    char *path;
    char buf[LFS_MAXPATHLEN];
    std::string result = "";
#ifdef _WIN32
    if ((path = ::_getcwd(buf, LFS_MAXPATHLEN)) != NULL)
#else
    if ((path = ::getcwd(buf, LFS_MAXPATHLEN)) != NULL)
#endif
        result = buf;
    return result;
}

bool DFHack::Filesystem::mkdir (std::string path)
{
    int fail;
#ifdef _WIN32
    fail = ::_mkdir(path.c_str());
#else
    fail = ::mkdir(path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP |
                   S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH);
#endif
    return !(bool)fail;
}

bool DFHack::Filesystem::rmdir (std::string path)
{
    int fail;
#ifdef _WIN32
    fail = ::_rmdir(path.c_str());
#else
    fail = ::rmdir(path.c_str());
#endif
    return !(bool)fail;
}

#ifdef _WIN32
_filetype mode2type (unsigned short mode) {
#else
_filetype mode2type (mode_t mode) {
#endif
    if (S_ISREG(mode))
        return FILETYPE_FILE;
    else if (S_ISDIR(mode))
        return FILETYPE_DIRECTORY;
    else if (S_ISLNK(mode))
        return FILETYPE_LINK;
    else if (S_ISSOCK(mode))
        return FILETYPE_SOCKET;
    else if (S_ISFIFO(mode))
        return FILETYPE_NAMEDPIPE;
    else if (S_ISCHR(mode))
        return FILETYPE_CHAR_DEVICE;
    else if (S_ISBLK(mode))
        return FILETYPE_BLOCK_DEVICE;
    else
        return FILETYPE_UNKNOWN;
}

bool DFHack::Filesystem::stat (std::string path, STAT_STRUCT &info)
{
    return !(bool)(STAT_FUNC(path.c_str(), &info));
}

bool DFHack::Filesystem::exists (std::string path)
{
    STAT_STRUCT info;
    return (bool)DFHack::Filesystem::stat(path.c_str(), info);
}

#include <iostream>
_filetype DFHack::Filesystem::filetype (std::string path)
{
    STAT_STRUCT info;
    DFHack::Filesystem::stat(path, info);
    return mode2type(info.st_mode);
}

bool DFHack::Filesystem::isfile (std::string path)
{
    return DFHack::Filesystem::filetype(path) == FILETYPE_FILE;
}

bool DFHack::Filesystem::isdir (std::string path)
{
    return DFHack::Filesystem::filetype(path) == FILETYPE_DIRECTORY;
}
