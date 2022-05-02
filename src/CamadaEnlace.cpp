#include "CamadaFisica.hpp"
#include "CamadaEnlace.hpp"

#include <iostream>
#include <bitset>
#include <math.h>

uint8_t TIPO_DE_ENQUADRAMENTO = INSERCAO_DE_BYTES; //CONTAGEM_DE_CARACTERES ou INSERCAO_DE_BYTES
uint8_t TIPO_DE_CONTROLE_DE_ERRO = BIT_DE_PARIDADE_PAR; //BIT_DE_PARIDADE_PAR, CRC ou CODIGO_DE_HAMMING

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

    cout << "Quadros com Contagem de Caracteres: ";
    PrintVector(quadrosEnfileirados);

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

    cout << "Quadros com Insercao de Bytes: ";
    PrintVector(quadrosEnfileirados);

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

    cout << "Quadros Desenquadrados Insercao de Bytes: ";
    PrintVector(quadroDesenquadrado);

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

    cout << "Quadros Desenquadrados Contagem de Caracteres: ";
    PrintVector(quadrosEnfileirados);

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

    cout << "Quadros com Bit de Paridade: ";
    PrintVector(quadro);
   
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

    cout << "Quadros sem Bit de Paridade: ";
    PrintVector(quadroSemBitDeParidade);

    return quadroSemBitDeParidade;
}

int bitParidade(vector<int> bits, int powder){
    int shift = BIT_1 << powder;
    int bitDeParidade;
    int counter = 0;
    for(int i = 0; i < bits.size(); i++){
        if((i & shift) == shift){
            if(bits[i] == BIT_1){
                counter++;
            }
        }
    }

    bitDeParidade = counter % 2 == 0 ? BIT_0 : BIT_1;
    return bitDeParidade;
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCodigoDeHamming(vector<int> quadro){
    vector<int> quadroCodificado;
    int powder = 0;

    for(int i = 0; i < quadro.size(); i++){
        if(quadroCodificado.size() + 1 == pow(2,powder)){
            quadroCodificado.push_back(bitParidade(quadro, powder));
            powder++;
        } else
            quadroCodificado.push_back(quadro[i]);
    }
    cout << "QUADRO   ORIGINAL: ";
    PrintVector(quadro);
    cout << "QUADRO CODIFICADO: ";
    PrintVector(quadroCodificado);

    // Retornando o quadro para não quebrar nada,
    // uma vez que a funcao de recepcao não foi implementada
    return quadro; // return quadroCodificado;

}

vector<int> CamadaEnlaceDadosReceptoraControleDeErroCodigoDeHamming(vector<int> quadro){
    return quadro;
}

vector<int> VectorXor(vector<int> vetor1, vector<int> vetor2){
    vector<int> resultado = {};
    if(vetor1.size() == vetor2.size()){
        for(int i = 0; i < vetor1.size(); i++){
            resultado.push_back(vetor1[i] ^ vetor2[i%4]);
        }

        return resultado;
    }
    return {0};
}

vector<int> DeleteFirstElement(vector<int> vetor){
    vetor.erase(vetor.begin());
    return vetor;
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCRC(vector<int> quadro){
    vector<int> aux = vector<int> (quadro.begin(), quadro.begin() + POLINOMIO_GERADOR.size());
    vector<int> crc(POLINOMIO_GERADOR.size() - 1, 0);

    // Adicionando n bits 0 ao final do quadro
    quadro.insert(quadro.end(), crc.begin(), crc.end());

    for(int i = 0; i <= quadro.size() - POLINOMIO_GERADOR.size(); i++){
        if(aux[0] == BIT_0){
            aux = VectorXor(aux, vector<int> (POLINOMIO_GERADOR.size(), 0));
        }else{
            aux = VectorXor(aux, POLINOMIO_GERADOR);
        }

        aux = DeleteFirstElement(aux);
        if (i < quadro.size() - POLINOMIO_GERADOR.size())
            aux.push_back(quadro[POLINOMIO_GERADOR.size() + i]);
    }

    crc = aux;
  
    for (int i = 0; i < crc.size(); i++){
        quadro[quadro.size() - crc.size() + i] = crc[i];
    }

    cout << "Quadros com CRC: ";
    PrintVector(quadro);
    cout << endl;

    return quadro;
}


vector<int> CamadaEnlaceDadosReceptoraControleDeErroCRC(vector<int> quadro){
    vector<int> crc = vector<int> (quadro.end() - POLINOMIO_GERADOR.size() + 1, quadro.end());
    vector<int> quadroSemCRC = vector<int> (quadro.begin(), quadro.end() - crc.size());

    cout << "QUADRO TESTADO: ";
    PrintVector(quadro);
    cout << "Verificando CRC..." << endl;


    vector<int> aux = vector<int> (quadro.begin(), quadro.begin() + POLINOMIO_GERADOR.size());

    for(int i = 0; i <= quadro.size() - POLINOMIO_GERADOR.size(); i++){
        if(aux[0] == BIT_0){
            aux = VectorXor(aux, vector<int> (POLINOMIO_GERADOR.size(), 0));
        }else{
            aux = VectorXor(aux, POLINOMIO_GERADOR);
        }

        aux = DeleteFirstElement(aux);
        if (i < quadro.size() - POLINOMIO_GERADOR.size())
            aux.push_back(quadro[POLINOMIO_GERADOR.size() + i]);
    }

    crc = aux;

    if (crc != vector<int> (POLINOMIO_GERADOR.size() - 1, 0))
        cout << "ERRO DETECTADO!";

    return quadroSemCRC;
}

