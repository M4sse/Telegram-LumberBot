#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "User32.lib")

#include <iostream>
#include <windows.h>

using namespace std;

bool hasBranchAt(int, int);
int* getNextMoves(int);

// Bottom to top
const int BRANCH_HEIGHTS[] = { 640, 540, 440, 340, 240, 140, 40 };
const int BRANCH_LEFT = 248;
const int BRANCH_RIGHT = 357;

HDC dc;

int main(int argc, char** argv) {
	dc = GetDC(NULL);

	// Tests
	COLORREF color = GetPixel(dc, 248, 642);

	int red = GetRValue(color);
	int green = GetGValue(color);
	int blue = GetBValue(color);

	cout << "red:\t"   << red << "\n"
	     << "green:\t" << green << "\n"
	     << "blue:\t"  << blue << endl;
	
	cout << "hasLeftFirst: " << hasBranchAt(BRANCH_LEFT, BRANCH_HEIGHTS[0]) << endl;
	cout << "hasRightFirst: " << hasBranchAt(BRANCH_RIGHT, BRANCH_HEIGHTS[0]) << endl;


	// Actual Bot
	cout << "Wait 2 Seconds" << endl;
	Sleep(2000);

	int score = 0;
	int currentVK = VK_LEFT;

	while (score < 3000000) { // Only checks every 12 moves
		
		const int MOVE_COUNT = 12;
		cout << "Reading" << endl;
		int* moves = getNextMoves(MOVE_COUNT);
		for (int i = 0; i < MOVE_COUNT; i++) {

			keybd_event(moves[i], 0, 0, 0);
			keybd_event(moves[i], 0, KEYEVENTF_KEYUP, 0);

			cout << moves[i] << endl;
			score++;
			Sleep(13);
		}
		delete[] moves;

		cout << "Waiting for App" << endl;	
		// Timer is to fast anyway, so we can at least try
		if (score == 840) {
			Sleep(50);
			continue;
		}
		Sleep(198);

	}

	cout << "Sorry I can not score better than my master!" << endl;
	ReleaseDC(NULL, dc);
	return 0;
}

// Only Even counts plox
int* getNextMoves(int count) {
	int currentVK = VK_LEFT;
	int* moves = new int[count];

	for (int i = 0; i < count; i++) {
		if (hasBranchAt(BRANCH_LEFT, BRANCH_HEIGHTS[i / 2]) && currentVK == VK_LEFT) {
                        currentVK = VK_RIGHT;
                } else if (hasBranchAt(BRANCH_RIGHT, BRANCH_HEIGHTS[i / 2]) && currentVK == VK_RIGHT) {
                        currentVK = VK_LEFT;
                }

		moves[i] = currentVK;
		moves[++i] = currentVK;
	}

	return moves;
}

bool hasBranchAt(int lOrR, int height) {
	COLORREF color = GetPixel(dc, lOrR, height);
	return GetRValue(color) == 161;
}
