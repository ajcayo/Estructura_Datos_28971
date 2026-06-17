#include "GestorFechas.h"
#include <ctime>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

// ============================================================
//  GESTOR DE FECHAS - Sistema de Matriculacion Vehicular Ecuador
//  Reglas:
//   - No se asignan turnos en sabados ni domingos.
//   - No se asignan turnos en feriados nacionales de Ecuador.
//   - La fecha debe estar entre hoy y exactamente 1 anio adelante.
// ============================================================

std::string GestorFechas::validarRangoFecha(int dia, int mes, int anio) {

    // --- LAMBDAS DE UTILIDAD (sin corchetes, solo en lambdas) ---

    auto esBisiesto = [](int a) -> bool {
        return (a % 4 == 0 && a % 100 != 0) || (a % 400 == 0);
    };

    auto diasDelMes = [&esBisiesto](int m, int a) -> int {
        if (m == 2) return esBisiesto(a) ? 29 : 28;
        if (m == 4 || m == 6 || m == 9 || m == 11) return 30;
        return 31;
    };

    // Validacion de dias en el mes ingresado
    if (dia < 1 || dia > diasDelMes(mes, anio)) {
        std::ostringstream msg;
        msg << "El mes " << mes << " del anio " << anio
            << " solo tiene " << diasDelMes(mes, anio) << " dias.";
        return msg.str();
    }

    // Obtener la fecha actual del sistema
    std::time_t ahora = std::time(nullptr);
    std::tm* hoy = std::localtime(&ahora);
    int dHoy = hoy->tm_mday;
    int mHoy = hoy->tm_mon + 1;
    int aHoy = hoy->tm_year + 1900;

    // Lambda recursiva: compara dos fechas (d1/m1/a1) vs (d2/m2/a2)
    // Retorna -1 si primera < segunda, 0 si igual, 1 si primera > segunda
    auto compararFechas = [](auto& self, int d1, int m1, int a1,
                                         int d2, int m2, int a2) -> int {
        if (a1 != a2) return (a1 < a2) ? -1 : 1;
        if (m1 != m2) return (m1 < m2) ? -1 : 1;
        if (d1 != d2) return (d1 < d2) ? -1 : 1;
        return 0;
    };

    // La fecha no puede ser anterior a hoy
    if (compararFechas(compararFechas, dia, mes, anio, dHoy, mHoy, aHoy) < 0) {
        return "La fecha ingresada ya paso. No puede agendar turnos en el pasado.";
    }

    // Calcular la fecha limite: exactamente 1 anio a partir de hoy
    int dLimite = dHoy;
    int mLimite = mHoy;
    int aLimite = aHoy + 1;

    // Ajuste para el 29 de febrero en anio no bisiesto
    if (dHoy == 29 && mHoy == 2 && !esBisiesto(aLimite)) {
        dLimite = 28;
    }

    // La fecha no puede superar 1 anio desde hoy
    if (compararFechas(compararFechas, dia, mes, anio,
                       dLimite, mLimite, aLimite) > 0) {
        std::ostringstream msg;
        msg << "Solo puede agendar turnos hasta 1 anio desde hoy ("
            << dLimite << "/" << mLimite << "/" << aLimite
            << "). En Ecuador la matriculacion se renueva anualmente.";
        return msg.str();
    }

    return ""; // Fecha valida
}

// ============================================================

