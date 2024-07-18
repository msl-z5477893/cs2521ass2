#include <stdbool.h>
#include <stdlib.h>

#include "Bookmark.h"

Bookmark BookmarkInit(size_t count) {
	Bookmark bm = {.count = count, .bookmarks = malloc(sizeof(bool) * count)};
	for (size_t i = 0; i < count; i++) {
		bm.bookmarks[i] = false;
	}
	return bm;
}

bool BookmarkMark(Bookmark bm, size_t index) {
	if (index >= bm.count || bm.bookmarks[index]) {
		return false;
	}
	bm.bookmarks[index] = true;
	return true;
}

void BookmarkFree(Bookmark bm) {
	free(bm.bookmarks);
}
