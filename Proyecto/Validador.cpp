#include "Validador.h"
#include <cctype>
#include <algorithm>

using namespace std;

// ============================================================
//  UTILIDADES DE TEXTO
// ============================================================
string Validador::aMayusculas(string texto) {
    string r = texto;
    char* p = &r.front();
    while (*p != '\0') { *p = toupper(*p); p++; }
    return r;
}

string Validador::trim(string texto) {
    if (texto.empty()) return texto;
    const char* ini = texto.c_str();
    while (*ini == ' ' || *ini == '\t') ini++;
    if (*ini == '\0') return "";
    const char* fin = texto.c_str() + texto.size() - 1;
    while (fin > ini && (*fin == ' ' || *fin == '\t')) fin--;
    return string(ini, fin - ini + 1);
}

bool Validador::validarTexto(string texto) {
    if (texto.empty()) return false;
    const char* p = texto.c_str();
    while (*p != '\0') { if (!isalpha(*p) && *p != ' ') return false; p++; }
    return true;
}

string Validador::leerTextoValido(string mensaje) {
    string valor;
    while (true) {
        cout << mensaje;
        getline(cin, valor);
        if (!valor.empty() && valor.back() == '\r') valor.pop_back();
        string t = trim(valor);
        if (t.empty()) { cout << "  [!] El campo no puede estar vacio.\n"; continue; }
        bool ok = true;
        const char* p = t.c_str();
        while (*p != '\0') {
            if (!isalpha((unsigned char)*p) && *p != ' ') { ok = false; break; }
            p++;
        }
        if (!ok) { cout << "  [!] Solo letras y espacios, sin numeros ni simbolos.\n"; continue; }
        return t;
    }
}

// ============================================================
//  LECTURA SEGURA DE DATOS
// ============================================================
int Validador::leerEntero(string mensaje, int mn, int mx) {
    int v;
    while (true) {
        cout << mensaje;
        if (cin >> v && v >= mn && v <= mx) { cin.ignore(10000, '\n'); return v; }
        cin.clear(); cin.ignore(10000, '\n');
        cout << "  [!] Ingrese un numero entre " << mn << " y " << mx << ".\n";
    }
}

string Validador::leerSoloLetras(const string& mensaje) {
    string valor;
    while (true) {
        cout << mensaje;
        getline(cin, valor);
        size_t ini = valor.find_first_not_of(" \t");
        size_t fin = valor.find_last_not_of(" \t");
        if (ini == string::npos) { cout << "  [!] El campo no puede estar vacio.\n"; continue; }
        valor = valor.substr(ini, fin - ini + 1);
        bool valido = true;
        for (char c : valor)
            if (!isalpha((unsigned char)c) && c != ' ') { valido = false; break; }
        if (!valido) { cout << "  [!] Solo se permiten letras, sin numeros ni simbolos.\n"; continue; }
        break;
    }
    return valor;
}

string Validador::leerSoloNumeros(const string& mensaje, int longitud) {
    string valor;
    while (true) {
        cout << mensaje;
        getline(cin, valor);
        valor.erase(remove(valor.begin(), valor.end(), ' '), valor.end());
        if (valor.empty()) { cout << "  [!] El campo no puede estar vacio.\n"; continue; }
        bool ok = true;
        for (char c : valor) if (!isdigit((unsigned char)c)) { ok = false; break; }
        if (!ok) { cout << "  [!] Solo se permiten numeros.\n"; continue; }
        if (longitud > 0 && (int)valor.size() != longitud) {
            cout << "  [!] Debe ingresar exactamente " << longitud << " digitos.\n"; continue;
        }
        break;
    }
    return valor;
}

// ============================================================
//  VALIDACIONES ECUADOR
// ============================================================
bool Validador::validarCedula(string cedula) {
    if (cedula.length() != 10) return false;
    const char* p = cedula.c_str();
    for (const char* t = p; *t != '\0'; t++) if (!isdigit(*t)) return false;
    int prov = (*p - '0') * 10 + (*(p+1) - '0');
    if (prov < 1 || (prov > 24 && prov != 30)) return false;
    if (*(p+2) - '0' >= 6) return false;
    int suma = 0;
    for (int i = 0; i < 9; i++) {
        int v = *(p+i) - '0';
        if (i % 2 == 0) { v *= 2; if (v > 9) v -= 9; }
        suma += v;
    }
    int calc = (suma % 10 == 0) ? 0 : 10 - (suma % 10);
    return calc == (*(p+9) - '0');
}

