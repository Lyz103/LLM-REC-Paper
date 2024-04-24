#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
std::map<std::string, std::string> mp;
std::map<std::string, std::string> mp1{{"=", "AO"},
                                       {"==", "RO"},
                                       {">", "RO"},
                                       {">=", "RO"},
                                       {"<", "RO"},
                                       {"<=", "RO"},
                                       {"||", "LO"},
                                       {"&&", "LO"},
                                       {"!", "LO"},
                                       {"!=", "RO"},
                                       {"+", "PLUS"},
                                       {"-", "MINUS"},
                                       {"*", "TIMES"},
                                       {"/", "DIVISION"},
                                       {",", "COMMA"},
                                       {"(", "BRACE"},
                                       {")", "BRACE"},
                                       {"{", "BRACE"},
                                       {"}", "BRACE"},
                                       {";", "SEMICOLON"}};

void solve()
{
    std::vector<string> ans;
    std::string s, str;
    std::string last;

    std::ifstream inputFile("in.txt");
    if (inputFile.is_open())
    {
        while (getline(inputFile, s))
        {
            str += s;
            str += '\n';
        }
        inputFile.close();
    }
    else
    {
        std::cout << "Failed to open input file." << std::endl;
        return;
    }

    std::cerr << "////\n";
    while (str.find("/*") != -1)
    {
        int pos = str.find("/*");
        int pos1 = str.find("*/");
        str.erase(pos, pos1 - pos + 2);
    }
    while (str.find("//") != -1)
    {
        int pos = str.find("//");
        int pos1 = str.find("\n", pos);
        str.erase(pos, pos1 - pos + 1);
    }

    int n = str.size();
    for (int i = 0; i < n; i++)
    {
        std::string token;
        if (s[i] == ' ' || s[i] == '\n')
        {
            if (token.size() > 0)
            {
                if (mp.count(token))
                {
                    ans.push_back(token + ' ' + mp[token] + '\n');
                }
                else
                {
                    int idx = 0;
                    std::string h;
                    ///////////////////////////////////////////////
                    while (idx <= token.size())
                    {
                        while (isalpha(token[idx]) || token[idx] == '_')
                        {
                            h += token[idx++];
                        }
                        if (h != "")
                        {
                            ans.push_back(h + " IDENT\n");
                        }
                        h.clear();
                        /////////////////////////////////////////////////
                        while (isdigit(token[idx]) || token[idx] == '.')
                        {
                            h += token[idx++];
                        }
                        if (h[0] == '.' || h.back() == '.')
                        {
                            std::cout << "Malformed number: Decimal point at the beginning or end of a floating point number.\n";
                            return;
                        }

                        if (std::count(h.begin(), h.end(), '.') >= 2)
                        {
                            std::cout << "Malformed number: More than one decimal point in a floating point number.";
                            return;
                        }
                        if (h[0] == '0' && std::count(h.begin(), h.end(), '.') == 0)
                        {
                            std::cout << "Malformed number: Leading zeros in an integer.\n";
                            return;
                        }
                        if (std::count(h.begin(), h.end(), '.') == 0)
                        {
                            ans.push_back(h + ' ' + "INT\n");
                        }
                        else
                        {
                            ans.push_back(h + " DOUBLE\n");
                        }
                        /////////////////////////////////////////////////
                        if (mp1.count(token.substr(idx, 2)))
                        {
                            ans.push_back(mp1[token.substr(idx, 2)] + "\n");
                            idx += 2;
                        }
                        else if (mp1.count(token.substr(idx, 1)))
                        {
                            ans.push_back(mp1[token.substr(idx, 1)] + "\n");
                            idx += 1;
                        }
                    }
                }
            }

            token = "";
        }
        else
        {
            token += s[i];
        }
    }

    for (auto s : ans)
    {
        std::cout << s;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    mp["int"] = "INTSYM";
    mp["double"] = "DOUBLESYM";
    mp["scanf"] = "SCANFSYM";
    mp["printf"] = "PRINTFSYM";
    mp["if"] = "IFSYM";
    mp["then"] = "THENSYM";
    mp["while"] = "WHILESYM";
    mp["do"] = "DOSYM";

    int t = 1;
    // std::cin >> t;

    while (t--)
    {
        solve();
    }
    return 0;
}