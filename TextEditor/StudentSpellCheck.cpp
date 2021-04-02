#include "StudentSpellCheck.h"
#include <string>
#include <vector>

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
	// TODO
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
    ifstream infile(dictionaryFile);
    if ( ! infile )
        return false;
    string s;
    while (getline(infile,s)) {
        string temp;
        for(int i=0;i<s.size();i++) {
            if(isalpha(s[i]) || s[i]=='\'') {
                temp+=tolower(s[i]);
            }
        }
        /*
        if (s.back()=='\r' || s.back()=='\n') {
            s.pop_back();
            if (s.back()=='\r' || s.back()=='\n') {
                s.pop_back();
            }
        }
         */
        
        addTrie(temp);
    }
    
    return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	return false; // TODO
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
    int s=0;
    int e=0;

    for (int i=0;i<line.size();i++) {
        //break into words, record start and end position
        char c=tolower(line[i]);
        if(!isalpha(c) )//&& c!='\'')
            continue;
        s=i;
        string temp="";
        temp+=c;
        for(int j=i+1;j<line.size();j++) {
            i++;
            char d=tolower(line[j]);
            if(isalpha(d))
                temp+=d;
            else if(d=='\'') {
                if (j!=line.size()-1 && isalpha(line[j+1]))
                    temp+=d;
                else if(j!=line.size()-1 && !isalpha(line[j+1])) {
                    e=j-1;
                    break;
                }
            }
            else { //if(j!=line.size()-1 && !isalpha(line[j+1])) {
                e=j-1;
                break;
            }
        }
        if (!wordIn(temp)){
            e=i;
         //   cerr<<"WRONG!"<<endl;
            Position p;
            p.start=s;
            p.end=e;
            problems.push_back(p);
        }
    }
     
}

void StudentSpellCheck::addTrie(string str) {
    Trie *curr=dummy;
    for(int i=0;i<str.size();i++) {
        for(int j=0;j<28;j++) {
            if(curr->children[j]!=nullptr && curr->children[j]->data==str[i]) {
                curr=curr->children[j];
                break;
            }
            else if(curr->children[j]==nullptr) {
                curr->children[j]=new Trie;
                curr->children[j]->data=str[i];
                curr=curr->children[j];
                break;
            }
        }
        if(i==str.size()-1){
            curr->children[0]=new Trie;
            curr->children[0]->data='.';
        }
    }
}

bool StudentSpellCheck::wordIn(string word) {
    Trie *curr=dummy;
    for(int i=0;i<word.size();i++) {
        for(int j=0;j<28;j++) {
            if(curr->children[j]==nullptr)
                break;
            if(curr->children[j]->data==tolower(word[i])) {
                curr=curr->children[j];
                break;
            }
            if(j==27)
                return false;
        }
        if(i==word.size()-1)
            if(curr->children[0]->data=='.')
            return true;
    }
    return false;
}
