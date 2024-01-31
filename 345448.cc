// Luca JIMENEZ GLUR
// 345448
// Project SAME GRANMA v1.0

#include <iostream>
#include <string>
#include <vector>
using namespace std;

const string NOT_IN_CAPITAL_LETTERS("The word is not purely in capital letters");
const string DOUBLE_WORD("Each word can be present only once");
const string EMPTY_DICO("The dictionnary cannot be empty");
const string NO_ANAGRAM("There is no anagram for this message and this dictionary");

struct Mot
{
	string vanilla; 		// Original word
	string alpha;			// Word sorted in alpabetical order
	size_t nbT;				// Word length
	size_t nbD;				// Number of different letters
};

using Dictionary = vector<Mot>;

void read_dico(Dictionary& dico);
bool all_caps(string word);
string make_alpha(string vanilla);
bool new_word(string word, Dictionary& dico);
size_t count_distinct_letters(string word);
bool bad_order(Mot& word1, Mot& word2);
void sort_dico(Dictionary& dico, size_t& nbM);
void display_dico(const Dictionary& dico, size_t& nbM);
void research_loop(Dictionary& dico, size_t& nbM);
string read_message();
bool find_anagrams(Dictionary& dico, string alpha_m,
				   vector<string>& anagram, size_t& nbM);
bool message_contains_word(string& alpha, string& alpha_m);
string message_minus_word(string alpha_m, string& alpha);
void dico_erase_word(Dictionary& dico, size_t& pos, size_t& nbM);
void dico_insert_word(Dictionary& dico, size_t& pos, Mot& word, size_t& nbM);

int main()
{
	Dictionary dico;
	read_dico(dico);
	size_t nbM(dico.size());
	sort_dico(dico, nbM);
	display_dico(dico, nbM);
	research_loop(dico, nbM);
	return 0;
}

void read_dico(Dictionary& dico)
{
	string word;
	cin >> word;
	
	while (word != ".")
	{
		if (all_caps(word))
		{
			if (new_word(word, dico))
			{
				dico.push_back({word, make_alpha(word), word.size(), 
								count_distinct_letters(word)});
			} else
			{
				cout << DOUBLE_WORD << "\n";
				exit(0);
			}
		} else
		{
			cout << NOT_IN_CAPITAL_LETTERS << "\n";
			exit(0);
		}
		cin >> word;
	}
	
	if (dico.size() == 0)
	{
		cout << EMPTY_DICO << "\n";
		exit(0);
	}
}

bool all_caps(string word)
{
	for (size_t i(0); i < word.size(); ++i)
	{
        if (not (word[i] >= 'A' and word[i] <= 'Z'))
			return false;
    }
	return true;
}

string make_alpha(string vanilla)
{
	string alpha = vanilla;
	for(size_t j(0) ; j < alpha.size()-1 ; j++)
	{
		for(size_t k(0) ; k < alpha.size()-1-j ; k++)
		{
			if (alpha[k] > alpha[k+1])
			{
				char temp = alpha[k];
				alpha[k] = alpha [k+1];
				alpha[k+1] = temp;
			}
		}
	}
	return alpha;
}

bool new_word(string word, Dictionary& dico)
{
	for (size_t i(0); i < dico.size(); ++i)
	{
		if (word == dico[i].vanilla)
			return false;
	}
	return true;
}

size_t count_distinct_letters(string word)
{
	int result(1);
	for (size_t i(1); i < word.size(); ++i)
	{
		++result;
		int j(i-1);
		while (j >= 0 and word[i] != word[j])
			--j;
		if (word[i] == word[j])
			--result;
	}
	return result;
}

bool bad_order(Mot& word1, Mot& word2)
{
	if (word1.nbT != word2.nbT)
		return (word1.nbT > word2.nbT);
		
	if (word1.nbD != word2.nbD)
		return (word1.nbD > word2.nbD);
	
	if (word1.alpha != word2.alpha)
		return (word1.alpha > word2.alpha);
	
	return (word1.vanilla > word2.vanilla);
}

