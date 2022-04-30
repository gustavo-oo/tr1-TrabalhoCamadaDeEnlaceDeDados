#ifndef CAMADA_ENLACE_HPP_INCLUDED
#define CAMADA_ENLACE_HPP_INCLUDED

#define CONTAGEM_DE_CARACTERES 0
#define INSERCAO_DE_BYTES 1
#define CODIGO_DE_HAMMING 2

#define BYTE_DE_FLAG 0
#define ESC 27 // Valor em decimal para caracter ESC na tabela ASCII

#include "CamadaFisica.hpp"

#include <stdint.h>
#include <vector>

using namespace std;

extern uint8_t TIPO_DE_ENQUADRAMENTO;

void CamadaEnlaceDadosTransmissora(vector<int> quadro);
vector<int> CamadaEnlaceDadosTransmissoraEnquadramento(vector<int> quadro);

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes (vector<int> quadro);
vector<int> CamadaEnlaceDadosReceptoraEnquadramentoInsercaoDeBytes (vector<int> quadro);

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoContagemDeCaracteres (vector<int> quadro);
vector<int> CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(vector<int> quadro);

vector<int> CamadaEnlaceDadosTransmissoraControleDeErro(vector<int> quadro);


void CamadaEnlaceDadosReceptora(vector<int> quadro);

vector<int> CamadaEnlaceDadosReceptoraEnquadramento (vector<int> quadro);


vector<int> CamadaEnlaceDadosReceptoraControleDeErro(vector<int> quadro);


#endif