#ifndef CAMADA_ENLACE_HPP_INCLUDED
#define CAMADA_ENLACE_HPP_INCLUDED

#include <stdint.h>
#include <vector>

using namespace std;

#define BYTE_DE_FLAG 0
#define ESC 27 // Valor em decimal para caracter ESC na tabela ASCII

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes (vector<int> quadro);
vector<int> CamadaEnlaceDadosReceptoraEnquadramentoInsercaoDeBytes (vector<int> quadro);
vector<int> CamadaEnlaceDadosReceptoraEnquadramento (vector<int> quadro);
vector<int> CamadaEnlaceDadosReceptora (vector<int> quadro);

#endif
