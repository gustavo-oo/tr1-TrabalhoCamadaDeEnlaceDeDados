#ifndef CAMADA_FISICA_HPP_INCLUDED
#define CAMADA_FISICA_HPP_INCLUDED

#define CODIFICACAO_BINARIA     0
#define CODIFICACAO_MANCHESTER  1
#define CODIFICACAO_BIPOLAR     2

#define VOLTAGEM_POSITIVA 1
#define VOLTAGEM_ZERO 0
#define VOLTAGEM_NEGATIVA -1

#define BIT_0 0
#define BIT_1 1

#include <stdint.h>
#include <vector>
#include <string>

using namespace std;

//Variavel que define a codificação a ser utilizada
extern uint8_t TIPO_DE_CODIFICACAO;

//Funções Auxiliares
vector<int> StringToBits(string mensagem);
string BitsToString(vector<int> quadro);
void PrintVector(vector<int> vetor);
void TransmitirMensagem(string mensagem);
vector<int> InitClock(int tamanho);

//Funções de Transmissão
void AplicacaoTransmissora();
void CamadaDeAplicacaoTransmissora(string mensagem);
void CamadaFisicaTransmissora(vector<int> quadro);
void MeioDeComunicacao(vector<int> fluxo_bruto_de_bits);

//Funções de Codificação
vector<int> CamadaFisicaTransmissoraCodificacaoBinaria(vector<int> quadro);
vector<int> CamadaFisicaTransmissoraCodificacaoManchester(vector<int> quadro);
vector<int> CamadaFisicaTransmissoraCodificacaoBipolar(vector<int> quadro);

//Funções de Recepção
void CamadaFisicaReceptora(vector<int> quadro);
void CamadaDeAplicacaoReceptora(vector<int> quadro);
void AplicacaoReceptora(string mensagem);

//Funções de Decodificação
vector<int> CamadaFisicaReceptoraDecodificacaoBinaria(vector<int> quadro);
vector<int> CamadaFisicaReceptoraDecodificacaoManchester(vector<int> quadro);
vector<int> CamadaFisicaReceptoraDecodificacaoBipolar(vector<int> quadro);

#endif
