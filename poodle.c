// poodle.c
// Assignment 2 main source file.
// completed by Michael Stephen Lape (z5477893)

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Bookmark.h"
#include "poodle.h"

////////////////////////////////////////////////////////////////////////
// Task 1

/*
 * given a path of computers, assess that it works and the time taken
 * to probe it
 *
 * STATUS: UNOPTIMISED (slowed on challenge tasks)
 *
 * - Fails challenge test for the following reason:
 * - current computer and connection lookup time is O(n), which is very slow
 *   for challenge times
 */
struct probePathResult probePath(struct computer computers[], int numComputers,
                                 struct connection connections[],
                                 int numConnections, int path[],
                                 int pathLength) {
	struct probePathResult res = {SUCCESS, 0};
	Bookmark vists = BookmarkInit(numComputers);

	// keep track of the final computer reached.
	int finalComp = path[0];
	for (int ix = 0; ix < pathLength - 1; ix++) {
		int compFrom = path[ix];
		int compTo = path[ix + 1];
		if (compFrom == compTo) {
			continue;
		}
		struct connection *currentConn = NULL;
		// check if connection exists between chosen computers.
		for (int connIx = 0; connIx < numConnections; connIx++) {
			bool fSide = (connections[connIx].computerA == compFrom &&
			              connections[connIx].computerB == compTo);
			bool bSide = (connections[connIx].computerB == compFrom &&
			              connections[connIx].computerA == compTo);
			if (fSide || bSide) {
				currentConn = &connections[connIx];
			}
		}

		if (currentConn == NULL) {
			// fprintf(stderr, "Cannot find path: %d -> %d\n", compFrom, compTo);
			res.status = NO_CONNECTION;
			break;
		}

		// source computer with security level X can sand data to computers with
		// security level X + 1 or smaller
		// so to check for invalidity we just check if dest computer has security
		// level is strictly higher than the security of the source computer + 1.
		if (computers[compTo].securityLevel >
		    computers[compFrom].securityLevel + 1) {
			res.status = NO_PERMISSION;
			break;
		}

		// if current path is valid then we can count the time to the total
		if (BookmarkMark(vists, compFrom)) {
			// printf("Computer %d marked!\n", compFrom);
			res.elapsedTime += computers[compFrom].poodleTime;
		}
		res.elapsedTime += currentConn->transmissionTime;
		// printf("CURRENTLY CALCULATED TIME: %d\n", res.elapsedTime);

		finalComp = compTo;
	}

	// if we didn't encounter issues the entire probe, account the poodle time of
	// final destination
	// if (res.status == SUCCESS) {
	// 	res.elapsedTime += computers[path[pathLength - 1]].poodleTime;
	// }
	res.elapsedTime += computers[finalComp].poodleTime;
	BookmarkFree(vists);
	// printf("ELAPSED TIME: %d\n", res.elapsedTime);

	return res;
}

////////////////////////////////////////////////////////////////////////
// Task 2

struct chooseSourceResult chooseSource(struct computer computers[],
                                       int numComputers,
                                       struct connection connections[],
                                       int numConnections) {
	// NOTE: This structure returns the following
	// int sourceComputer: number id of computer
	// int numComputers: number of vulnerable computers
	// int []computers: id of vulnerable computers.
	struct chooseSourceResult res = {0, 0, NULL};

	return res;
}

////////////////////////////////////////////////////////////////////////
// Task 3

struct poodleResult poodle(struct computer computers[], int numComputers,
                           struct connection connections[], int numConnections,
                           int sourceComputer) {
	struct poodleResult res = {0, NULL};

	return res;
}

////////////////////////////////////////////////////////////////////////
// Task 4

/**
 * Describe your solution in detail here:
 *
 * TODO
 */
struct poodleResult advancedPoodle(struct computer computers[],
                                   int numComputers,
                                   struct connection connections[],
                                   int numConnections, int sourceComputer) {
	struct poodleResult res = {0, NULL};

	return res;
}

