#include "CamadaFisica.hpp"
#include "CamadaEnlace.hpp"

#include <iostream>
#include <bitset>
#include <math.h>

uint8_t TIPO_DE_ENQUADRAMENTO = INSERCAO_DE_BYTES; //mudar depois
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

void CamadaEnlaceDadosTransmissora(vector<int> quadros){
    vector<int> quadrosEnquadrados;
    
    quadrosEnquadrados = CamadaEnlaceDadosTransmissoraEnquadramento(quadros);

    quadrosEnquadrados = CamadaEnlaceDadosTransmissoraControleDeErro(quadrosEnquadrados);

    CamadaFisicaTransmissora(quadrosEnquadrados);
}

vector<int> CamadaEnlaceDadosTransmissoraEnquadramento(vector<int> quadros){
    vector<int> quadrosEnquadrados;

    switch(TIPO_DE_ENQUADRAMENTO){
        case CONTAGEM_DE_CARACTERES:
            quadrosEnquadrados = CamadaEnlaceDadosTransmissoraEnquadramentoContagemDeCaracteres(quadros);
            break;

        case INSERCAO_DE_BYTES:
            quadrosEnquadrados = CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes(quadros);
            break;
    }

    return quadrosEnquadrados;
}

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoContagemDeCaracteres(vector<int> quadros){
    vector<int> quadroAtual = {};
    vector<int> quadrosEnfileirados = {};
    vector <int> quadroEnquadrado = {};
    int quantidadeBytes;

    for(int j = 0; j < quadros.size(); j += TAMANHO_QUADRO_BYTES * 8){

        if(j + TAMANHO_QUADRO_BYTES * 8 > quadros.size()){
            quadroAtual = vector<int> (quadros.begin() + j, quadros.end());
        }else{
            quadroAtual = vector<int> (quadros.begin() + j, quadros.begin() + j + TAMANHO_QUADRO_BYTES * 8);
        }

        quantidadeBytes = quadroAtual.size()/8;

        quadroEnquadrado = NumberToByte(quantidadeBytes);

        for(int i = 0; i < quantidadeBytes * 8; i++){
            quadroEnquadrado.push_back(quadroAtual[i]);
        }

        quadrosEnfileirados.insert(quadrosEnfileirados.end(), quadroEnquadrado.begin(), quadroEnquadrado.end());
    }

    return quadrosEnfileirados;
}

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes(vector<int> quadros) {
    vector <int> quadroEnquadrado = {}, quadroAtual = {}, quadrosEnfileirados = {};
    vector <int> flag = NumberToByte(BYTE_DE_FLAG);
    vector <int> esc = NumberToByte(ESC);
    vector <int> current_byte = {};
    int esc_inserted = 0;

    for(int j = 0; j < quadros.size(); j += TAMANHO_QUADRO_BYTES * 8){

        if(j + TAMANHO_QUADRO_BYTES * 8 > quadros.size()){
            quadroAtual = vector<int> (quadros.begin() + j, quadros.end());
        }else{
            quadroAtual = vector<int> (quadros.begin() + j, quadros.begin() + j + TAMANHO_QUADRO_BYTES * 8);
        }

        quadroEnquadrado = flag;

        for (int i = 0; i < quadroAtual.size(); i++){
            if (i % 8 == 0){
                current_byte = vector<int> (quadroAtual.begin() + i, quadroAtual.begin() + i + 8);
                esc_inserted = 0;
            }
            if ((current_byte == flag || current_byte == esc) && esc_inserted == 0){
                quadroEnquadrado.insert(quadroEnquadrado.end(), esc.begin(), esc.end());
                esc_inserted = 1;
            }
            quadroEnquadrado.push_back(quadroAtual[i]);
        }
        quadroEnquadrado.insert(quadroEnquadrado.end(), flag.begin(), flag.end());

        quadrosEnfileirados.insert(quadrosEnfileirados.end(), quadroEnquadrado.begin(), quadroEnquadrado.end());
    }

    return quadrosEnfileirados;
}

vector<int> CamadaEnlaceDadosReceptoraEnquadramentoInsercaoDeBytes(vector<int> quadro) {
    vector <int> quadroDesenquadrado = {};

    vector <int> flag = NumberToByte(BYTE_DE_FLAG);
    vector <int> esc = NumberToByte(ESC);
    vector <int> previous_byte = {};
    vector <int> current_byte = {};
    int read = 0, stop = 0;

    // Loop sem os elementos das pontas do quadro, por serem sempre o byte de flag'
    for (int i = 0; i < quadro.size(); i += 8){
        current_byte = vector<int> (quadro.begin() + i, quadro.begin() + i + 8);

        if(i >= 8){
            previous_byte = vector<int> (quadro.begin() + i - 8, quadro.begin() + i);
        }
    

        if(read){
            if(current_byte == flag && previous_byte != esc){
                stop = 1;
            }else{
                if (current_byte != esc || previous_byte == esc){
                    quadroDesenquadrado.insert(quadroDesenquadrado.end(), current_byte.begin(), current_byte.end());
                }
            } 
        }

        if(current_byte == flag && previous_byte != esc && !stop){
            read = 1;
        }

        if(stop){
            read = 0;
            stop = 0;
        }

    }

    return quadroDesenquadrado;
}

void CamadaEnlaceDadosReceptora (vector<int> quadro){
    quadro = CamadaEnlaceDadosReceptoraControleDeErro(quadro);

    quadro = CamadaEnlaceDadosReceptoraEnquadramento(quadro);

    CamadaDeAplicacaoReceptora(quadro);
}

vector<int> CamadaEnlaceDadosReceptoraEnquadramento (vector<int> quadros){
    vector<int> quadrosDesenquadrados;

    switch (TIPO_DE_ENQUADRAMENTO){
    case CONTAGEM_DE_CARACTERES:
        quadrosDesenquadrados = CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(quadros);
        break;
    
    case INSERCAO_DE_BYTES:
        quadrosDesenquadrados = CamadaEnlaceDadosReceptoraEnquadramentoInsercaoDeBytes(quadros);
        break;
    }

    return quadrosDesenquadrados;
}


vector<int> CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(vector<int> quadros){
    vector<int> quadroDesenquadrado = {};
    vector<int> quadrosEnfileirados = {};
    vector<int> contagemCaracteresEmBits = {};
    int contagemCaracteres = 0;
    int i;

    cout << "\nQUADROS:";
    PrintVector(quadros);
    cout << "\n";

    for(int j = 0;;){
        contagemCaracteresEmBits = vector<int> (quadros.begin() + j, quadros.begin() + j + 8);
        j += 8;

        contagemCaracteres = ByteToNumber(contagemCaracteresEmBits) * 8;

        for(i = j; i < j + contagemCaracteres; i++){
            quadroDesenquadrado.push_back(quadros[i]);
        }

        if(i == quadros.size()){
            break;
        }

        j = i;
        i = 0;
    }

    quadrosEnfileirados.insert(quadrosEnfileirados.end(), quadroDesenquadrado.begin(), quadroDesenquadrado.end());

    return quadrosEnfileirados;
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

