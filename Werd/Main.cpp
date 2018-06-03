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

void GameLoop(const std::vector<std::string>& textData)
{
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
			std::cout << "The internet says that's not a common word." << std::endl;
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
}

bool VecContains(const std::vector<std::string>& vec, const std::string& value)
{
	if (std::find(vec.begin(), vec.end(), value) != vec.end())
	{
		return true;
	}

	return false;
}

bool FileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

int main()
{
	std::vector<std::string> googleListTextData;
	std::vector<std::string> sgbTextData;

	if (!FileExists("combinedList.txt"))
	{
		std::cout << "One moment, setting up for the first time\n";
		std::cout << "...\n";
		{
			std::ifstream textDataFile("20k.txt");
			if (!textDataFile)
			{
				std::cout << "Couldn't load file! ";
				std::cin.get();
				return -1;
			}

			for (std::string line; std::getline(textDataFile, line); )
			{
				if (line.empty() || line.size() != 5)
				{
					continue;
				}

				googleListTextData.push_back(line);
			}
			std::cout << "...\n";
		}
		{
			std::ofstream combinedList("combinedList.txt");
			std::ifstream textDataFile("sgb-words.txt");
			if (!textDataFile)
			{
				std::cout << "Couldn't load file! ";
				std::cin.get();
				return -1;
			}

			for (std::string line; std::getline(textDataFile, line); )
			{
				if (line.empty() || !VecContains(googleListTextData, line))
				{
					continue;
				}

				sgbTextData.push_back(line);
				combinedList << line << '\n';
			}
			std::cout << "...\n";
		}
	}
	else
	{
		std::ifstream textDataFile("combinedList.txt");
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

			sgbTextData.push_back(line);
		}
	}

	GameLoop(sgbTextData);

	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
	return 0;
}