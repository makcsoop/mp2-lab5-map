#include "Polinom.h"

Polinom::Polinom() {
    // pFirst = 0;
    // sz = 0;
    PushFront(Monom{0.0, 0});
}

Polinom::Polinom(Monom monom) {
    TNode* tmp = new TNode{monom, nullptr};
    sz = 1;
    pFirst = tmp;
}

Polinom::Polinom(double number) { PushFront(Monom{number, 0}); }

Polinom::Polinom(string pol_str) {
    pol_str.erase(remove(pol_str.begin(), pol_str.end(), ' '), pol_str.end());
    vector<string> Monoms = split(pol_str, '+');
    for (int i = Monoms.size() - 1; i >= 0; i--) {
        map<string, double> res = parseMonom(Monoms[i]);
        PushFront(Monom{res["value"], ((int)res["x"] * 100 + (int)res["y"] * 10 + (int)res["z"])});
    }
    if (Monoms.size() == 0) {
        PushFront(Monom{0.0, 0});
    }
}

Polinom Polinom::operator+(const Polinom& pol) {
    TNode* index_f = pFirst;
    TNode* index_s = pol.pFirst;
    Polinom res;
    int index_res = 0;
    int flag = 1;
    while (index_f != nullptr && index_s != nullptr) {
        // log("TEK DEGRESS " + to_string(index_f->value.degrees) + ' ' + to_string(index_s->value.degrees) );
        if (index_f != nullptr && index_s != nullptr && index_s->value.degrees == index_f->value.degrees) {
            if ((index_s->value.coeef + index_f->value.coeef) != 0) {
                res.PushAfter(index_res,
                              Monom{index_s->value.coeef + index_f->value.coeef, index_s->value.degrees});
                index_res++;
            }
            index_f = index_f->pNext;
            index_s = index_s->pNext;

        } else if (index_f != nullptr && index_s != nullptr &&
                   index_s->value.degrees > index_f->value.degrees) {
            if (index_s->value.coeef != 0) {
                res.PushAfter(index_res, Monom{index_s->value.coeef, index_s->value.degrees});
                index_res++;
            }
            index_s = index_s->pNext;

        } else if (index_f != nullptr) {
            if (index_f->value.coeef != 0) {
                res.PushAfter(index_res, Monom{index_f->value.coeef, index_f->value.degrees});
                index_res++;
            }
            index_f = index_f->pNext;
        }
    }
    while (index_f != nullptr) {
        if (index_f->value.coeef != 0) {
            res.PushAfter(index_res, Monom{index_f->value.coeef, index_f->value.degrees});
            index_res++;
        }
        index_f = index_f->pNext;
    }
    while (index_s != nullptr) {
        if (index_s->value.coeef != 0) {
            res.PushAfter(index_res, Monom{index_s->value.coeef, index_s->value.degrees});
            index_res++;
        }
        index_s = index_s->pNext;
    }
    if (res.Front().degrees == 0 && res.Front().coeef == 0 && res.size() != 1) {
        res.PopFront();
    }
    return res;
}

Polinom Polinom::operator+=(const Monom& monom) {
    int flag = 1;
    if (monom.coeef == 0) {
        return *this;
    }
    for (int i = 0; i < size(); i++) {
        if ((*this)[i].degrees == monom.degrees) {
            (*this)[i].coeef += monom.coeef;
            flag = 0;
            break;
        }
    }
    if (flag) {
        for (int i = size() - 1; i >= 0; i--) {
            if ((*this)[i].degrees > monom.degrees) {
                (*this).PushAfter(i, monom);
                flag = 0;
                break;
            }
        }
    }
    if (flag) {
        (*this).PushFront(monom);
    }
    return *this;
}

Polinom Polinom::operator-(const Polinom& pol) {
    Polinom obrat("-1");
    Polinom tmp((obrat * pol).Print());
    return tmp + (*this);
}

Polinom Polinom::operator*(const Polinom& pol) {
    Polinom tmp;
    for (int i = 0; i < (*this).size(); i++) {
        for (int j = 0; j < pol.size(); j++) {
            if (((*this)[i].coeef * pol[j].coeef) != 0) {
                tmp += Monom{(*this)[i].coeef * pol[j].coeef, (*this)[i].degrees + pol[j].degrees};
            }
        }
    }
    int n = tmp.size();
    if (n > 1 && tmp[n - 1].coeef == 0 && tmp[n - 1].degrees == 0) {
        tmp.EraseAfter(n - 2);
    }
    return tmp;
}

