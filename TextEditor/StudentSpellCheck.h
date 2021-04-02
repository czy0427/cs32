#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

#include <iostream>
#include <fstream>
using namespace std;

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck() { }
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
    struct Trie{
        char data;
        Trie* children[28]={nullptr};
    };
    
    Trie* dummy=new Trie;
    void addTrie(string str);
    bool wordIn(string word);

};

#endif  // STUDENTSPELLCHECK_H_
