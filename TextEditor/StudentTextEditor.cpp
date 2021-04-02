#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) {
	// TODO
     m_row=0;
     m_col=0;
     l.push_back("");
     m_it=l.begin();
}

StudentTextEditor::~StudentTextEditor()
{
    l.clear();
}

bool StudentTextEditor::load(std::string file) {
    ifstream infile(file);
    if ( ! infile )
    {
        cerr << "Error: Cannot open "<< file << endl;
        return false;
    }
    /*
    char c;
    while(infile.get(c)) {
        if(c=='\r' || c=='\n') {
            enter();
        }
        else
            insert(c);
    }
    */
    if(loaded==true) {
        reset();
    }
    loaded=true;
    string s;
    while (getline(infile,s)) {
        if (s.back()=='\r' || s.back()=='\n') {
            s.pop_back();
            if (s.back()=='\r' || s.back()=='\n') {
                s.pop_back();
            }
        }
        l.push_back(s);
    }
   // m_it=l.begin();
    return true;
}

bool StudentTextEditor::save(std::string file) {
    ofstream outfile(file);   // outfile is a name of our choosing.
    if ( ! outfile )           // Did the creation fail?
    {
        cerr << "Error: Cannot create results.txt!" << endl;
        return false;
    }
  //  outfile << "This will be written to the file" << endl;
  //  outfile << "2 + 2 = " << 2+2 << endl;
    for(list<string>::const_iterator it=l.begin();it!=l.end();it++)
        outfile << *it << endl;
    return true;
}

void StudentTextEditor::reset() {
    l.clear();
    m_row=0;
    m_col=0;
    l.push_back("");
    m_it=l.begin();
}

void StudentTextEditor::move(Dir dir) {
    switch(dir) {
        case RIGHT:
            if(m_row==l.size()-1 && m_col==int(m_it->size()))
                return;
            else if(m_row<l.size()-1 && m_col==int(m_it->size())){
                m_col=0;
                m_row++;
                m_it++;
            }
            else
                m_col++;
            break;
        case LEFT:
            if(m_col==0 && m_row==0)
                return;
            else if(m_col!=0)
                m_col--;
            else if(m_col==0 && m_row != 0)
            {
                m_it--;
                m_row--;
                m_col=int(m_it->size());
            }
            break;
        case DOWN:
            if(m_row<l.size()-1){
                m_row++;
                m_it++;
                if(m_col>int(m_it->size()))
                   m_col=int((*m_it).size());
            }
            break;
        case UP:
            if(m_row>0) {
                m_row--;
                m_it--;
                if(m_col>int(m_it->size()))
                   m_col=int((*m_it).size());
            }
            break;
        case END:
            m_col=int(m_it->size());
            break;
        case HOME:
            m_col=0;
            break;
    }
}

void StudentTextEditor::del() {
    if(m_col<(*m_it).size()) 
        (*m_it).erase(m_col,1);
    else if(m_col==(*m_it).size()) {
        if(m_row==l.size()-1)
            return;
        string s=(*m_it++);
        (*m_it).erase();
        l.erase(m_it);
        m_it--;
        (*m_it).append(s);
    }
}

void StudentTextEditor::backspace() {
    if(m_col==0 && m_row==0)
        return;
    if(m_col>0)
        (*m_it).erase(--m_col,1);
    else if(m_col==0) {
        string s=(*m_it);
     //   (*m_it).erase();
        l.erase(m_it);
        m_it--;
        m_col=int((*m_it).length());
        (*m_it).append(s);
        m_row--;
    }
}

void StudentTextEditor::insert(char ch) {
  //  if(l.size()==0) {
 //       l.push_back("");
 //       m_it=l.begin();
  //  }
    if(ch=='\t') {
        (*m_it).insert(m_col,4,' ');
        m_col+=4;
    }
    else {
        (*m_it).insert(m_col,1,ch);
        //move(RIGHT);
        m_col++;
    }
}

void StudentTextEditor::enter() {
    string s=(*m_it).substr(m_col);
    (*m_it).erase(m_col);
    if(m_row==l.size()-1) {
        l.push_back(s);
        m_it++;
    }
    else {
        m_it++;
        l.insert(m_it,s);
        m_it--;
        // careful of where the iterator is!! "www.cplusplus.com/reference/list/list/insert/"
    }
    m_col=0;
    m_row++;
}

void StudentTextEditor::getPos(int& row, int& col) const {
	row = m_row; col = m_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
    list<string>::const_iterator it=l.begin();
    advance(it,startRow);
    int countdown=numRows;
    while(it!=l.end() && countdown!=0) {
        lines.push_back(*it);
        it++;
        countdown--;
    }
 //   for(list<string>::const_iterator it=l.begin();it!=l.end();it++)
 //       lines.push_back(*it);
	return 0; // TODO
}

void StudentTextEditor::undo() {
	// TODO
}
