#include "indice.h"

using namespace std;

bool antecessor(long int rrnCurrent, int i, long int rrnDad, bool decreased, int MAX);
bool reconstituir(long int rrn, PAGE dad, int posPai, bool diminuiu, int MAX);
void deleteKey(int MAX);
bool excluirAux(string record, long int rrnCurrent, bool decreased, int MAX);
bool Delete(string record, long int rrnCurrent, bool decreased, int MAX);

