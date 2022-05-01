#ifndef CAMADA_ENLACE_HPP_INCLUDED
#define CAMADA_ENLACE_HPP_INCLUDED

#define CONTAGEM_DE_CARACTERES 0
#define INSERCAO_DE_BYTES 1

#define BIT_DE_PARIDADE_PAR 0
#define CRC 1
#define CODIGO_DE_HAMMING 2

#define TAMANHO_QUADRO 3

#define BYTE_DE_FLAG 0
#define ESC 27 // Valor em decimal para caracter ESC na tabela ASCII

#include "CamadaFisica.hpp"

#include <stdint.h>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

#define POLINOMIO_GERADOR vector<int> {1, 1, 0, 1}

extern uint8_t TIPO_DE_ENQUADRAMENTO;
extern uint8_t TIPO_DE_CONTROLE_DE_ERRO;

vector<int> NumberToByte(int number);
int ByteToNumber(vector<int> byte);

void CamadaEnlaceDadosTransmissora(vector<int> quadro);
vector<int> CamadaEnlaceDadosTransmissoraEnquadramento(vector<int> quadro);

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes (vector<int> quadro);
vector<int> CamadaEnlaceDadosReceptoraEnquadramentoInsercaoDeBytes (vector<int> quadro);

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoContagemDeCaracteres (vector<int> quadro);
vector<int> CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(vector<int> quadro);

vector<int> CamadaEnlaceDadosTransmissoraControleDeErro(vector<int> quadro);

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(vector<int> quadro);
vector<int> CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(vector<int> quadro);

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCRC(vector<int> quadro);
vector<int> CamadaEnlaceDadosReceptoraControleDeErroCRC(vector<int> quadro);

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCodigoDeHamming(vector<int> quadro);
vector<int> CamadaEnlaceDadosReceptoraControleDeErroCodigoDeHamming(vector<int> quadro);


void CamadaEnlaceDadosReceptora(vector<int> quadro);

vector<int> CamadaEnlaceDadosReceptoraEnquadramento (vector<int> quadro);

vector<int> CamadaEnlaceDadosReceptoraControleDeErro(vector<int> quadro);


#endif