#ifndef CAMADA_ENLACE_HPP_INCLUDED
#define CAMADA_ENLACE_HPP_INCLUDED

#define CONTAGEM_DE_CARACTERES 0
#define INSERCAO_DE_BYTES 1

#define BIT_DE_PARIDADE_PAR 0
#define CRC 1
#define CODIGO_DE_HAMMING 2

#define TAMANHO_QUADRO_BYTES 3

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

//Funções Auxiliares

vector<int> NumberToByte(int number);
int ByteToNumber(vector<int> byte);
vector<int> VectorXor(vector<int> vetor1, vector<int> vetor2);
vector<int> DeleteFirstElement(vector<int> vetor);

//Funções da Camda de Enlace de Dados

void CamadaEnlaceDadosTransmissora(vector<int> quadros);
vector<int> CamadaEnlaceDadosTransmissoraEnquadramento(vector<int> quadros);

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes (vector<int> quadros);
vector<int> CamadaEnlaceDadosReceptoraEnquadramentoInsercaoDeBytes (vector<int> quadros);

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoContagemDeCaracteres (vector<int> quadros);
vector<int> CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(vector<int> quadros);

vector<int> CamadaEnlaceDadosTransmissoraControleDeErro(vector<int> quadros);

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(vector<int> quadros);
vector<int> CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(vector<int> quadros);

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCRC(vector<int> quadros);
vector<int> CamadaEnlaceDadosReceptoraControleDeErroCRC(vector<int> quadros);

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCodigoDeHamming(vector<int> quadros);
vector<int> CamadaEnlaceDadosReceptoraControleDeErroCodigoDeHamming(vector<int> quadros);


void CamadaEnlaceDadosReceptora(vector<int> quadros);

vector<int> CamadaEnlaceDadosReceptoraEnquadramento (vector<int> quadros);

vector<int> CamadaEnlaceDadosReceptoraControleDeErro(vector<int> quadros);


#endif