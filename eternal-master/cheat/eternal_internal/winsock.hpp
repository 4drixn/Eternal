#pragma once

#include "main.hpp"
#include "FakeLag.hpp"
#include <chrono>
#include <thread>

typedef int(WINAPI* WSArecv_)(SOCKET, LPWSABUF, DWORD, LPDWORD, LPDWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);
typedef int(WINAPI* WSAsend_)(SOCKET, LPWSABUF, DWORD, LPDWORD, DWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);

namespace hooks {
    inline WSArecv_ WSAReceive;
    inline WSAsend_ _WSASend_ = 0;

    __forceinline int WINAPI WSAReceiveHook(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
    {

        if (fakelag::inbound::m_enabled == true || fakelag::should_do_it == true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(fakelag::delay));

            return WSAReceive(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
        }
        else {

            return WSAReceive(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
        }
        return 0;
    }

    __forceinline int WINAPI WSASendHook(
        SOCKET                             s,
        LPWSABUF                           lpBuffers,
        DWORD                              dwBufferCount,
        LPDWORD                            lpNumberOfBytesSent,
        DWORD                              dwFlags,
        LPWSAOVERLAPPED                    lpOverlapped,
        LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
    )
    {
        if (fakelag::outbound::m_enabled || fakelag::should_do_it) {

            std::this_thread::sleep_for(std::chrono::milliseconds(fakelag::delay));
            return _WSASend_(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
        }
        else {

            return _WSASend_(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
        }

    }
};