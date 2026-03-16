// #include "Synthetik.h"

// using namespace Direktor;
// Direktor::HookLock::~HookLock() {
//     //! THIS SHOULD ONLY RUN ON PROGRAM EXIT. IF IT IS RUNNING ANY OTHER TIME WE HAVE A SERIOUS PROBLEM.
//     //? (notice only static instances of this class exist, and construct_in_place for the unique_lock)
//     if (m_UniqueLock)
//     {
//         int32_t retryCount = 0;
//         while (m_UniqueLock && (++retryCount < 20)) std::this_thread::sleep_for(std::chrono::milliseconds(25));
//         if (m_UniqueLock)
//             Log("ERROR: MUTEX STILL LOCKED WHILE GLOBAL DESTRUCT HAPPENING.");
//     }
//     auto outMutex = m_UniqueLock.release();
//     if (outMutex && outMutex != m_Mutex)
//     {
//         Log("HookLock mutex mismatch on destruction. This means we are doing an undefined behavior/memory leak."
//                 " Mutexes: this->%p, returned: %p", this->m_Mutex, outMutex);
//         free(outMutex);
//     }
//     if (m_Mutex)
//         free(m_Mutex);
// }
// bool* Direktor::HookLock::getLockRef() {
//     if (m_Mutex)
//         m_Locked = m_UniqueLock.operator bool();
//     return &m_Locked;
// }
// LockStatus Direktor::HookLock::lock() {
//     int retryCount = 0;
//     bool lockGot = false;
//     while (m_Mutex && ((++retryCount * 1000) < (LOCK_TIMEOUT_MS / 50))) {
//         std::this_thread::sleep_for(std::chrono::milliseconds(50));
//         if (m_UniqueLock.try_lock()) {
//             lockGot = true;
//             break;
//         }
//     }
//     if (m_Mutex && lockGot) {
//         return LockStatus::LOCK_OKAY;
//     }
//     else if (m_Mutex && m_UniqueLock) {
//         return LockStatus::LOCK_ALREADY_LOCKED;
//     }
//     return LockStatus::LOCK_UNKNOWN_ERROR;
// }
// LockStatus Direktor::HookLock::unlock() {
//     if (m_Mutex && m_UniqueLock) {
//         m_UniqueLock.unlock();
//         return LockStatus::LOCK_OKAY;
//     }
//     else if (m_Mutex && !m_UniqueLock) {
//         return LockStatus::LOCK_NOT_LOCKED;
//     }
//     return LockStatus::LOCK_UNKNOWN_ERROR;
// }