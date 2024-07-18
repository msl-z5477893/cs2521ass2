#ifndef Network_h_INCLUDED
#define Network_h_INCLUDED

#include "poodle.h"

struct network {
	int computerCount;
	int connectionCount;
	struct connection *connections;
	struct computer *computers;
};

#endif  // Network_h_INCLUDED
