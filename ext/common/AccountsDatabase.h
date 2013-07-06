/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */
#ifndef _PASSENGER_ACCOUNTS_DATABASE_H_
#define _PASSENGER_ACCOUNTS_DATABASE_H_

#include <string>
#include <map>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include "Account.h"
#include "ServerInstanceDir.h"
#include "StaticString.h"

/* This source file follows the security guidelines written in Account.h. */

namespace Passenger {

using namespace std;
using namespace boost;

class AccountsDatabase;
typedef shared_ptr<AccountsDatabase> AccountsDatabasePtr;


class AccountsDatabase {
private:
	mutable boost::mutex lock;
	map<string, AccountPtr> accounts;
	unsigned int uniqueNumber;
	
public:
	AccountsDatabase() {
		uniqueNumber = 0;
	}
	
	unsigned int size() const {
		boost::lock_guard<boost::mutex> l(lock);
		return (unsigned int) accounts.size();
	}
	
	vector<string> listUsernames() const {
		map<string, AccountPtr>::const_iterator it;
		vector<string> result;
		
		for (it = accounts.begin(); it != accounts.end(); it++) {
			result.push_back(it->second->getUsername());
		}
		return result;
	}
	
	AccountPtr add(const string &username, const string &passwordOrHash, bool hashGiven, int rights = Account::ALL) {
		AccountPtr account(new Account(username, passwordOrHash, hashGiven, rights));
		boost::lock_guard<boost::mutex> l(lock);
		accounts[username] = account;
		return account;
	}
	
	const AccountPtr get(const string &username) const {
		boost::lock_guard<boost::mutex> l(lock);
		map<string, AccountPtr>::const_iterator it = accounts.find(username);
		if (it == accounts.end()) {
			return AccountPtr();
		} else {
			return it->second;
		}
	}
	
	AccountPtr authenticate(const string &username, const StaticString &userSuppliedPassword) const {
		boost::lock_guard<boost::mutex> l(lock);
		map<string, AccountPtr>::const_iterator it = accounts.find(username);
		if (it == accounts.end()) {
			return AccountPtr();
		} else {
			AccountPtr account = it->second;
			if (account->checkPasswordOrHash(userSuppliedPassword)) {
				return account;
			} else {
				return AccountPtr();
			}
		}
	}
	
	bool remove(const string &username) {
		boost::lock_guard<boost::mutex> l(lock);
		map<string, AccountPtr>::iterator it = accounts.find(username);
		if (it == accounts.end()) {
			return false;
		} else {
			accounts.erase(it);
			return true;
		}
	}
	
	unsigned int getUniqueNumber() {
		boost::lock_guard<boost::mutex> l(lock);
		unsigned int result = uniqueNumber;
		uniqueNumber++;
		return result;
	}
};

} // namespace Passenger

#endif /* _PASSENGER_ACCOUNTS_DATABASE_H_ */
