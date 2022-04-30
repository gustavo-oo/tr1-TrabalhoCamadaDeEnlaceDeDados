#include "CamadaFisica.hpp"
#include "CamadaEnlace.hpp"

#include <iostream>
#include <bitset>
#include <math.h>

uint8_t TIPO_DE_ENQUADRAMENTO = CONTAGEM_DE_CARACTERES; //mudar depois

vector<int> NumberToByte(int number){
    bitset<8> byte;
    vector<int> byteInVector = {};

    byte = number;

    for(int j = 7; j > -1; j--){
        byteInVector.push_back(byte[j]);
    }

    return byteInVector;
}

int ByteToNumber(vector<int> byte){
    int number = 0;

    for(int i = 0; i < byte.size(); i++){
        number += byte[i] * pow(2, 7 - (i%8));
    }

    return number;
}

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

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoContagemDeCaracteres(vector<int> quadro){
    int contagemCaracteres = quadro.size();

    vector <int> quadroEnquadrado = NumberToByte(contagemCaracteres);

    for(int i = 0; i < quadro.size(); i++){
        quadroEnquadrado.push_back(quadro[i]);
    }

    return quadroEnquadrado;
}

vector<int> CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(vector<int> quadro){
    vector<int> quadroDesenquadrado = {};

    vector<int> contagemCaracteresEmBits = vector<int> (quadro.begin(), quadro.begin() + 7);

    int contagemCaracteres = ByteToNumber(contagemCaracteresEmBits);

    for(int i = 8; i < contagemCaracteres + 8; i++){
        quadroDesenquadrado.push_back(quadro[i]);
    }

    return quadroDesenquadrado;
}

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes(vector<int> quadro) {
    vector <int> quadroEnquadrado = {};
    quadroEnquadrado.push_back(BYTE_DE_FLAG);

    for (int i = 0; i < quadro.size(); i++){
        if (quadro[i] == BYTE_DE_FLAG || quadro[i] == ESC)
            quadroEnquadrado.push_back(ESC);
        quadroEnquadrado.push_back(quadro[i]);
    }
    quadroEnquadrado.push_back(BYTE_DE_FLAG);

    return quadroEnquadrado;
}

vector<int> CamadaEnlaceDadosReceptoraEnquadramentoInsercaoDeBytes(vector<int> quadro) {
    vector <int> quadroDesenquadrado = {};

    // Loop sem os elementos das pontas do quadro, por serem sempre o byte de flag
    for (int i = 1; i < quadro.size() - 1; i++){
        if (quadro[i] != ESC)
            quadroDesenquadrado.push_back(quadro[i]);
        else if (quadro[i - 1] == ESC)
            quadroDesenquadrado.push_back(quadro[i]);
    }

    return quadroDesenquadrado;
}

vector<int> CamadaEnlaceDadosReceptoraEnquadramento (vector<int> quadro){
    vector<int> quadroDesenquadrado;

    switch (TIPO_DE_ENQUADRAMENTO){
    case CONTAGEM_DE_CARACTERES:
        quadroDesenquadrado = CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(quadro);
        break;
    
    case INSERCAO_DE_BYTES:
        quadroDesenquadrado = CamadaEnlaceDadosReceptoraEnquadramentoInsercaoDeBytes(quadro);
        break;
    }

    return quadroDesenquadrado;
}

void CamadaEnlaceDadosReceptora (vector<int> quadro){
    quadro = CamadaEnlaceDadosReceptoraEnquadramento(quadro);

    quadro = CamadaEnlaceDadosReceptoraControleDeErro(quadro);

    CamadaDeAplicacaoReceptora(quadro);
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErro(vector<int> quadro){
    return quadro;
}

vector<int> CamadaEnlaceDadosReceptoraControleDeErro(vector<int> quadro){
    return quadro;
}
