// Bookmark.h
// Create a bookmark associated with a list structure.

#ifndef Bookmark_h_INCLUDED
#define Bookmark_h_INCLUDED

#include <stdbool.h>
#include <stdlib.h>

struct bookmark {
	bool *bookmarks;
	size_t count;
};

typedef struct bookmark Bookmark;

/*
 * Create a list of bookmarks corresponding to a list structure.
 * */
Bookmark BookmarkInit(size_t count);

/*
 * Set a bookmark, returns false if bookmark is already set
 * or item in related structure does not exist. 
 * */
bool BookmarkMark(Bookmark bm, size_t index);

/*
 * Deinitialise or free a bookmark.
 * */

void BookmarkFree(Bookmark bm);

#endif  // Bookmark_h_INCLUDED
