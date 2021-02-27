////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2021 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/ThreadImpl.hpp>
#include <SFML/System/Thread.hpp>
#include <iostream>
#include <cassert>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
ThreadImpl::ThreadImpl(Thread* owner) :
    m_thread(nullptr)
{
    m_thread = SDL_CreateThread(ThreadImpl::entryPoint, "", owner);

    if (!m_thread)
        std::cerr << "Failed to create thread" << std::endl;
}


////////////////////////////////////////////////////////////
void ThreadImpl::wait()
{
    if (m_thread)
    {
        SDL_WaitThread(m_thread, nullptr);
    }
}


////////////////////////////////////////////////////////////
void ThreadImpl::terminate()
{
    if (m_thread)
        SDL_DetachThread(m_thread);
}


////////////////////////////////////////////////////////////
int ThreadImpl::entryPoint(void* userData)
{
    // The Thread instance is stored in the user data
    Thread* owner = static_cast<Thread*>(userData);

    // Forward to the owner
    owner->run();

    return 0;
}

} // namespace priv

} // namespace sf
