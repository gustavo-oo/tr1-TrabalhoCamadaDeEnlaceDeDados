#ifndef CAMADA_ENLACE_HPP_INCLUDED
#define CAMADA_ENLACE_HPP_INCLUDED

#define CONTAGEM_DE_CARACTERES 0
#define INSERCAO_DE_BYTES 1
#define CODIGO_DE_HAMMING 2

#include "CamadaFisica.hpp"

#include <vector>

using namespace std;

extern uint8_t TIPO_DE_ENQUADRAMENTO;

void CamadaEnlaceDadosTransmissora(vector<int> quadro);
vector<int> CamadaEnlaceDadosTransmissoraEnquadramento(vector<int> quadro);

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes (vector<int> quadro);
vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoContagemDeCaracteres (vector<int> quadro);

vector<int> CamadaEnlaceDadosTransmissoraControleDeErro(vector<int> quadro);


#endif