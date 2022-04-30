#include "CamadaFisica.hpp"
#include "CamadaEnlace.hpp"

uint8_t TIPO_DE_ENQUADRAMENTO = 0; //mudar depois

void CamadaEnlaceDadosTransmissora(vector<int> quadro){

    quadro = CamadaEnlaceDadosTransmissoraEnquadramento(quadro);

    quadro = CamadaEnlaceDadosTransmissoraControleDeErro(quadro);

    CamadaFisicaTransmissora(quadro);
}

vector<int> CamadaEnlaceDadosTransmissoraEnquadramento(vector<int> quadro){
    vector<int> quadroEnquadrado;

    switch(TIPO_DE_ENQUADRAMENTO){
        case CONTAGEM_DE_CARACTERES:
            quadroEnquadrado = CamadaEnlaceDadosTransmissoraEnquadramentoContagemDeCaracteres(quadro);
            break;

        case INSERCAO_DE_BYTES:
            quadroEnquadrado = CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes(quadro);
            break;

        case CODIGO_DE_HAMMING:
            break;
    }

    return quadroEnquadrado;
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErro(vector<int> quadro){
    return quadro;
}

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoContagemDeCaracteres(vector<int> quadro){
    return quadro;
}

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes(vector<int> quadro){
    return quadro;
}
