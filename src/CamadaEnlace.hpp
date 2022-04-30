#ifndef CAMADA_ENLACE_HPP_INCLUDED
#define CAMADA_ENLACE_HPP_INCLUDED

#define CONTAGEM_DE_CARACTERES 0
#define INSERCAO_DE_BYTES 1
#define CODIGO_DE_HAMMING 2

#include "CamadaFisica.hpp"

#include <vector>

using namespace std;

extern uint8_t TIPO_DE_ENQUADRAMENTO;

void CamadaEnlaceTransmissora(vector<int> quadro);
vector<int> CamadaEnlaceTransmissoraEnquadramento(vector<int> quadro);

vector<int> CamadaEnlaceTransmissoraEnquadramentoContagemDeCaracteres(vector<int> quadro);
vector<int> CamadaEnlaceTransmissoraEnquadramentoInsercaoDeBytes(vector<int> quadro);

vector<int> CamadaEnlaceTransmissoraControleDeErro(vector<int> quadro);


#endif