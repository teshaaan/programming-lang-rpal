#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<string> keywords;

void initKeywords() {
  keywords.push_back("let");
  keywords.push_back("in");
  keywords.push_back("fn");
  keywords.push_back("where");
  keywords.push_back("aug");
  keywords.push_back("or");
  keywords.push_back("not");
  keywords.push_back("gr");
  keywords.push_back("ge");
  keywords.push_back("ls");
  keywords.push_back("le");
  keywords.push_back("eq");
  keywords.push_back("ne");
  keywords.push_back("true");
  keywords.push_back("false");
  keywords.push_back("nil");
  keywords.push_back("dummy");
  keywords.push_back("within");
  keywords.push_back("and");
  keywords.push_back("rec");
}

bool isLetter(char c) {
  if (c >= 'a' && c <= 'z')
    return true;
  if (c >= 'A' && c <= 'Z')
    return true;
  return false;
}

bool isDigit(char c) {
  if (c >= '0' && c <= '9')
    return true;
  return false;
}

bool isOperator(char c) {
  string ops = "+-*<>&.@/:=~|$!#%^_{}[]()";
  for (int i = 0; i < ops.length(); i++) {
    if (c == ops[i])
      return true;
  }
  return false;
}

bool isKeyword(string s) {
  for (int i = 0; i < keywords.size(); i++) {
    if (s == keywords[i])
      return true;
  }
  return false;
}

int currentLine = 1;

void lex(string input) {
  int i = 0;
  while (i < input.length()) {
    char c = input[i];

    if (isspace(c)) {
      if (c == '\n')
        currentLine++;
      i++;
      continue;
    }

    if (c == '/' && i + 1 < input.length() && input[i + 1] == '/') {
      while (i < input.length() && input[i] != '\n') {
        i++;
      }
      continue;
    }

    if (isLetter(c)) {
      string word = "";
      while (i < input.length() &&
             (isLetter(input[i]) || isDigit(input[i]) || input[i] == '_')) {
        word += input[i];
        i++;
      }
      if (isKeyword(word)) {
        cout << "<KEYWORD," << word << ">" << endl;
      } else {
        cout << "<IDENTIFIER," << word << ">" << endl;
      }
      continue;
    }

    if (isDigit(c)) {
      string num = "";
      while (i < input.length() && isDigit(input[i])) {
        num += input[i];
        i++;
      }
      cout << "<INTEGER," << num << ">" << endl;
      continue;
    }

    if (c == '\'') {
      string str = "";

      i++;
      while (i < input.length()) {
        if (input[i] == '\'') {
          i++;
          break;
        }
        if (input[i] == '\\' && i + 1 < input.length()) {
          str += input[i];
          str += input[i + 1];
          i += 2;
          continue;
        }
        str += input[i];
        i++;
      }
      cout << "<STRING," << str << ">" << endl;
      continue;
    }

    if (isOperator(c)) {
      string op = "";
      op += c;
      if (i + 1 < input.length() && isOperator(input[i + 1])) {
        string twoChar = op + input[i + 1];
        if (twoChar == "->" || twoChar == ">=" || twoChar == "<=" ||
            twoChar == "==" || twoChar == "!=" || twoChar == "**") {
          op = twoChar;
          i++;
        }
      }

      if (op == "(" || op == ")" || op == ";" || op == ",") {
        cout << "<PUNCTUATION," << op << ">" << endl;
      } else {
        cout << "<OPERATOR," << op << ">" << endl;
      }

      i++;
      continue;
    }

    if (c == '(' || c == ')' || c == ';' || c == ',') {
      cout << "<PUNCTUATION," << c << ">" << endl;
      i++;
      continue;
    }

    i++;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Usage: ./rpal <filename>" << endl;
    return 1;
  }

  string filename = argv[1];
  ifstream file(filename);
  if (!file.is_open()) {
    cout << "Error: Could not open file " << filename << endl;
    return 1;
  }

  stringstream buffer;
  buffer << file.rdbuf();
  string content = buffer.str();

  initKeywords();
  lex(content);

  return 0;
}