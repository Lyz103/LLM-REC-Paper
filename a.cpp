/*
    wirte by @lyz
    2024.4.26
    编译原理实验1：词法分析器
*/
#include <bits/stdc++.h>

#define LOCAL 1

std::map<std::string, std::string> mp{
    {"int", "INTSYM"},       {"double", "DOUBLESYM"}, {"scanf", "SCANFSYM"},
    {"printf", "PRINTFSYM"}, {"if", "IFSYM"},         {"then", "THENSYM"},
    {"while", "WHILESYM"},   {"do", "DOSYM"}};
std::map<std::string, std::string> mp1{
    {"=", "AO"},    {"==", "RO"},      {">", "RO"},    {">=", "RO"},
    {"<", "RO"},    {"<=", "RO"},      {"||", "LO"},   {"&&", "LO"},
    {"!", "LO"},    {"!=", "RO"},      {"+", "PLUS"},  {"-", "MINUS"},
    {"*", "TIMES"}, {"/", "DIVISION"}, {",", "COMMA"}, {"(", "BRACE"},
    {")", "BRACE"}, {"{", "BRACE"},    {"}", "BRACE"}, {";", "SEMICOLON"}};

void solve() {
  std::vector<std::string> ans;
  std::string s, str;
  std::string last;

  // 输入流 ////////////////////////////////////////////
  std::ifstream input("C:\\Users\\LYZ\\Desktop\\untitled1\\in2.txt");
  std::ofstream fout("C:\\Users\\LYZ\\Desktop\\untitled1\\out.txt");
  if (LOCAL) {
    if (input.is_open()) {
      while (getline(input, s)) {
        str += s;
        str += '\n';
      }
      input.close();
    } else {
      std::cout << "Failed to open input file." << std::endl;
      return;
    }
  } else
    while (getline(std::cin, s)) {
      str += s;
      str += '\n';
    }

  // 去注释////////////////////////////////////////////
  while (str.find("/*") != -1) {
    int pos = str.find("/*");
    int pos1 = str.find("*/");
    if (pos1 == -1) {
      str.erase(pos);
    }
    str.erase(pos, pos1 - pos + 2);
  }
  while (str.find("//") != -1) {
    int pos = str.find("//");
    int pos1 = str.find("\n", pos);
    str.erase(pos, pos1 - pos + 1);
  }
  s.clear();

  // 加空格////////////////////////////////////////////
  int n = str.size();
  for (int i = 0; i < n; i++) {
    if (i + 2 <= n && mp1.count(str.substr(i, 2))) {
      s.push_back(' ');
      s.push_back(str[i]);
      s.push_back(str[i + 1]);
      s.push_back(' ');
      i += 1;
    } else if (i + 1 <= n && mp1.count(str.substr(i, 1))) {
      s.push_back(' ');
      s.push_back(str[i]);
      s.push_back(' ');
    } else {
      s.push_back(str[i]);
    }
  }
  str = s;
  n = str.size();

  // 分词//////////////////////////////////////////////
  std::string token;
  for (int i = 0; i < n; i++) {
    if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') {
      if (!token.empty()) {
        if (mp.count(token)) {
          ans.push_back(token + ' ' + mp[token] + '\n');
        } else {
          int idx = 0;
          bool alp = 0;
          bool dig = 0;
          std::string h;
          ///////////////////////////////////////////////
          while (idx < token.size()) {
            while (isalpha(token[idx])) {
              h += token[idx++];
              alp = true;
            }
            while (isdigit(token[idx]) && alp) {
              h += token[idx++];
            }
            if (!h.empty()) {
              ans.push_back(h + " IDENT\n");
            }
            h.clear();
            /////////////////////////////////////////////////
            while (isdigit(token[idx]) || token[idx] == '.') {
              h += token[idx++];
              dig = 1;
            }
            if (dig && std::count(h.begin(), h.end(), '.') >= 2) {
              std::cout << "Malformed number: More than one decimal point in a "
                           "floating point number.\n";
              return;
            }
            if (dig && (h[0] == '.' || h.back() == '.')) {
              std::cout << "Malformed number: Decimal point at the beginning "
                           "or end of a floating point number.\n";
              return;
            }
            if (dig && h[0] == '0' && h.size() > 1) {
              bool isok = 1;
              if (h.find('.') != -1) {
                for (int j = 0; j < h.size(); j++) {
                  if (h[j] == '.') {
                    break;
                  }
                  if (h[j] != '0') {
                    isok = 0;
                    break;
                  }
                }
              } else {
                isok = 0;
              }
              if (!isok) {
                std::cout << "Malformed number: Leading zeros in an integer.\n";
                return;
              }
            }
            if (dig && std::count(h.begin(), h.end(), '.') == 0) {
              ans.push_back(h + ' ' + "INT\n");
            } else if (dig) {
              ans.push_back(h + " DOUBLE\n");
            }
            /////////////////////////////////////////////////
            if (idx + 2 <= token.size() && mp1.count(token.substr(idx, 2))) {
              ans.push_back(token.substr(idx, 2) + " " +
                            mp1[token.substr(idx, 2)] + "\n");
              idx += 2;
            } else if (mp1.count(token.substr(idx, 1))) {
              ans.push_back(token.substr(idx, 1) + " " +
                            mp1[token.substr(idx, 1)] + "\n");
              idx += 1;
            }

            if (idx < token.size() &&
                (!isdigit(token[idx]) && !isalpha(token[idx])) &&
                (dig == 0 && alp == 0)) {
              std::cout << "Unrecognizable characters.\n";
              return;
            }
          }
        }
      }

      token = "";
    } else {
      token += str[i];
    }
  }

  // 输出流////////////////////////////////////////////
  if (LOCAL)
    for (auto s : ans) {
      fout << s;
    }
  else
    for (auto s : ans) {
      std::cout << s;
    }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int t = 1;

  while (t--) {
    solve();
  }
  return 0;
}
