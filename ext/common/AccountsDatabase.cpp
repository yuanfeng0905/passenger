/*
 *  Phusion Passenger - http://www.modrails.com/
 *  Copyright (c) 2010 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include "AccountsDatabase.h"
#include "RandomGenerator.h"
#include "Exceptions.h"
#include "Constants.h"
#include "Utils.h"

namespace Passenger {

AccountsDatabasePtr
AccountsDatabase::createDefault(const ServerInstanceDir::GenerationPtr &generation,
                                bool userSwitching, const string &defaultUser,
                                const string &defaultGroup)
{
	AccountsDatabasePtr database(new AccountsDatabase());
	struct passwd *defaultUserEntry;
	struct group  *defaultGroupEntry;
	uid_t defaultUid;
	gid_t defaultGid;
	RandomGenerator random;
	string passengerStatusPassword = random.generateByteString(MESSAGE_SERVER_MAX_PASSWORD_SIZE);
	
	defaultUserEntry = getpwnam(defaultUser.c_str());
	if (defaultUserEntry == NULL) {
		throw NonExistentUserException("Default user '" + defaultUser +
			"' does not exist.");
	}
	defaultUid = defaultUserEntry->pw_uid;
	defaultGroupEntry = getgrnam(defaultGroup.c_str());
	if (defaultGroupEntry == NULL) {
		throw NonExistentGroupException("Default group '" + defaultGroup +
			"' does not exist.");
	}
	defaultGid = defaultGroupEntry->gr_gid;
	
	// An account for the 'passenger-status' command. Its password is only readable by
	// root, or (if user switching is turned off) only by the web server's user.
	database->add("_passenger-status", passengerStatusPassword, false,
		Account::INSPECT_BASIC_INFO | Account::INSPECT_SENSITIVE_INFO |
		Account::INSPECT_BACKTRACES);
	if (geteuid() == 0 && !userSwitching) {
		createFile(generation->getPath() + "/passenger-status-password.txt",
			passengerStatusPassword, S_IRUSR, defaultUid, defaultGid);
	} else {
		createFile(generation->getPath() + "/passenger-status-password.txt",
			passengerStatusPassword, S_IRUSR | S_IWUSR);
	}
	
	return database;
}

} // namespace Passenger
