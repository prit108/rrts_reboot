#include <iostream>
#include <string>
using namespace std;

#include "user.hpp"
#include "mayor.hpp"
#include "complaint.hpp"
#include "city.hpp"
#include "area.hpp"
#include "road.hpp"

string Mayor::GetUserType() {
	return string("Mayor");
}