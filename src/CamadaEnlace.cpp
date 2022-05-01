#include "CamadaFisica.hpp"
#include "CamadaEnlace.hpp"

#include <iostream>
#include <bitset>
#include <math.h>

uint8_t TIPO_DE_ENQUADRAMENTO = CONTAGEM_DE_CARACTERES; //mudar depois
uint8_t TIPO_DE_CONTROLE_DE_ERRO = BIT_DE_PARIDADE_PAR;

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
    quadroEnquadrado.insert(quadroEnquadrado.end(), flag.begin(), flag.end());

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

void CamadaEnlaceDadosReceptora (vector<int> quadro){
    quadro = CamadaEnlaceDadosReceptoraEnquadramento(quadro);

    quadro = CamadaEnlaceDadosReceptoraControleDeErro(quadro);

    CamadaDeAplicacaoReceptora(quadro);
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


vector<int> CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(vector<int> quadro){
    vector<int> quadroDesenquadrado = {};

    vector<int> contagemCaracteresEmBits = vector<int> (quadro.begin(), quadro.begin() + 8);

    int contagemCaracteres = ByteToNumber(contagemCaracteresEmBits) * 8;

    int i;
    for(i = 8; i < contagemCaracteres + 8; i++){
        quadroDesenquadrado.push_back(quadro[i]);
    }

    //Bit de Paridade
    quadroDesenquadrado.push_back(quadro.back());

    return quadroDesenquadrado;
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErro(vector<int> quadro){
    vector<int> quadroComControleDeErro;

    switch(TIPO_DE_CONTROLE_DE_ERRO){
        case BIT_DE_PARIDADE_PAR:
            quadroComControleDeErro = CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(quadro);
            break;
        case CRC:
            quadroComControleDeErro = CamadaEnlaceDadosTransmissoraControleDeErroCRC(quadro);
            break;
        case CODIGO_DE_HAMMING:
            quadroComControleDeErro = CamadaEnlaceDadosTransmissoraControleDeErroCodigoDeHamming(quadro);
            break;
    }
    return quadroComControleDeErro;
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(vector<int> quadro){
    int counter = 0;

    for(int i = 0; i < quadro.size(); i++){
        if(quadro[i] == BIT_1){
            counter++;
        }
    }

    if(counter % 2 == 0){
        quadro.push_back(BIT_0);
    }else{
        quadro.push_back(BIT_1);
    }
   
    return quadro;
}

vector<int> CamadaEnlaceDadosReceptoraControleDeErro(vector<int> quadro){

    vector<int> quadroSemControleDeErro;

    switch(TIPO_DE_CONTROLE_DE_ERRO){
        case BIT_DE_PARIDADE_PAR:
            quadroSemControleDeErro = CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(quadro);
            break;
        case CRC:
            quadroSemControleDeErro = CamadaEnlaceDadosReceptoraControleDeErroCRC(quadro);
            break;
        case CODIGO_DE_HAMMING:
            quadroSemControleDeErro = CamadaEnlaceDadosReceptoraControleDeErroCodigoDeHamming(quadro);
            break;
    }

    return quadroSemControleDeErro;
}

vector<int> CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(vector<int> quadro){
    vector<int> quadroSemBitDeParidade = vector<int> (quadro.begin(), quadro.end() - 1);
    int counter = 0;

    for(int i = 0; i < quadro.size(); i++){
        if(quadro[i] == BIT_1){
            counter++;
        }
    }

    if(counter % 2 != 0){
        cout << "\n" << "ERRO DETECTADO" << endl; 
    }

    return quadroSemBitDeParidade;
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCodigoDeHamming(vector<int> quadro){
    return quadro;
}

vector<int> CamadaEnlaceDadosReceptoraControleDeErroCodigoDeHamming(vector<int> quadro){
    return quadro;
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCRC(vector<int> quadro){
    return quadro;
}
vector<int> CamadaEnlaceDadosReceptoraControleDeErroCRC(vector<int> quadro){
    return quadro;
}

