//
//  moving_books.cpp
//  Algorithms Lab
//
//  Created by Jonas Gessner
//  Copyright © 2019 Jonas Gessner. All rights reserved.
//

#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

void testcase()
{
	int friendCount, boxCount;
	cin >> friendCount >> boxCount;

	int strongestFriend = 0;
	vector<int> friends(friendCount);
	for (int i = 0; i < friendCount; i++)
	{
		int x;
		cin >> x;
		friends.at(i) = x;

		strongestFriend = max(strongestFriend, x);
	}

	int heaviestBox = 0;
	multiset<int, greater<int>> boxes;
	for (int i = 0; i < boxCount; i++)
	{
		int x;
		cin >> x;
		boxes.insert(x);

		heaviestBox = max(heaviestBox, x);
	}

	if (strongestFriend < heaviestBox)
	{
		cout << "impossible\n";
		return;
	}

	sort(friends.begin(), friends.end());

	int trip = 0;
	while (!boxes.empty())
	{
		for (int f = friends.size() - 1; f >= 0; f--)
		{
			int liftBra = friends.at(f);

			const auto lB = boxes.lower_bound(liftBra); // Comparator for the multiset is > not <. Hence, this returns the first value that does not evaluate greater than liftBra
			if (lB != boxes.end())
			{
				boxes.erase(lB);
			}
			else
			{
				friends.erase(friends.begin() + f);
			}
		}

		trip++;
	}

	cout << (trip - 1) * 3 + 2 << "\n";
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
