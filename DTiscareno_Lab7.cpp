/*------------------------------------------------------*
*CREATED BY:											*
*Name: David Tiscareno									*
*Class: GSP480											*
*Week 7 Lab Assignment									*
*Date: 8/21/13											*
*DTiscareno_Lab7.cpp									*
*-------												*
*This file contains the main implementation	of a		*
*language parser test driver.							*
*-------------------------------------------------------*/
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>

struct BinaryDecisionTree
{
	BinaryDecisionTree* left;
	BinaryDecisionTree* right;
	BinaryDecisionTree* prev;
	int lastTried;
	bool badNode;
	void Init()
	{
		left = NULL;
		right = NULL;
		prev = NULL;
		lastTried = 1;
		badNode = false;
	}
	void Init(BinaryDecisionTree* previous)
	{
		left = NULL;
		right = NULL;
		prev = previous;
		lastTried = 1;
		badNode = false;
	}
	void CleanUp()
	{
		if (left != NULL)
			left->CleanUp();
		if (right != NULL)
			right->CleanUp();
		delete this;
	}
};

struct Bigram
{
	std::string first;
	std::string second;
	std::string bigramStr;
	bool full;
};

struct Trigram
{
	std::string first;
	std::string second;
	std::string third;
	std::string trigramStr;
	bool full;
};

Bigram createBigram(Bigram bi, std::string word);
Trigram createTrigram(Trigram tri, std::string word);
void addToMap(std::unordered_map<std::string, int>& wordmap, std::string word);
void SetBadNodes(BinaryDecisionTree* current);

