#include "Synthetik.h"
using namespace Direktor;

RValueLock::~RValueLock() {
	//! THIS SHOULD ONLY RUN ON PROGRAM EXIT. IF IT IS RUNNING ANY OTHER TIME WE HAVE A SERIOUS PROBLEM.
	int32_t retryCount = 0;
	LockStatus lastStatus = LockStatus::LOCK_UNKNOWN_ERROR;
	while ((lastStatus = this->lock()) != LockStatus::LOCK_OKAY && (++retryCount <= 200))
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	
	if (lastStatus != LockStatus::LOCK_OKAY)
	{
		Log("ERROR: RVALUEMUTEX STILL LOCKED WHILE GLOBAL DESTRUCT HAPPENING.");
		this->unlock();
	}
	this->lock();
	int32_t count = ::Direktor::s_InstalledHooks().size();
	for (size_t i = 0; i < ::Direktor::s_InstalledHooks().size(); i++)
	{
		if (s_InstalledHooks()[i])
		{
			::Direktor::s_InstalledHooks()[i]->Uninstall();
			::Direktor::s_InstalledHooks()[i]->~GMHook();
			free(::Direktor::s_InstalledHooks()[i]);
			::Direktor::s_InstalledHooks()[i] = nullptr;
		}
	}
	::Direktor::s_InstalledHooks().clear();
	LockStatus unlockStatus = this->unlock();
	if (unlockStatus != LockStatus::LOCK_OKAY)
		Log("GLOBAL DESTRUCTOR ERROR: RVALUEMUTEX UNLOCK STATUS: %d AFTER UNLOCKING", (int32_t)unlockStatus);
}
bool* RValueLock::getLockRef() {
	static bool locked = false;
	return &locked;	
}

LockStatus RValueLock::lock() {
	if (!*getLockRef()) {
		LOCK_RVALUE_MUTEX();
		(*getLockRef() = true);
		return LockStatus::LOCK_OKAY;
	}
	return LockStatus::LOCK_ALREADY_LOCKED;
}
LockStatus RValueLock::unlock() {
	if (*getLockRef()) {
		UNLOCK_RVALUE_MUTEX();
		(*getLockRef() = false);
		return LockStatus::LOCK_OKAY;
	}
	return LockStatus::LOCK_NOT_LOCKED;
}

