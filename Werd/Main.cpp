#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <limits>
#include <cctype>

bool ContainsWord(const std::vector<std::string>& vec, const std::string& word)
{
	for (const auto& w : vec)
	{
		if (w == word)
		{
			return true;
		}
	}

	return false;
}

std::vector<int> fill_buckets(const std::string& word)
{
	std::vector<int> buckets(26, 0);

	for (char c : word)
	{
		buckets[c - 'a']++;
	}

	return buckets;
}

int Score(const std::string& target, const std::string& guess)
{
	const std::vector<int> buckets1 = fill_buckets(target);
	const std::vector<int> buckets2 = fill_buckets(guess);

	int score = 0;
	for (int i = 0; i < 26; ++i)
	{
		score += std::min(buckets1[i], buckets2[i]);
	}

	for (int i = 0; i < 5; ++i)
	{
		if (target[i] == guess[i])
		{
			++score;
		}
	}

	return score;
}

int main()
{
	std::vector<std::string> textData;

	{
		std::ifstream textDataFile("sgb-words.txt");
		if (!textDataFile)
		{
			std::cout << "Couldn't load file! ";
			std::cin.get();
			return -1;
		}

		for (std::string line; std::getline(textDataFile, line); )
		{
			if (line.empty())
			{
				continue;
			}

			textData.push_back(line);
		}
	}

	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, textData.size() - 1);
	const std::string target = textData[dist(rng)];

	while (true)
	{
		std::string guess;
		std::cout << "Guess a five letter word: ";
		std::getline(std::cin, guess);
		for ( auto& c : guess )
		{
			c = std::tolower(c);
		}

		if (guess.size() != 5)
		{
			std::cout << "I said a FIVE letter word, asshole." << std::endl;
			continue;
		}
		else if (!ContainsWord(textData, guess))
		{
			std::cout << "The internet says that's not a word." << std::endl;
			continue;
		}

		const int score = Score(target, guess);
		if (score == 10)
		{
			std::cout << "You did it, great" << std::endl;
			break;
		}
		else
		{
			std::cout << "Ehhhhhh, you get a " << score << " womp womp" << std::endl;
			continue;
		}
	}

	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
	return 0;
}