void sort_dico(Dictionary& dico, size_t& nbM)
{
	for(size_t i(0) ; i < nbM-1 ; i++)
	{
		for(size_t j(0) ; j < nbM-1-i ; j++)
		{
			if (bad_order(dico[j], dico[j+1]))
			{
				Mot temp = dico[j];
				dico[j] = dico[j+1];
				dico[j+1] = temp;
			}
		}
	}
}

void display_dico(const Dictionary& dico, size_t& nbM)
{
	for (size_t i(0); i < nbM; ++i)
		cout << dico[i].vanilla << "\n";
}

void research_loop(Dictionary& dico, size_t& nbM)
{
	string message(read_message());
	while (message != "*")
	{
		vector<string> anagram;
		anagram.reserve(nbM);
		cout << "\n";
		if (not (message == ""))
		{
			message = make_alpha(message);
			if (not find_anagrams(dico, message, anagram, nbM))
				cout << NO_ANAGRAM << "\n";		
		}
		message = read_message();
	}
	exit(0);
}

string read_message()
{
	string word;
	cin >> word;
	string message;
	
	if (word == "*") 
		return word;
		
	while (word != ".")
	{
		if (all_caps(word))
		{
			message += word;
		} else
		{
			cout << "\n" << NOT_IN_CAPITAL_LETTERS;
			message = "";
		}
		cin >> word;
	}
	
	return message;
}

bool find_anagrams(Dictionary& dico, string alpha_m,
				   vector<string>& anagram, size_t& nbM)
{
	if (nbM == 0) return false;
	bool success(false);
	string to_be_displayed("");
	for (size_t i(0); i < nbM; ++i)
	{
		if (message_contains_word(dico[i].alpha, alpha_m))
		{
			vector<string> anag_next;
			anag_next.swap(anagram);
			anag_next.push_back(dico[i].vanilla);
			string alpha_m_next = message_minus_word(alpha_m, dico[i].alpha);
			if (alpha_m_next.size() == 0)
			{
				for (size_t j(0); j < anag_next.size()-1; ++j)
					to_be_displayed += anag_next[j] + " ";
				to_be_displayed += anag_next[anag_next.size()-1] + "\n";
				success = true;
			} else
			{
				Mot temp = dico[i];
				dico_erase_word(dico, i, nbM);
				Dictionary dico_next;
				dico_next.swap(dico);
				--nbM;
				if (find_anagrams(dico_next, alpha_m_next, anag_next, nbM))
					success = true;
				++nbM;
				dico_next.swap(dico);
				dico_insert_word(dico, i, temp, nbM);
			}
			anag_next.pop_back();
			anag_next.swap(anagram);
		}
	}
	cout << to_be_displayed;
	return success;
}

bool message_contains_word(string& alpha, string& alpha_m)
{
	size_t m_size(alpha_m.size());
	size_t j(0);
	size_t word_size(alpha.size());
	for (size_t i(0); i < m_size and j < word_size; ++i)
	{
		if (alpha_m[i] > alpha[j])
			return false;
		else if (alpha_m[i] == alpha[j])
			++j;
	}
	if (j == word_size)
		return true;
	return false;
}

string message_minus_word(string alpha_m, string& alpha)
{
	for (auto letter : alpha)
		alpha_m.erase(alpha_m.find(letter), 1);
	return alpha_m;
}

void dico_erase_word(Dictionary& dico, size_t& pos, size_t& nbM)
{
	for (size_t j(pos); j < nbM - 1; ++j)
		dico[j] = dico[j+1];
	dico.pop_back();
}

void dico_insert_word(Dictionary& dico, size_t& pos, Mot& word, size_t& nbM)
{
	dico.push_back(dico[0]);
	for (size_t j(nbM-1); j > pos; --j)
		dico[j] = dico[j-1];
	dico[pos] = word;
}
