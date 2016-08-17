#include <TestSupport.h>
#include <Utils/License.h>

using namespace Passenger;
using namespace std;

namespace tut {
	struct LicenseTest {
	};

	void checkValid(int expectResult, string licensePart, int year, int month, int day) {
		struct tm dateTimeCheck;
		dateTimeCheck.tm_year = year - 1900;
		dateTimeCheck.tm_mon = month - 1;
		dateTimeCheck.tm_mday = day;

		char message[1024];
		snprintf(message, sizeof(message), "(validity) %s " " vs. %d-%02d-%02d (checkdate)", licensePart.c_str(), year, month, day);
		char *expiresAfter = copyExpiresAfterDate((char *) licensePart.c_str());
		ensure(expiresAfter != NULL);
		ensure_equals(message, compareDates(expiresAfter, dateTimeCheck), expectResult);
		free(expiresAfter);
	}

	DEFINE_TEST_GROUP(LicenseTest);

	TEST_METHOD(1) {
		set_test_name("license dates within expiration");

		checkValid(1, "Expires after: 2016-06-20\n", 2016, 6, 20);
		checkValid(1, "Expires after: 2016-06-20\n", 2016, 6, 19);
		checkValid(1, "Expires after: 2016-06-20\n", 2015, 6, 21);
		checkValid(1, "Expires after: 20161-01-01\n", 2016, 6, 20);
	}

	TEST_METHOD(2) {
		set_test_name("expired license dates");

		checkValid(0, "Expires after: 2016-06-20\n", 2016, 6, 21);
		checkValid(0, "Expires after: 2016-06-20\n", 2017, 6, 20);
		checkValid(0, "Expires after: 2016-06-20\n", 20161, 1, 1);
	}
}
