#include "rewind.h"
#include <string.h>
#include <stdio.h>
#include <malloc.h>

bool undo(void* data) {
	char* text = (char*)data;
	text[0] = 'H';
	text[5] = ',';
	return true;
}
bool redo(void* data) {
	char* text = (char*)data;
	text[0] = 'F';
	text[5] = 'w';
	return true;
}

int main() {
	Rewind::Buffer buffer;
	Rewind::SetBuffer(&buffer);

	const char* hello = "Hello, World!";
	char* test1 = (char*)malloc(strlen(hello) + 1);
	strcpy(test1, hello);

	printf("Check recording step\n");
	if (!Rewind::BeginRewindEntry("Replace 'Hello, World!' with 'Fellow World!'")) {
		return 1;
	}
	Rewind::RecordStep(undo, redo, test1);
	Rewind::EndRewindEntry(false);

	printf("Check redo step\n");
	if (!Rewind::Redo(1)) {
		printf("Redo fail\n");
		return 1;
	}
	if (strcmp(test1, "Fellow World!") != 0) {
		printf("strcmp fail\n");
		return 1;
	}

	printf("Check undo step\n");
	if (!Rewind::Undo(1)) {
		printf("Undo fail\n");
		return 1;
	}
	if (strcmp(test1, "Hello, World!") != 0) {
		printf("strcmp fail\n");
		return 1;
	}

	printf("Check complete\n");
	return 0;
}