int main()
{
	std::string line, word;
	bool selected = false;
	int inputNum;
	while (!selected)
	{
		std::cout << "Would you like to parse the corpus? (1 for yes, 2 for no): ";
		std::cin >> inputNum;
		std::cin.ignore();
		if (inputNum == 1 || inputNum == 2)
			selected = true;
	}

	if (inputNum == 1)
	{
		std::fstream corpus;
		float wordCount = 0;
		float biCount = 0;
		float triCount = 0;
		std::unordered_map<std::string, int> wordmap, bigramMap, trigramMap;
		Bigram bi;
		bi.full = false;
		Trigram tri;
		tri.full = false;

		std::cout << "Hold on a moment while we parse your corpus.";

		corpus.open("corpus.txt");

		if (corpus.is_open())
		{
			while (corpus.good())
			{
				getline(corpus, line);
				//Filter line
				for (int i = 0; i < line.size(); i++)
				{
					if ((line[i] >= 'A' && line[i] <= 'Z') ||
						(line[i] >= 'a' && line[i] <= 'z') ||
						line[i] == '.' || line[i] == '!' ||
						line[i] == '?' || line[i] == ' ')
					{
						if (line[i] == ' ')
						{
							if (word.size() != 0)
							{
								wordCount++;
								int count = wordCount;
								if (count % 20000 == 0)
									std::cout << " .";
								addToMap(wordmap, word);
								bi = createBigram(bi, word);
								if (bi.full)
								{
									biCount++;
									addToMap(bigramMap, bi.bigramStr);
								}
								tri = createTrigram(tri, word);
								if (tri.full)
								{
									triCount++;
									addToMap(trigramMap, tri.trigramStr);
								}
								word.resize(0);
							}
						}
						else if (line[i] == '.' ||
							line[i] == '!' ||
							line[i] == '?')
						{
							if (word.size() != 0)
							{
								wordCount++;
								int count = wordCount;
								if (count % 20000 == 0)
									std::cout << " .";
								addToMap(wordmap, word);
								bi = createBigram(bi, word);
								if (bi.full)
								{
									biCount++;
									addToMap(bigramMap, bi.bigramStr);
								}
								tri = createTrigram(tri, word);
								if (tri.full)
								{
									triCount++;
									addToMap(trigramMap, tri.trigramStr);
								}
								word.resize(0);
							}
							word.append(line, i, 1);
							wordCount++;
							int count = wordCount;
							if (count % 20000 == 0)
								std::cout << " .";
							addToMap(wordmap, word);
							bi = createBigram(bi, word);
							if (bi.full)
							{
								biCount++;
								addToMap(bigramMap, bi.bigramStr);
							}
							tri = createTrigram(tri, word);
							if (tri.full)
							{
								triCount++;
								addToMap(trigramMap, tri.trigramStr);
							}
							word.resize(0);
						}
						else
						{
							line[i] = tolower(line[i]);
							word.append(line, i, 1);
						}
					}
				}
				if (word.size() != 0)
				{
					wordCount++;
					int count = wordCount;
					if (count % 20000 == 0)
						std::cout << " .";
					addToMap(wordmap, word);
					bi = createBigram(bi, word);
					if (bi.full)
					{
						biCount++;
						addToMap(bigramMap, bi.bigramStr);
					}
					tri = createTrigram(tri, word);
					if (tri.full)
					{
						triCount++;
						addToMap(trigramMap, tri.trigramStr);
					}
					word.resize(0);
				}
			}
			corpus.close();
		}
		corpus.clear();
		line.resize(0);

		std::cout << std::endl << "Parsing complete!" << std::endl
			<< "Now hold on a moment, while we save the results.";

		std::ofstream output;
		output.open("wordcount.txt");
		if (output.is_open())
		{
			for (std::unordered_map<std::string, int>::iterator iter = wordmap.begin();
				iter != wordmap.end();
				iter++)
			{
				float num = iter->second;
				float percentage = num / wordCount;
				output << iter->first << ' '
					<< iter->second << ' '
					<< percentage << std::endl;
			}
			output.close();
		}
		output.clear();

		std::cout << " .";

		output.open("bicount.txt");
		if (output.is_open())
		{
			for (std::unordered_map<std::string, int>::iterator iter = bigramMap.begin();
				iter != bigramMap.end();
				iter++)
			{
				float num = iter->second;
				float percentage = num / biCount;
				output << iter->first << ' '
					<< iter->second << ' '
					<< percentage << std::endl;
			}
			output.close();
		}
		output.clear();

		std::cout << " .";

		output.open("tricount.txt");
		if (output.is_open())
		{
			for (std::unordered_map<std::string, int>::iterator iter = trigramMap.begin();
				iter != trigramMap.end();
				iter++)
			{
				float num = iter->second;
				float percentage = num / triCount;
				output << iter->first << ' '
					<< iter->second << ' '
					<< percentage << std::endl;
			}
			output.close();
		}
		output.clear();
		std::cout << std::endl << "Saving complete!" << std::endl << std::endl;
	}

	std::cout << "Welcome to the Word Parser Program created by David Tiscareno!" << std::endl;

	bool quit = false;
	std::unordered_map<std::string, int> unigramMap;
	std::ifstream inputFile;
	inputFile.open("wordcount.txt");
	if (!inputFile.is_open())
	{
		std::cout << "It appears you have not parsed the corpus! Restart and try again." << std::endl;
		system("pause");
		quit = true;
	}
	else
	{
		std::string frequency;
		while (inputFile.good())
		{
			frequency.resize(0);
			word.resize(0);
			line.resize(0);
			getline(inputFile, line);
			int spaceCount = 0;
			for (int i = 0; i < line.size() && spaceCount < 2; i++)
			{
				if (line[i] == ' ')
					spaceCount++;
				else if (spaceCount == 0)
					word.append(line, i, 1);
				else
					frequency.append(line, i, 1);
			}
			char freqChar[10];
			for (int i = 0; i < frequency.size(); i++)
				freqChar[i] = frequency[i];
			freqChar[frequency.size()] = '\0';
			int freqNum = atoi(freqChar);
			unigramMap.emplace(word, freqNum);
			int x = 0;
		}
	}
	inputFile.close();
	while (!quit)
	{
		std::cout << "Please input a sentence without spaces, capitals, or symbols . . ." << std::endl;
		std::string input;
		std::string newWord;
		line.resize(0);
		word.resize(0);
		std::getline(std::cin, input);
		for (int i = 0; i < input.size(); i++)
		{
			if ((input[i] >= 'A' && input[i] <= 'Z') ||
				(input[i] >= 'a' && input[i] <= 'z') ||
				input[i] == '.' || input[i] == '!' ||
				input[i] == '?')
			{
				input[i] = tolower(input[i]);
				line.append(input, i, 1);
			}
		}
		std::cout << std::endl << "Your sentence: " << line << std::endl;

		bool foundAnswer = false;
		std::string answer;

		std::cout << std::endl << "Hmmm...let me think..." << std::endl << std::endl;
		BinaryDecisionTree* tree;
		tree = new BinaryDecisionTree;
		BinaryDecisionTree* current;
		tree->Init();
		current = tree;
		while (!foundAnswer)
		{
			word.resize(0);
			answer.resize(0);
			if (tree->badNode)
			{
				tree->CleanUp();
				tree = new BinaryDecisionTree;
				tree->Init();
				newWord = line[line.size() - 1] + newWord;
				line.resize(line.size() - 1);
			}
			current = tree;
			for (int i = 0; i < line.size(); i++)
			{
				word.append(line, i, 1);
				std::unordered_map<std::string, int>::const_iterator search = unigramMap.find(word);
				if (search != unigramMap.end())
				{	
					if (current->lastTried == 1 && (current->left!= NULL && !current->left->badNode) ||
						current->lastTried == 1 && current->left == NULL ||
						current->lastTried == 0 && (current->right != NULL && current->right->badNode))
					{
						current->lastTried = 0;
						if (current->left == NULL)
						{
							current->left = new BinaryDecisionTree;
							current->left->Init(current);
						}
						current = current->left;
						answer += (word + ' ');
						word.resize(0);
					}
					else if (current->lastTried == 0 && (current->right != NULL && !current->right->badNode) ||
						current->lastTried == 0 && current->right == NULL ||
						current->lastTried == 1 && (current->left != NULL && current->left->badNode))
					{
						current->lastTried = 1;
						if (current->right == NULL)
						{
							current->right = new BinaryDecisionTree;
							current->right->Init(current);
						}
						current = current->right;

					}
				}
			}
			if (word.size() == 0)
				foundAnswer = true;
			else
			{
				current->badNode = true;
				if (current->prev != NULL)
					SetBadNodes(current->prev);
			}
		}
		tree->CleanUp();

		std::cout << "I believe you were trying to say: " << answer;
		if (newWord.size() != 0)
			std::cout << newWord << std::endl
				<< "You entered a word which was not in our corpus: " << newWord << std::endl;
		selected = false;
		while (!selected)
		{
			std::cout << std::endl << "Would you like to try again? (1 for yes, 2 for no):";
			std::cin >> inputNum;
			std::cin.ignore();
			if (inputNum == 1 || inputNum == 2)
				selected = true;
		}
		if (inputNum == 2)
			quit = true;
	}
	std::cout << std::endl << "Thank you for playing. Please wait, while we shut down.";
}