string Validador::pedirCedula(const string& mensaje) {
    string cedula;
    while (true) {
        cedula = leerSoloNumeros(mensaje, 10);
        if (!validarCedula(cedula))
            cout << "  [!] Cedula ecuatoriana no valida. Verifique los 10 digitos.\n";
        else break;
    }
    return cedula;
}

bool Validador::validarPlaca(string placa) {
    string p = aMayusculas(placa);
    p.erase(remove(p.begin(), p.end(), '-'), p.end());
    if (p.length() < 6 || p.length() > 7) return false;
    const char* ptr = p.c_str();
    for (int i = 0; i < 3; i++) if (!isalpha(*(ptr+i))) return false;
    for (size_t i = 3; i < p.length(); i++) if (!isdigit(*(ptr+i))) return false;
    string prov = "ABCEGHIJKLMNPQRSTUVWXYUZ";
    bool found = false;
    for (const char* q = prov.c_str(); *q != '\0'; q++)
        if (*q == *ptr) { found = true; break; }
    return found;
}

string Validador::normalizarPlaca(const string& placa) {
    string p = placa;
    for (char& c : p) c = toupper((unsigned char)c);
    if (p.size() == 8 && p[3] == '-') return p;
    if (p.size() == 7) return p.substr(0, 3) + "-" + p.substr(3);
    return p;
}

string Validador::pedirPlaca(const string& mensaje) {
    string placa;
    while (true) {
        cout << mensaje;
        getline(cin, placa);
        placa.erase(remove(placa.begin(), placa.end(), ' '), placa.end());
        if (placa.empty()) { cout << "  [!] El campo no puede estar vacio.\n"; continue; }
        if (!validarPlaca(placa)) { cout << "  [!] Placa invalida. Formato: ABC-1234.\n"; continue; }
        placa = normalizarPlaca(placa);
        break;
    }
    return placa;
}

// ============================================================
//  CIFRADO CESAR
// ============================================================
string Validador::cifrarPlacaCesar(string placa, int d) {
    string r = "";
    auto f = [&](auto& self, int i) -> void {
        if (i == (int)placa.size()) return;
        char c = placa[i];
        if      (c >= 'A' && c <= 'Z') r += (char)('A' + (c - 'A' + d) % 26);
        else if (c >= '0' && c <= '9') r += (char)('0' + (c - '0' + d) % 10);
        else                           r += c;
        self(self, i + 1);
    };
    f(f, 0);
    return r;
}

// ============================================================
//  CIFRADO XOR BIT A BIT
// ============================================================
string Validador::mostrarBits(unsigned char byte) {
    string b = "";
    for (int i = 7; i >= 0; i--) {
        b += ((byte >> i) & 1) ? '1' : '0';
        if (i == 4) b += ' ';
    }
    return b;
}

string Validador::cifrarXOR(string texto, unsigned char clave) {
    string r = "";
    for (size_t i = 0; i < texto.size(); i++)
        r += (char)((unsigned char)texto[i] ^ clave);
    return r;
}

string Validador::descifrarXOR(string cifrado, unsigned char clave) {
    return cifrarXOR(cifrado, clave);
}

void Validador::mostrarProcesoXOR(string texto, unsigned char clave) {
    cout << "\n  Clave XOR : " << (int)clave << "  ->  [" << mostrarBits(clave) << "]\n";
    cout << "  =============================================================\n";
    cout << "  Char | Bits originales   ^ Bits clave      = Resultado\n";
    cout << "  -------------------------------------------------------------\n";
    string cifrado = "";
    for (size_t i = 0; i < texto.size(); i++) {
        unsigned char orig = (unsigned char)texto[i];
        unsigned char res  = orig ^ clave;
        cifrado += (char)res;
        cout << "   '" << texto[i] << "'"
             << "  [" << mostrarBits(orig) << "]"
             << " ^ [" << mostrarBits(clave) << "]"
             << " = [" << mostrarBits(res) << "]"
             << " -> '" << (char)(res >= 32 && res < 127 ? res : '?')
             << "' (" << (int)res << ")\n";
    }
    cout << "  -------------------------------------------------------------\n";
    cout << "  Original : " << texto << "\n";
    cout << "  Cifrado  : ";
    for (size_t i = 0; i < cifrado.size(); i++) {
        unsigned char c = (unsigned char)cifrado[i];
        if (c >= 32 && c < 127) cout << (char)c;
        else                    cout << "[" << (int)c << "]";
    }
    cout << "\n  (XOR es simetrico: la misma clave descifra el texto)\n";
}