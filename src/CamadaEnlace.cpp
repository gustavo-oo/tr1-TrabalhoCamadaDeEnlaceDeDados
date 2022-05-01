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
    int contagemCaracteres = quadro.size()/8;

    vector <int> quadroEnquadrado = NumberToByte(contagemCaracteres);

    for(int i = 0; i < quadro.size(); i++){
        quadroEnquadrado.push_back(quadro[i]);
    }

    return quadroEnquadrado;
}

vector<int> CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(vector<int> quadro){
    vector<int> quadroDesenquadrado = {};

    vector<int> contagemCaracteresEmBits = vector<int> (quadro.begin(), quadro.begin() + 8);

    int contagemCaracteres = ByteToNumber(contagemCaracteresEmBits) * 8;

    for(int i = 8; i < contagemCaracteres + 8; i++){
        quadroDesenquadrado.push_back(quadro[i]);
    }

    return quadroDesenquadrado;
}

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes(vector<int> quadro) {
    vector <int> quadroEnquadrado = {};
    vector <int> flag = NumberToByte(BYTE_DE_FLAG);
    vector <int> esc = NumberToByte(ESC);
    vector <int> current_byte = {};
    int esc_inserted = 0;

    quadroEnquadrado.insert(quadroEnquadrado.begin(), flag.begin(), flag.end());

    for (int i = 0; i < quadro.size(); i++){
        if (i % 8 == 0){
            current_byte = vector<int> (quadro.begin() + i, quadro.begin() + i + 8);
            esc_inserted = 0;
        }
        if ((current_byte == flag || current_byte == esc) && esc_inserted == 0){
            quadroEnquadrado.insert(quadroEnquadrado.begin(), esc.begin(), esc.end());
            esc_inserted = 1;
        }
        quadroEnquadrado.push_back(quadro[i]);
    }
    quadroEnquadrado.insert(quadroEnquadrado.begin(), flag.begin(), flag.end());

    return quadroEnquadrado;
}

vector<int> CamadaEnlaceDadosReceptoraEnquadramentoInsercaoDeBytes(vector<int> quadro) {
    vector <int> quadroDesenquadrado = {};

    vector <int> flag = NumberToByte(BYTE_DE_FLAG);
    vector <int> esc = NumberToByte(ESC);
    vector <int> previous_byte = {};
    vector <int> current_byte = {};

    // Loop sem os elementos das pontas do quadro, por serem sempre o byte de flag'
    for (int i = 8; i < quadro.size() - 8; i++){
        if (i % 8 == 0){
            current_byte = vector<int> (quadro.begin() + i, quadro.begin() + i + 8);
            previous_byte = vector<int> (quadro.begin() + i - 8, quadro.begin() + i);
        }
        if (current_byte != esc || previous_byte == esc)
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