Bigram createBigram(Bigram bi, std::string word)
{
	if (bi.full)
	{
		bi.first = bi.second;
		bi.second = word;
		bi.bigramStr = bi.first + ' ' + bi.second;
	}
	else if (bi.first.size() != 0)
	{
		bi.second = word;
		bi.full = true;
		bi.bigramStr = bi.first + ' ' + bi.second;
	}
	else
		bi.first = word;
	return bi;
}

Trigram createTrigram(Trigram tri, std::string word)
{
	if (tri.full)
	{
		tri.first = tri.second;
		tri.second = tri.third;
		tri.third = word;
		tri.trigramStr = tri.first + ' ' + tri.second + ' ' + tri.third;
	}
	else if (tri.first.size() != 0 &&
			 tri.second.size() != 0)
	{
		tri.third = word;
		tri.full = true;
		tri.trigramStr = tri.first + ' ' + tri.second + ' ' + tri.third;
	}
	else if (tri.first.size() != 0)
		tri.second = word;
	else
		tri.first = word;
	return tri;
}

void addToMap(std::unordered_map<std::string, int>& wordmap, std::string word)
{
	std::unordered_map<std::string, int>::const_iterator search = wordmap.find(word);
	if (search == wordmap.end())
		wordmap.emplace(word, 1);
	else
		wordmap[word] += 1;
}

void SetBadNodes(BinaryDecisionTree* current)
{
	BinaryDecisionTree* currentTemp = current;
	if (currentTemp->left != NULL && currentTemp->right != NULL)
	{
		if (currentTemp->left->badNode && currentTemp->right->badNode)
		{
			currentTemp->badNode = true;
			if (currentTemp->prev != NULL)
				SetBadNodes(currentTemp->prev);
		}
	}
}