std::string GestorFechas::ajustarFechaLaborable(std::tm* fecha) {

    // ---- LAMBDAS DE UTILIDAD ----

    auto esBisiesto = [](int a) -> bool {
        return (a % 4 == 0 && a % 100 != 0) || (a % 400 == 0);
    };

    auto diasDelMes = [&esBisiesto](int m, int a) -> int {
        if (m == 2) return esBisiesto(a) ? 29 : 28;
        if (m == 4 || m == 6 || m == 9 || m == 11) return 30;
        return 31;
    };

    // Algoritmo de Zeller: 0=Dom, 1=Lun, 2=Mar, 3=Mie, 4=Jue, 5=Vie, 6=Sab
    auto calcularDiaSemana = [](int d, int m, int a) -> int {
        if (m == 1 || m == 2) { m += 12; a -= 1; }
        int K = a % 100;
        int J = a / 100;
        int h = (d + (13 * (m + 1)) / 5 + K + (K / 4) + (J / 4) + 5 * J) % 7;
        return (h + 6) % 7;
    };

    // Normaliza una fecha (maneja desbordamientos de dia y mes)
    auto normalizar = [&diasDelMes, &calcularDiaSemana](std::tm* f) {
        int a = f->tm_year + 1900;
        int m = f->tm_mon + 1;
        int d = f->tm_mday;
        while (d <= 0) {
            m--;
            if (m <= 0) { m = 12; a--; }
            d += diasDelMes(m, a);
        }
        while (d > diasDelMes(m, a)) {
            d -= diasDelMes(m, a);
            m++;
            if (m > 12) { m = 1; a++; }
        }
        f->tm_year = a - 1900;
        f->tm_mon  = m - 1;
        f->tm_mday = d;
        f->tm_wday = calcularDiaSemana(d, m, a);
    };

    // Lambda recursiva: verifica si una fecha (d,m,a) es feriado fijo de Ecuador.
    // Aplica la Ley de Traslados: si cae en fin de semana, el goce es el lunes/viernes.
    auto esFeriadoFijo = [&normalizar](auto& self, int dEval, int mEval, int aEval,
                                        int dFeriado, int mFeriado) -> bool {
        std::tm f = {0};
        f.tm_mday = dFeriado;
        f.tm_mon  = mFeriado - 1;
        f.tm_year = aEval - 1900;
        normalizar(&f);

        // Ley de traslados Ecuador
        if (f.tm_wday == 6) f.tm_mday -= 1;      // Sabado -> Viernes
        else if (f.tm_wday == 0) f.tm_mday += 1; // Domingo -> Lunes
        else if (f.tm_wday == 2) f.tm_mday -= 1; // Martes -> Lunes
        else if (f.tm_wday == 3) f.tm_mday += 2; // Miercoles -> Viernes
        else if (f.tm_wday == 4) f.tm_mday += 1; // Jueves -> Viernes
        normalizar(&f);

        return (f.tm_mday == dEval && (f.tm_mon + 1) == mEval);
    };

    // Lambda recursiva: verifica si la fecha es feriado movil (Carnaval / Viernes Santo)
    auto esFeriadoMovil = [&normalizar](auto& self, int dEval, int mEval, int aEval) -> bool {
        int A = aEval % 19; int B = aEval % 4;  int C = aEval % 7;
        int K = aEval / 100;
        int P = (13 + 8 * K) / 25;
        int Q = K / 4;
        int M = (15 - P + K - Q) % 30;
        int N = (4 + K - Q) % 7;
        int D = (19 * A + M) % 30;
        int E = (2 * B + 4 * C + 6 * D + N) % 7;
        int mesPascua = 3;
        int diaPascua = 22 + D + E;
        if (diaPascua > 31) { mesPascua = 4; diaPascua = D + E - 9; }
        if (mesPascua == 4 && diaPascua == 26) diaPascua = 19;
        if (mesPascua == 4 && diaPascua == 25 && D == 28 && E == 6 &&
            ((11 * M + 11) % 30 < 19)) diaPascua = 18;

        std::tm pascua = {0};
        pascua.tm_year = aEval - 1900;
        pascua.tm_mon  = mesPascua - 1;
        pascua.tm_mday = diaPascua;
        normalizar(&pascua);

        std::tm lunCarnaval = pascua; lunCarnaval.tm_mday -= 48; normalizar(&lunCarnaval);
        std::tm marCarnaval = pascua; marCarnaval.tm_mday -= 47; normalizar(&marCarnaval);
        std::tm vieSanto    = pascua; vieSanto.tm_mday    -= 2;  normalizar(&vieSanto);

        if (dEval == lunCarnaval.tm_mday && mEval == lunCarnaval.tm_mon + 1) return true;
        if (dEval == marCarnaval.tm_mday && mEval == marCarnaval.tm_mon + 1) return true;
        if (dEval == vieSanto.tm_mday    && mEval == vieSanto.tm_mon + 1)    return true;
        return false;
    };

    // Lambda: carga feriados fijos desde feriados.txt y retorna el nombre si la fecha coincide
    auto nombreFeriadoFijo = [&normalizar](auto& self, int dEval, int mEval, int aEval) -> std::string {
        auto choca = [&](int dF, int mF) -> bool {
            std::tm f = {0};
            f.tm_mday = dF; f.tm_mon = mF - 1; f.tm_year = aEval - 1900;
            normalizar(&f);
            if (f.tm_wday == 6) f.tm_mday -= 1;
            else if (f.tm_wday == 0) f.tm_mday += 1;
            else if (f.tm_wday == 2) f.tm_mday -= 1;
            else if (f.tm_wday == 3) f.tm_mday += 2;
            else if (f.tm_wday == 4) f.tm_mday += 1;
            normalizar(&f);
            return (f.tm_mday == dEval && (f.tm_mon + 1) == mEval);
        };

        std::ifstream archivo("feriados.txt");
        if (!archivo.is_open()) return "";

        std::string linea;
        while (std::getline(archivo, linea)) {
            // Ignorar lineas vacias y comentarios (#)
            if (linea.empty() || linea[0] == '#') continue;
            std::istringstream ss(linea);
            int dF, mF;
            std::string nombre;
            if (!(ss >> dF >> mF)) continue;
            std::getline(ss, nombre);
            // Quitar espacio inicial del nombre
            if (!nombre.empty() && nombre[0] == ' ') nombre = nombre.substr(1);
            if (choca(dF, mF)) {
                archivo.close();
                return nombre;
            }
        }
        archivo.close();
        return "";
    };

    // Lambda: nombre del feriado movil si aplica
    auto nombreFeriadoMovil = [&normalizar](auto& self, int dEval, int mEval, int aEval) -> std::string {
        int A = aEval % 19; int B = aEval % 4;  int C = aEval % 7;
        int K = aEval / 100;
        int P = (13 + 8 * K) / 25; int Q = K / 4;
        int M = (15 - P + K - Q) % 30; int N = (4 + K - Q) % 7;
        int D = (19 * A + M) % 30; int E = (2 * B + 4 * C + 6 * D + N) % 7;
        int mesPascua = 3; int diaPascua = 22 + D + E;
        if (diaPascua > 31) { mesPascua = 4; diaPascua = D + E - 9; }
        if (mesPascua == 4 && diaPascua == 26) diaPascua = 19;
        if (mesPascua == 4 && diaPascua == 25 && D == 28 && E == 6 &&
            ((11 * M + 11) % 30 < 19)) diaPascua = 18;

        std::tm pascua = {0};
        pascua.tm_year = aEval - 1900; pascua.tm_mon = mesPascua - 1; pascua.tm_mday = diaPascua;
        normalizar(&pascua);

        std::tm lunCarnaval = pascua; lunCarnaval.tm_mday -= 48; normalizar(&lunCarnaval);
        std::tm marCarnaval = pascua; marCarnaval.tm_mday -= 47; normalizar(&marCarnaval);
        std::tm vieSanto    = pascua; vieSanto.tm_mday    -= 2;  normalizar(&vieSanto);

        if (dEval == lunCarnaval.tm_mday && mEval == lunCarnaval.tm_mon + 1) return "Lunes de Carnaval";
        if (dEval == marCarnaval.tm_mday && mEval == marCarnaval.tm_mon + 1) return "Martes de Carnaval";
        if (dEval == vieSanto.tm_mday    && mEval == vieSanto.tm_mon + 1)    return "Viernes Santo";
        return "";
    };

    // ---- PROCESAMIENTO PRINCIPAL ----
    normalizar(fecha);

    int dOriginal = fecha->tm_mday;
    int mOriginal = fecha->tm_mon + 1;
    int aOriginal = fecha->tm_year + 1900;
    int wOriginal = fecha->tm_wday;

    // Detectar razon inicial del ajuste (si la hay)
    std::string razonInicial = "";

    if (wOriginal == 0 || wOriginal == 6) {
        // Es fin de semana
        std::string tipoDia = (wOriginal == 0) ? "Domingo" : "Sabado";
        std::ostringstream ss;
        ss << "La fecha " << dOriginal << "/" << mOriginal << "/" << aOriginal
           << " es " << tipoDia
           << ". No se asignan turnos en fin de semana.";
        razonInicial = ss.str();
    } else {
        // Verificar si es feriado
        std::string nomFijo = nombreFeriadoFijo(nombreFeriadoFijo,
                                                dOriginal, mOriginal, aOriginal);
        std::string nomMovil = nombreFeriadoMovil(nombreFeriadoMovil,
                                                  dOriginal, mOriginal, aOriginal);
        std::string nomFeriado = nomFijo.empty() ? nomMovil : nomFijo;

        if (!nomFeriado.empty()) {
            std::ostringstream ss;
            ss << "La fecha " << dOriginal << "/" << mOriginal << "/" << aOriginal
               << " corresponde al feriado: " << nomFeriado << ".";
            razonInicial = ss.str();
        }
    }

    // Bucle de ajuste: empuja hasta encontrar un dia laborable
    bool ajustado = true;
    while (ajustado) {
        ajustado = false;

        // Fin de semana -> empujar al Lunes
        if (fecha->tm_wday == 0) {
            fecha->tm_mday += 1;
            normalizar(fecha);
            ajustado = true;
        } else if (fecha->tm_wday == 6) {
            fecha->tm_mday += 2;
            normalizar(fecha);
            ajustado = true;
        }

        // Leer feriados desde archivo y verificar si la fecha actual es feriado
        {
            int dEval = fecha->tm_mday;
            int mEval = fecha->tm_mon + 1;
            int aEval = fecha->tm_year + 1900;

            bool esFeriado = false;

            // Verificar feriados fijos desde feriados.txt
            std::ifstream arch("feriados.txt");
            if (arch.is_open()) {
                std::string linea;
                while (std::getline(arch, linea)) {
                    if (linea.empty() || linea[0] == '#') continue;
                    std::istringstream ss(linea);
                    int dF, mF;
                    if (!(ss >> dF >> mF)) continue;
                    if (esFeriadoFijo(esFeriadoFijo, dEval, mEval, aEval, dF, mF)) {
                        esFeriado = true;
                        break;
                    }
                }
                arch.close();
            }

            // Verificar feriados moviles (Carnaval / Viernes Santo)
            if (!esFeriado)
                esFeriado = esFeriadoMovil(esFeriadoMovil, dEval, mEval, aEval);

            if (esFeriado) {
                fecha->tm_mday += 1;
                normalizar(fecha);
                ajustado = true;
            }
        }
    }

    return razonInicial;
}
