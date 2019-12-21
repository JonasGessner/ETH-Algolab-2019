//
//  evolution.cpp
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
#include <map>
#include <unordered_map>

using namespace std;

typedef struct Species_ Species;

struct Species_
{
	string name;
	int age;
	vector<const Species *> children;
};

string binarySearchAncestor(int maxAge, vector<const Species *> &ancestors)
{
	int l = 0;
	int r = ancestors.size() - 1;
	int find = ancestors.size() - 1;

	while (l < r)
	{
		int mid = (l + r) / 2;

		if (ancestors.at(mid)->age > maxAge)
		{
			l = mid + 1;
		}
		else
		{
			find = mid;
			r = mid;
		}
	}

	return ancestors.at(find)->name;
}

void dfs(const Species *current, vector<const Species *> &ancestors, const unordered_map<string, vector<pair<int, int>>> &queries, vector<string> &res)
{
	ancestors.push_back(current);

	if (queries.find(current->name) != queries.end())
	{
		const vector<pair<int, int>> &q = queries.at(current->name);

		for (const auto &query : q)
		{
			const string name = binarySearchAncestor(query.first, ancestors);
			res.at(query.second) = name;
		}
	}

	for (const Species *child : current->children)
	{
		dfs(child, ancestors, queries, res);
	}

	ancestors.pop_back();
}

void testcase()
{
	int n, q;
	cin >> n >> q;

	unordered_map<string, Species> species;
	species.reserve(n);

	Species *root = NULL;
	int rootAge = 0;

	for (int i = 0; i < n; i++)
	{
		string name;
		int age;
		cin >> name >> age;

		Species s;
		s.name = name;
		s.age = age;

		species[name] = s;

		if (age > rootAge)
		{
			rootAge = age;
			root = &species[name];
		}
	}

	for (int i = 0; i < n - 1; i++)
	{
		string child;
		string parent;
		cin >> child >> parent;

		species.at(parent).children.push_back(&species.at(child));
	}

	unordered_map<string, vector<pair<int, int>>> queries;
	queries.reserve(q);

	for (int i = 0; i < q; i++)
	{
		string name;
		int age;
		cin >> name >> age;

		if (queries.find(name) == queries.end())
		{
			vector<pair<int, int>> empty;
			queries[name] = empty;
		}

		queries.at(name).push_back({age, i});
	}

	vector<string> res(q);

	vector<const Species *> empty;
	dfs(root, empty, queries, res);

	string final;
	for (int i = 0; i < q; i++)
	{
		final.append(res.at(i));
		if (i < q - 1)
		{
			final.append(" ");
		}
	}

	cout << final << "\n";
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
