// Nicolás Herrera Hernández, A01114972
// Actividad integradora 1 - Longest common substring


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

#define MAX 1000

using namespace std;

// Funcion que convierte nuestros archivos de entrada en string para su procesamiento
string fileToString(string fileName){
    ifstream f(fileName); 
    string str;
    if(f) {
        ostringstream ss;
        ss << f.rdbuf(); 
        str = ss.str();
    }
    str.erase(remove(str.begin(), str.end(), '\n'), str.end());
    return str;
}

// Funcion auxiliar del algoritmo KMP
vector<int> lpsv(string p){
    int n = p.length();
    vector<int> lpsv(n, 0); 
    int j=0, i=1;
    while (i < n){
        if (p[i] == p[j]){
            lpsv[i] = j+1;
            i++;
            j++;
        }
        else{
            if (j == 0){
                lpsv[i] = 0;
                i++;
            }
            else{
                j = lpsv[j-1];
            }
        }
    }
    return lpsv;
}

// COMPLEJIDAD: O(n)
vector<int> KMP(string t, string p){
    vector<int> matchPos;
    vector<int> lps = lpsv(p);
    int j = 0, i = 0;
    int n = t.length();
    int m = p.length();
    while (i < n){
        if (t[i] == p[j]){
            j++;
            i++;
            if (j == m){
                matchPos.push_back(i-m);
                j = lps[j-1];
            }
        }
        else{
            if(j == 0){
                i++;
            }
            else{
                j = lps[j-1];
            }
        }
    }
    return matchPos;
}

// Aplica el algoritmo KMP para buscar incidencias de codigo malicioso proporcionado en un string
void applyKMP(string line, string fileString, int i, ofstream &output){
    vector<int> positions = KMP(fileString, line);
    output.open("output.txt", ios::app);       
    output << "Transmission " << i << ".txt =========> " << positions.size() << " veces\n";
    if (positions.size() == 0){
        output << "0" ;
    }
    else {
        for (int j = 0; j < positions.size(); j++){
        output << positions[j] << " ";
        }
    }
    output << "\n"; 
    output.close();
}


// COMPLEJIDAD: O(n)
string manacher(string texto){
	string T = "";
	int n = texto.length();
	for (int i=0; i<n; i++){
		T += "|";
		T += texto[i];
	}
	T += "|";
	n = T.length();
	vector<int> L(n);
	L[0] = 0;
	L[1] = 1;
	int maxLong=0, maxCentro=0;
	bool exp;
	for (int c=1, li=0, ri=2; ri<n; ri++){
		li = c-(ri-c);
		exp = false;
		if (c-maxLong <= ri && ri >= c+maxLong){
			if (L[li] < (c+L[c]-ri)){
				L[ri] = L[li];
			}
			else if(L[li] == (c + L[c]) - ri && (c + L[c]) == 2*n){
				L[ri] = L[li];
			}
			else if(L[li] == (c + L[c]) - ri && (c + L[c]) < 2*n){
				L[ri] = L[li];
				exp = true;
			}
			else if(L[li] > (c + L[c]) - ri && (c + L[c]) < 2*n){
				L[ri] = (c+L[c]) - ri;
				exp = true;
			}
		}
		else{
			L[ri] = 0;
			exp = true;
		}
		if (exp){
			while((ri + L[ri] < n) && (ri - L[ri] > 0) && (T[ri - L[ri] - 1] == T[ri + L[ri] + 1])){
				L[ri]++;
			}
		}
		c = ri;
		if (L[ri] > maxLong){
	    	maxLong = L[ri];
	    	maxCentro = ri;
	    }
	}
	int inicio = (maxCentro - maxLong) / 2;
	string salida = "";
	for (int i=inicio; i<(inicio+maxLong); i++){
		salida+=texto[i];
	}
	return salida;
}


// COMPLEJIDAD: O(n*m)
string LCSubstr(string s1, string s2){
    int m = s1.length();
    int n = s2.length();
    int result = 0;
    int end;
    int len[2][MAX];
    int currRow = 0;
 
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                len[currRow][j] = 0;
            }
            else if (s1[i-1] == s2[j-1]) {
                len[currRow][j] = len[1 - currRow][j-1] + 1;
                if (len[currRow][j] > result) {
                    result = len[currRow][j];
                    end = i - 1;
                }
            }
            else {
                len[currRow][j] = 0;
            }
        }
        currRow = 1 - currRow;
    }
    if (result == 0){
        return "No common substring";
    }
    return s1.substr(end - result + 1, result);
}


int main(){
    int mat[MAX][MAX];
    ofstream output; 
    // Lectura de archivos y pasar dichos archivos a strings
    string transmission1 =  fileToString("transmission1.txt"); 
    string transmission2 =  fileToString("transmission2.txt"); 
    string transmission3 =  fileToString("transmission3.txt"); 
    // Incidencias de codigo malicioso y sus posiciones
    ifstream file("mcode.txt");
    if (file.is_open()) {
        output.open("output.txt");  
        output << "====================================== \n";
        output.close();
        string line;
        while (getline(file, line)) {
            output.open("output.txt", ios::app);
            output << "Codigo: " << line << "\n"; 
            output.close();
            applyKMP(line, transmission1, 1, output);
            applyKMP(line, transmission2, 2, output);
            applyKMP(line, transmission3, 3, output);
            output.open("output.txt", ios::app);
            output << "----------------------------------\n"; 
            output.close();
        }
        file.close();
    }
    output.open("output.txt", ios::app);
    output << "======================================" <<  "\n"; 
    // Palindromos 
    // Busqueda de palindromo mas grande de cada archivo usando el algoritmo de Manacher
    string palTrans1 = manacher(transmission1);
    string palTrans2 = manacher(transmission2);
    string palTrans3 = manacher(transmission3);

    output << "Palindromo mas grande: " <<  "\n"; 
    output << "Transmission1.txt =====> Posicion: " << transmission1.find(palTrans1) << "\n"; 
    output << palTrans1 <<  "\n"; 
    output << "Transmission2.txt =====> Posicion: " << transmission2.find(palTrans2) <<  "\n"; 
    output << palTrans2 <<  "\n"; 
    output << "Transmission3.txt =====> Posicion: " << transmission3.find(palTrans3) << "\n"; 
    output << palTrans3 << "\n"; 
    // Substrings - longest common substring
    output << "======================================" << "\n"; 
    output << "LCS Transmision 1 & 2: " << LCSubstr(transmission1, transmission2) << "\n"; 
    output << "LCS Transmision 1 & 3: " << LCSubstr(transmission1, transmission3) << "\n"; 
    output << "LCS Transmision 2 & 3: " << LCSubstr(transmission2, transmission3) << "\n"; 
    output << "======================================" << "\n"; 
    output.close();
}