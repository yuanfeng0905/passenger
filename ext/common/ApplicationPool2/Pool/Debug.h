/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2011-2015 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  See LICENSE file for license information.
 */

// This file is included inside the Pool class.

protected:

struct DebugSupport {
	/** Mailbox for the unit tests to receive messages on. */
	MessageBoxPtr debugger;
	/** Mailbox for the ApplicationPool code to receive messages on. */
	MessageBoxPtr messages;

	// Choose aspects to debug.
	bool restarting;
	bool spawning;
	bool oobw;
	bool testOverflowRequestQueue;
	bool detachedProcessesChecker;
	bool rollingRestarting;

	// The following fields may only be accessed by Pool.
	boost::mutex syncher;
	unsigned int spawnLoopIteration;
	unsigned int spawnErrors;

	DebugSupport() {
		debugger = boost::make_shared<MessageBox>();
		messages = boost::make_shared<MessageBox>();
		restarting = true;
		spawning   = true;
		oobw       = false;
		detachedProcessesChecker = false;
		testOverflowRequestQueue = false;
		spawnLoopIteration = 0;
		rollingRestarting = false;
		spawnErrors = 0;
	}
};

typedef boost::shared_ptr<DebugSupport> DebugSupportPtr;

DebugSupportPtr debugSupport;