Polinom Polinom::operator/(const Polinom& pol) { return Polinom("1"); }

Polinom Polinom::operator+(double pol) {
    Polinom tmp(to_string(pol));
    return (*this) + tmp;
}

Polinom Polinom::operator-(double pol) {
    Polinom tmp(to_string(pol));
    return (*this) - tmp;
}

Polinom Polinom::operator*(double pol) {
    Polinom tmp(to_string(pol));
    return (*this) * tmp;
}

bool Polinom::operator!=(int number) {
    if (size() == 1 && pFirst->value.coeef == number) {
        return true;
    }
    return false;
}

bool Polinom::operator==(const Polinom& pol) const { return Print() == pol.Print(); }

Polinom::~Polinom() {}

string Polinom::Print() {
    vector<string> res;
    string final = "";
    for (int i = 0; i < size(); i++) {
        int x = (*this)[i].degrees / 100;
        int y = (*this)[i].degrees / 10 % 10;
        int z = (*this)[i].degrees % 10;
        if (x == 0 && y == 0 && z == 0) {
            res.push_back(format_double((*this)[i].coeef));
            continue;
        }
        res.push_back(format_double((*this)[i].coeef) + "x" + to_string(x) + "y" + to_string(y) + "z" +
                      to_string(z));
    }
    for (int i = 0; i < size(); i++) {
        final += res[i];
        if (i < size() - 1) {
            final += "+";
        }
    }

    return final;
}

string Polinom::Print() const {
    vector<string> res;
    string final = "";
    for (int i = 0; i < size(); i++) {
        int x = (*this)[i].degrees / 100;
        int y = (*this)[i].degrees / 10 % 10;
        int z = (*this)[i].degrees % 10;
        if (x == 0 && y == 0 && z == 0) {
            res.push_back(format_double((*this)[i].coeef));
            continue;
        }
        res.push_back(format_double((*this)[i].coeef) + "x" + to_string(x) + "y" + to_string(y) + "z" +
                      to_string(z));
    }
    for (int i = 0; i < size(); i++) {
        final += res[i];
        if (i < size() - 1) {
            final += "+";
        }
    }

    return final;
}

ostream& operator<<(ostream& osr, const Polinom& pol) {
    Polinom::TNode* tmp = pol.pFirst;
    for (int i = 0; i < pol.size(); i++) {
        osr << tmp->value.coeef;
        osr << "\n";
        tmp = tmp->pNext;
    }
    return osr;
}

istream& operator>>(istream& ist, const Polinom& pol) {
    ist >> pol;
    return ist;
}

vector<string> split(string str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);

    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

map<string, double> parseMonom(string Monom) {
    map<string, double> res;

    bool flag = 1, flag_x = 1, flag_y = 1, flag_z = 1;
    string value = "";
    for (int i = 0; i < Monom.size(); i++) {
        if (i == 0 && Monom[i] == '-') {
            value += '-';
            continue;
        }
        if ((!isdigit(Monom[i]) && Monom[i] != '.') && flag) {
            res["value"] = stod(value);
            flag = 0;
        }
        if (flag) {
            value += Monom[i];
        } else {
            switch (Monom[i]) {
                case 'x':
                    res["x"] = Monom[i + 1] - '0';
                    flag_x = 0;
                    i++;
                    break;

                case 'y':
                    res["y"] = Monom[i + 1] - '0';
                    flag_y = 0;
                    i++;
                    break;
                case 'z':
                    res["z"] = Monom[i + 1] - '0';
                    flag_z = 0;
                    i++;
                    break;
            }
        }
    }
    if (flag_x) {
        res["x"] = 0;
    }
    if (flag_y) {
        res["y"] = 0;
    }
    if (flag_z) {
        res["z"] = 0;
    }
    if (value.size() > 0) {
        res["value"] = stod(value);
    }
    return res;
}

string format_double(double value) {
    stringstream ss;
    ss << value;
    return ss.str();
}

void log(string message, int type) {
    if (type == 0) {
        cout << Colors::GREEN << "[INFO] " << message << Colors::RESET << "\n";

    } else if (type == 1) {
        cout << Colors::YELLOW << "[DEBUG] " << message << Colors::RESET << "\n";
    } else {
        cout << Colors::RED << "[ERROR] " << message << Colors::RESET << "\n";
    }
}