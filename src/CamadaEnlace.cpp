#include "CamadaFisica.hpp"
#include "CamadaEnlace.hpp"

uint8_t TIPO_DE_ENQUADRAMENTO = 0; //mudar depois

void CamadaEnlaceTransmissora(vector<int> quadro){

    quadro = CamadaEnlaceTransmissoraEnquadramento(quadro);

    quadro = CamadaEnlaceTransmissoraControleDeErro(quadro);

    CamadaFisicaTransmissora(quadro);
}

vector<int> CamadaEnlaceTransmissoraEnquadramento(vector<int> quadro){
    vector<int> quadroEnquadrado;

    switch(TIPO_DE_ENQUADRAMENTO){
        case CONTAGEM_DE_CARACTERES:
            quadroEnquadrado = CamadaEnlaceTransmissoraEnquadramentoContagemDeCaracteres(quadro);
            break;

        case INSERCAO_DE_BYTES:
            quadroEnquadrado = CamadaEnlaceTransmissoraEnquadramentoInsercaoDeBytes(quadro);
            break;

        case CODIGO_DE_HAMMING:
            break;
    }

    return quadroEnquadrado;
}

vector<int> CamadaEnlaceTransmissoraControleDeErro(vector<int> quadro){
    return quadro;
}

vector<int> CamadaEnlaceTransmissoraEnquadramentoContagemDeCaracteres(vector<int> quadro){
    return quadro;
}

vector<int> CamadaEnlaceTransmissoraEnquadramentoInsercaoDeBytes (vector<int> quadro){
    return quadro;
}
